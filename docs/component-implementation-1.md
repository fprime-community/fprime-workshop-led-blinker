# LED Blinker: Initial Component Design and Implementation

The purpose of this exercise is to walk you through the creation and initial design and implementation of an F´ component to control the blinking of an LED. This section will discuss the design of the component, the implementation of a command to start/stop the LED blinking, and the sending of events.  Users will then proceed to the initial ground testing before finishing the implementation in a later section.

## Component Design

In order for our component to blink an LED, it needs to accept a command to turn on the LED and drive a GPIO pin via a port call to the GPIO driver. It will also need a [rate group](https://nasa.github.io/fprime/UsersGuide/best/rate-group.html) input port to control the timing of the blink. Additionally, we will define events and telemetry channels to report component state, and a parameter to control the period of the blink.

This component design is captured in the block diagram below with input ports on the left and output ports on the right. Ports for standard F´ functions (e.g. commands, events, telemetry, and parameters) are circled in green.

![Led Component Block Diagram](img/component-design.png)

In this exercise, the `BLINKING_ON_OFF` command shall toggle the blinking state of the LED. The period of the blinking is controlled by the `BLINK_INTERVAL` parameter. Blinking is implemented on the `run` rate group input port. The component also defines several telemetry channels and events describing the various actions taken by the component.

### Design Summary

**Component Ports:**
1. `run`: invoked at a set rate from the rate group, used to control the LED blinking
2. `gpioSet`: invoked by the `Led` component to control the GPIO driver

> Standard component ports (circled in green) are not listed here.

**Commands:**
1. `BLINKING_ON_OFF`: turn the LED blinking on/off

**Events:**
1. `SetBlinkingState`: emitted when the component sets the blink state
2. `BlinkIntervalSet`: emitted when the component blink interval parameter is set
3. `LedState`: emitted when the LED is driven to a new state

**Telemetry Channels:**
1. `BlinkingState`: state of the LED blinking
2. `LedTransitions`: count of the LED transitions

**Parameters:**
1. `BLINK_INTERVAL`: LED blink period in number of rate group calls

### Create the component

It is time to create the basic component. In a terminal, navigate to the project's root directory and run the following:

```bash
# In led-blinker
cd Components

fprime-util new --component
```
You will be prompted for information regarding your component. Fill out the prompts as shown below:

```bash
[INFO] Cookiecutter source: using builtin
  [1/8] Component name (MyComponent): Led
  [2/8] Component short description (Component for F Prime FSW framework.): Component to blink an LED driven by a rate group
  [3/8] Component namespace (Components): Components
  [4/8] Select component kind
    1 - active
    2 - passive
    3 - queued
    Choose from [1/2/3] (1): 1
  [5/8] Enable Commands?
    1 - yes
    2 - no
    Choose from [1/2] (1): 1
  [6/8] Enable Telemetry?
    1 - yes
    2 - no
    Choose from [1/2] (1): 1
  [7/8] Enable Events?
    1 - yes
    2 - no
    Choose from [1/2] (1): 1
  [8/8] Enable Parameters?
    1 - yes
    2 - no
    Choose from [1/2] (1): 1
[INFO] Found CMake file at 'led-blinker/Components/CMakeLists.txt'
Add Led to led-blinker/Components/CMakeLists.txt at end of file? (yes/no) [yes]: yes
Generate implementation files? (yes/no) [yes]: yes
Refreshing cache and generating implementation files...
[INFO] Created new component and generated initial implementations.
```
Your new component is located in the directory `led-blinker/Components/Led`.

### Commands

Commands are used to command the component from the ground system or a command sequencer. We will add a command named `BLINKING_ON_OFF` to turn on or off the blinking LED. This command will take in an argument named `onOff` of type `Fw.On`.


Inside your `led-blinker/Components/Led` directory, open the file `Led.fpp` and search for the following:

```
        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port
        @ TODO
        async command TODO opcode 0
```

Replace that block with the following:

```
        @ Command to turn on or off the blinking LED
        async command BLINKING_ON_OFF(
            onOff: Fw.On @< Indicates whether the blinking should be on or off
        )
```

### Events

Events represent a log of system activities. Events are typically emitted any time the system takes an action. Events are also emitted to report off-nominal conditions.

Inside your `led-blinker/Components/Led` directory, open the `Led.fpp` file. After the command you added in the previous section, add this event:

```
        @ Reports the state we set to blinking.
        event SetBlinkingState($state: Fw.On) \
            severity activity high \
            format "Set blinking state to {}."
```

> NOTE: `state` is a keyword in FPP. In order to use it as a variable name, you need to escape it by prepending `$`.

### Do it Yourself

Below is a table with tasks you must complete before moving on to the next section. These tasks require you to update the component's fpp file.

| Task | Solution |
|-------|-------------|
| 1. Add an activity high event named `BlinkIntervalSet` to the fpp. The event takes an argument of `U32` type to indicate the set interval. | <details><summary>Answer</summary>`event BlinkIntervalSet(interval: U32) severity activity high format "LED blink interval set to {}"`</details> |
| 2. Add an activity low event named `LedState` to the fpp. The event takes an argument of `Fw.On` type to indicate the LED has been driven to a different state. | <details><summary>Answer</summary>`event LedState(onOff: Fw.On) severity activity low format "LED is {}"`</details> |


You have completed the Command and Event design phase. We'll move on to the Command and Event implementation phase.

## Component Implementation

In the `led-blinker/Components/Led` directory, run the following:

```bash
# In led-blinker/Components/Led
fprime-util impl
```

This command will auto generate two files: `Led.template.hpp` and `Led.template.cpp`. These files contain the stub implementation for the component's newly added command.

Since this is the start of the component's implementation, we can use the generated template files for our initial component implementation. Inside your `led-blinker/Components/Led` directory, rename `Led.template.hpp` to `Led.hpp` and rename `Led.template.cpp` to `Led.cpp`. You can rename the files through the terminal using the two commands below:

```bash
# In led-blinker/Components/Led
mv Led.template.hpp Led.hpp
mv Led.template.cpp Led.cpp
```

Verify your component is building correctly by running the following command in the `led-blinker/Components/Led` directory.

```bash
# In led-blinker/Components/Led
fprime-util build
```

> Fix any errors that occur before proceeding with the rest of the tutorial.

### Component State

Many of the behaviors of the component discussed in the [Component Design](#component-design) section require the tracking of some state. Let us set up and initialize that state.

Open `Led.hpp` in `led-blinker/Components/Led`. Add the following private member variables to the end of the file just before the two closing `}` of the class definition and namespace.

```cpp
    Fw::On m_state = Fw::On::OFF; //! Keeps track if LED is on or off
    U64 m_transitions = 0; //! The number of on/off transitions that have occurred from FSW boot up
    U32 m_toggleCounter = 0; //! Keeps track of how many ticks the LED has been on for
    bool m_blinking = false; //! Flag: if true then LED blinking will occur else no blinking will happen
```

Run the following in the `led-blinker/Components/Led` directory to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```

Now that the member variables are set up, we can continue into the component implementation.

### Command

Now we will implement the behavior of the `BLINKING_ON_OFF` command. An initial implementation is shown below and may be copied into `Led.cpp` in-place of the BLINKING_ON_OFF command stub.

```cpp
void Led ::BLINKING_ON_OFF_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, Fw::On onOff) {
    this->m_toggleCounter = 0;               // Reset count on any successful command
    this->m_blinking = Fw::On::ON == onOff;  // Update blinking state

    // TODO: Emit an event SetBlinkingState to report the blinking state (onOff).
    // NOTE: This event will be added during the "Events" exercise.

    // TODO: Report the blinking state (onOff) on channel BlinkingState.
    // NOTE: This telemetry channel will be added during the "Telemetry" exercise.

    // Provide command response
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}
```
Run the following command in the terminal to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```

> Fix any errors that occur before proceeding with the rest of the tutorial.

### Events

Open `Led.cpp` in your `led-blinker/Components/Led` directory and navigate to the `BLINKING_ON_OFF` command. Report, via an event, the blinking state has been set.

To do so, replace:
```cpp
      // TODO: Emit an event SetBlinkingState to report the blinking state (onOff).
      // NOTE: This event will be added during the "Events" exercise.
```

with:
```cpp
    this->log_ACTIVITY_HI_SetBlinkingState(onOff);
```

Run the following to verify your component is building correctly.

```bash
fprime-util build
```

> Resolve any `fprime-util build` errors before continuing

## Conclusion

Congratulations!  You have now implemented some basic functionality in a new F´ component. Your command should look like this
```cpp
void Led ::BLINKING_ON_OFF_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, Fw::On onOff) {
    this->m_toggleCounter = 0;               // Reset count on any successful command
    this->m_blinking = Fw::On::ON == onOff;  // Update blinking state

    this->log_ACTIVITY_HI_SetBlinkingState(onOff);

    // TODO: Report the blinking state (onOff) on channel BlinkingState.
    // NOTE: This telemetry channel will be added during the "Telemetry" exercise.

    // Provide command response
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}
```

Before finishing the implementation, let's take a break and try running the above command through the ground system. This will require integrating the component into the system topology, which we will get into in the next section.

> NOTE: The last TODO in the `BLINKING_ON_OFF` command handler will be addressed in a future section. 

### Next Step: [Initial Component Integration](./initial-integration.md).
