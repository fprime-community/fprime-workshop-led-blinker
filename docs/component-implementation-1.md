# LED Blinker: Component Design and Initial Implementation

The purpose of this exercise is to walk you through the creation and initial implementation of an F´ component to control the blinking of an LED. This section will discuss the design of the full component, the implementation of a command to start/stop the LED blinking, and the transmission of a telemetry channel.  Users will then proceed to the initial ground testing before finishing the implementation in a later section.

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

**Telemetry Channels:**
1. `BlinkingState`: state of the LED blinking
2. `LedTransitions`: count of the LED transitions

**Events:**
1. `InvalidBlinkArgument`: emitted when an invalid argument was supplied to the `BLINKING_ON_OFF` command
2. `SetBlinkingState`: emitted when the component sets the blink state
3. `BlinkIntervalSet`: emitted when the component blink interval parameter is set
4. `LedState`: emitted when the LED is driven to a new state

**Parameters:**
1. `BLINK_INTERVAL`: LED blink period in number of rate group calls

## Create the component

It is time to create the basic component. In a terminal, navigate to the project's root directory and run the following:

```bash
# In led-blinker
mkdir -p Components
cd Components

fprime-util new --component
```
You will be prompted for information regarding your component. Fill out the prompts as shown below:

```bash
[INFO] Cookiecutter source: using builtin
component_name [MyComponent]: Led
component_short_description [Example Component for F Prime FSW framework.]: Component to blink an LED driven by a rate group
component_namespace [Led]: Components
Select component_kind:
1 - active
2 - passive
3 - queued
Choose from 1, 2, 3 [1]: 1
Select enable_commands:
1 - yes
2 - no
Choose from 1, 2 [1]: 1  
Select enable_telemetry:
1 - yes
2 - no
Choose from 1, 2 [1]: 1
Select enable_events:
1 - yes
2 - no
Choose from 1, 2 [1]: 1
Select enable_parameters:
1 - yes
2 - no
Choose from 1, 2 [1]: 1
[INFO] Found CMake file at 'LedBLinker/project.cmake'
Add component Led to led-blinker/project.cmake at end of file (yes/no)? yes
Generate implementation files (yes/no)? yes
```
Your new component is located in the directory `led-blinker/Components/Led`.

## Component State

Many of the behaviors of the component discussed in the [Component Design](#component-design) section require the tracking of some state. Before diving into the implementation of the behavior let us set up and initialize that state.

Open `Led.hpp` in `led-blinker/Components/Led`, and add the following private member variables to the end of the file.

```cpp
    Os::Mutex lock; //! Protects our data from thread race conditions
    Fw::On state; //! Keeps track if LED is on or off
    U64 transitions; //! The number of on/off transitions that have occurred from FSW boot up
    U32 count; //! Keeps track of how many ticks the LED has been on for
    bool blinking; //! Flag: if true then LED blinking will occur else no blinking will happen
```

Add the following to the top of the `Led.hpp`:

```c++
#include <Os/Mutex.hpp>
```

Open `Led.cpp` in `led-blinker/Components/Led`, and initialize your member variables in the constructor:

```cpp
Led ::Led(const char* const compName) : LedComponentBase(compName),
    state(Fw::On::OFF),
    transitions(0),
    count(0),
    blinking(false)
{}
```

Now that the member variables are set up, we can continue into the component implementation.

> The above code will fail to find the `Fw::On` enum type until we use it in the FPP model in the next section. To fix immediately, add `#include <Fw/Types/OnEnumAc.hpp>` to the top of `Led.hpp`.

### Commands

Commands are used to command the component from the ground system or a command sequencer. We will add a command named `BLINKING_ON_OFF` to turn on or off the blinking LED. This command will take in an argument named `on_off` of type `Fw.On`.


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
            on_off: Fw.On @< Indicates whether the blinking should be on or off
        )
```

Exit the text editor, and run the following in the `led-blinker/Components/Led` directory:

```bash
# In led-blinker/Components/Led
fprime-util impl
```

This command will auto generate two files: Led.hpp-template and Led.cpp-template. These files contain the stub implementation for the component. These should now include stubs for this newly added command.

Inside your `led-blinker/Components/Led` directory, open `Led.hpp-template` and copy the following block of code. Paste it in replacement of the `TODO_cmdHandler` block in `Led.hpp`.

```cpp
      //! Implementation for BLINKING_ON_OFF command handler
      //! Command to turn on or off the blinking LED
      void BLINKING_ON_OFF_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          Fw::On on_off /*!< 
          Indicates whether the blinking should be on or off
          */
      );
```

Inside your `led-blinker/Components/Led` directory, open `Led.cpp-template` and copy the following block of code and paste it into `Led.cpp` replacing the `TODO_cmdHandler` block.

```cpp
  void Led ::
    BLINKING_ON_OFF_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Fw::On on_off
    )
  {
    // TODO
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }
```

> This pattern of copying implementations from *-template files into our cpp and hpp files will be repeated throughout the rest of this tutorial.


Now we will implement the behavior of the `BLINKING_ON_OFF` command. An initial implementation is shown below and may be copied into `Led.cpp` in-place of the stub we just copied in.

```cpp
  void Led ::
    BLINKING_ON_OFF_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Fw::On on_off
    )
  {
    // Create a variable to represent the command response
    auto cmdResp = Fw::CmdResponse::OK;

    // Verify if on_off is a valid argument.
    // Note: isValid is an autogenerate helper function for enums defined in fpp.
    if(!on_off.isValid())
    {
        // TODO: Add an event that indicates we received an invalid argument.
        // NOTE: Add this event after going through the "Events" exercise.

        // Update command response with a validation error
        cmdResp = Fw::CmdResponse::VALIDATION_ERROR;
    }
    else
    {
      this->count = 0; // Reset count on any successful command
      this->lock.lock();
      this->blinking = Fw::On::ON == on_off; // Update blinking state
      this->lock.unlock();

      // TODO: Add an event that reports the state we set to blinking.
      // NOTE: This event will be added during the "Events" exercise.

      // TODO: Report the blinking state via a telemetry channel.
      // NOTE: This telemetry channel will be added during the "Telemetry" exercise.
    }

    // Provide command response
    this->cmdResponse_out(opCode,cmdSeq,cmdResp);
  }
```
> `lock.lock()` locks the mutex used to protect the `blinking` member variable as it is set in `BLINKING_ON_OFF_cmdHandler` but will be read elsewhere.

Save the file then run the following command in the terminal to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```

> Fix any errors that occur before proceeding with the rest of the tutorial.

## Telemetry Channels

Telemetry channels represent the state of the system. Typically, telemetry channels are defined for any states that give crucial insight into the component's behavior. This tutorial defines two channels: one will be shown, and the other is left up to the student.

Inside your `led-blinker/Components/Led` directory, open the `Led.fpp` file. After the command you added in the previous section, add a telemetry channel of type `Fw.On` to report the blinking state.

```
        @ Telemetry channel to report blinking state.
        telemetry BlinkingState: Fw.On
```

Save the file. In the terminal, run the following to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```

> Fix any errors that occur before proceeding with the rest of the tutorial.

Inside your `led-blinker/Components/Led` directory, open `Led.cpp`, and navigate to the `BLINKING_ON_OFF` command. Report the blinking state via the telemetry channel we just added. To do so, replace the following:

```cpp
      // TODO: Report the blinking state via a telemetry channel.
      // NOTE: This telemetry channel will be added during the "Telemetry" exercise.
```

with the command to send the telemetry channel:

```cpp
this->tlmWrite_BlinkingState(on_off);
```

Save the file. In the terminal, run the following to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```
> Fix any errors that occur before proceeding with the rest of the tutorial.


Now complete the component's telemetry model by adding the `LedTransitions` channel of type `U64` to `Led.fpp`. You will emit this telemetry channel in a further section.

> Build with that new channel and fix any errors before continuing.


## Conclusion

Congratulations!  You have now implemented some basic functionality in a new F´ component. Before finishing the implementation, let's take a break and try running the above command through the ground system. This will require integrating the component into the system topology.

> When running in the ground system try running `led.BLINKING_ON_OFF` with a value of `ON` and ensure that the channel `BlinkingState` is emitted indicating the blinking switched to on!

### Next Step: [Initial Component Integration](./initial-integration.md).
