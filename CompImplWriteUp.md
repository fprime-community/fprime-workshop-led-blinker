# Component Implementation Workshop

The purpose of this exercise is to walk you through the creation and implementation of an F Prime component. We will implement the component to control the blinking of an LED.

## Set up

In a terminal, run the following:

```bash
# Check out the F Prime project for this exercise
git clone https://github.jpl.nasa.gov/FPRIME/fprime-workshop-led-blinker.git

# Change directory into the F Prime project
cd fprime-workshop-led-blinker

# Initialize the F Prime submodule
git submodule update --init

# Create a python virtual environment in which we will install the tools needed for F Prime.
python3 -m venv fprime-venv

# Need to run this one line in each new terminal in which you want to run fprime-util
source fprime-venv/bin/activate

# Install dependencies
pip install -U setuptools setuptools_scm wheel pip

# Install F Prime dependencies
pip install -r fprime/requirements.txt

# Generate the build cache to support other commands run by fprime-util
fprime-util generate
```

## Create the component

In a terminal, navigate to the project's root directory. Run the following:

```bash
# This is the directory in which we want to create our components
cd Components

# This creates a new component for us
fprime-util new --component
```
You will be prompted for information regarding your component. Fill out the prompts as shown below:

```bash
[INFO] Cookiecutter source: using builtin
component_name [MyComponent]: Led
component_short_description [Example Component for F Prime FSW framework.]: Component to blink an LED driven by a rate group
component_namespace [Led]: LedBlinker
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
[INFO] Found CMake file at 'fprime-workshop-led-blinker/Components/CMakeLists.txt'
Add component Led to fprime-workshop-led-blinker/Components/CMakeLists.txt at end of file (yes/no)? yes
Generate implementation files (yes/no)? yes
```
Your new component is located in directory `Led`.

## Component State

During the design phase of a component, you may discover the need to manage and keep track of the component's state. In this exercise, we give you the state we want the Led component to update and report.

Open Led.hpp, and add the following private member variables.

```cpp
    Fw::On state; //! Keeps track if LED is on or off
    U64 transitions; //! The number of on/off transitions that have occurred from FSW boot up
    U32 count; //! Keeps track of how many ticks the LED has been on for
    bool blinking; //! Flag: if true then LED blinking will occur else no blinking will happen
```

Open Led.cpp, and initilize your member variables in the constructor:

```cpp
Led ::Led(const char* const compName) : LedComponentBase(compName),
    state(Fw::On::OFF),
    transitions(0),
    count(0),
    blinking(false)
{}
```

### Commands

Commands are used for ground to command the component. We will add a command named `BLINKING_ON_OFF` to turn on or off the blinking LED. This command will take in an argument named `on_off` of type `Fw.On`.


Inside your Led directory, open the file `Led.fpp` and search for the following:

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

Exit the text editor, and run the following:

```bash
fprime-util impl
```

This command will auto generate two files: Led.hpp-template and Led.cpp-template. These files contain the stub function for the command you specified in the fpp.

Open Led.hpp-template and copy this block of code and replace the `TODO_cmdHandler` block inside of Led.hpp

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

Open Led.cpp-template and copy this block of code and replace the `TODO_cmdHandler` block inside of Led.cpp

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

Now we will implement what actions the `BLINKING_ON_OFF` command should perform.

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
      this->blinking = Fw::On::ON == on_off; // Update blinking state

      // TODO: Add an event that reports the state we set to blinking.
      // NOTE: This event will be added during the "Events" exercise.

      // TODO: Report the blinking state via a telemetry channel.
      // NOTE: This telemetry channel will be added during the "Telemetry" exercise.
    }

    // Provide command response
    this->cmdResponse_out(opCode,cmdSeq,cmdResp);
  }
```

Save and close the file. In the terminal, run the following to verify your component is building correctly.

```bash
fprime-util build
```

### Telemetry

Open the Led.fpp file. After the command you added in the previous section, add a telemetry channel of type `Fw.On` to report the blinking state.

```
        @ Telemetry channel to report blinking state.
        telemetry BlinkingState: Fw.On
```

Save and close the file. In the terminal, run the following to verify your component is building correctly.

```bash
fprime-util build
```

Open Led.cpp, and navigate to the `BLINKING_ON_OFF` command. Report the blinking state via the telemetry channel we just added. So, replace the following

```cpp
      // TODO: Report the blinking state via a telemetry channel.
      // NOTE: This telemetry channel will be added during the "Telemetry" exercise.
```

with this

```cpp
this->tlmWrite_BlinkingState(on_off);
```

Save and close the file. In the terminal, run the following to verify your component is building correctly.

```bash
fprime-util build
```

### Events

Open the Led.fpp file. After the telemetry channel you added in the previous section, add two events:

```
        @ Indicates we received an invalid argument.
        event InvalidBlinkArgument(badArgument: Fw.On) \
            severity warning low \
            format "Invalid Blinking Argument: {}"

        @ Reports the state we set to blinking.
        event SetBlinkingState(state: Fw.On) \
            severity activity high \
            format "Set blinking state to {}."
```

Save and close the file. In the terminal, run the following to verify your component is building correctly.

```bash
fprime-util build
```

Open Led.cpp, and navigate to the `BLINKING_ON_OFF` command. Report via an event if there is an error in the input argument. Similarly, use an event to report the set state of the blinking state. So, replace the following

```cpp
        // TODO: Add an event that indicates we received an invalid argument.
        // NOTE: Add this event after going through the "Events" exercise.
```

with

```cpp
        this->log_WARNING_LO_InvalidBlinkArgument(on_off);
```

And replace the following

```cpp
      // TODO: Add an event that reports the state we set to blinking.
      // NOTE: This event will be added during the "Events" exercise.
```

with

```cpp
      this->log_ACTIVITY_HI_SetBlinkingState(on_off);
```

Save and close the file. In the terminal, run the following to verify your component is building correctly.

```bash
fprime-util build
```

### Parameters

Open the Led.fpp file. After the events you added in the previous section, add a parameter for the blinking interval. Give the parameter the name `BLINK_INTERVAL` of type `U32`.

```
        @ Blinking interval in rate group ticks
        param BLINK_INTERVAL: U32
```

Save and close the file. In the terminal, run the following to verify your component is building correctly.

```bash
fprime-util build
```

Open the file Led.hpp and add the following function signature:

```cpp
    //! Emit parameter updated EVR
    //!
    void parameterUpdated(FwPrmIdType id /*!< The parameter ID*/
    );
```

Save and close the file. Open Led.cpp and add the implementation for `parameterUpdated`:

```cpp
void Led ::parameterUpdated(FwPrmIdType id) {
    // Read back the parameter value
    Fw::ParamValid isValid;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);

    // Check the parameter ID is expected and the read was valid before sending the event
    if ((PARAMID_BLINK_INTERVAL == id) && (Fw::ParamValid::VALID == isValid)) {
        // Emit the blink set event
        // TODO: Add an event with, severity activity high, named BlinkIntervalSet that takes in an argument of type U32 to report the blink interval.
    }
}
```

When you are done, save and close the file. In the terminal, run the following to verify your component is building correctly.

```bash
fprime-util build
```

### Ports

1. Add a synchronous input port called `run`. Make the port be of type `Svc.Sched`. This port is expected to be called on every rate group cycle and perform the LED-blinking logic.
1. Add an output port called `gpioSet`. Make the port be of type `Drv.GpioWrite`. This port will be used to turn on or off the LED.

## HPP

### State

Add the member variables to LED which will help you keep track of state. Add the following:

```c++
    Fw::On state; //! Keeps track if LED is on or off
    U64 transitions; //! The number of on/off transitions that have occurred from FSW boot up
    U32 count; //! Keeps track of how many ticks the LED has been on for
    bool blinking; //! Flag: if true then LED blinking will occur else no blinking will happen
```

### CPP

### Initializing Member variables

It's important to initialize all your variables before use. Initialize your member variables:

```c++
Led ::Led(const char* const compName) : LedComponentBase(compName),
    state(Fw::On::OFF),
    transitions(0),
    count(0),
    blinking(false)
{}
```

### Updating Parameter

For each parameter you define in your fpp, the F' autocoder will autogenerate a SET and SAVE command. The SET command allows ground to update the parameter. The SAVE command allows ground to save the current value of the parameter for use even after FSW reboots.

When you SET the parameter, the `parameterUpdated` function is called. This function is defined in the parent class of Led; however, its implementation is empty. Implement this functionality in the Led class. Do the following:

1. Add the `parameterUpdated` signature to Led.hpp
```c++
    //! Emit parameter updated EVR
    //!
    void parameterUpdated(FwPrmIdType id /*!< The parameter ID*/
    );
```
2. Implement `parameterUpdated` in your cpp file. 

```c++
void Led ::parameterUpdated(FwPrmIdType id) {
    // 1. Read the parameter value
    Fw::ParamValid isValid;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
    // 2. Check the parameter ID is expected and the read is valid. If parameter ID and read is valid, then send event indicating the Blink Interval was set
    U32 local_parameter_id = (id - this->getIdBase());
    if ((PARAMID_BLINK_INTERVAL == local_parameter_id ) && (Fw::ParamValid::VALID == isValid)) {
        // 3. Emit the blink set event
        //TODO
    }
}
```

### Run Handler

TODO Write about gpioSet_out first

The run handler will be called on every rate cycle. Implement the logic which handles blinking of the LED.

```c++
void Led ::run_handler(const NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    // 1. Read back the parameter value
    Fw::ParamValid isValid;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);

    // 2. Force interval to be 0 when invalid or not set
    interval = ((Fw::ParamValid::INVALID == isValid) || (Fw::ParamValid::UNINIT == isValid)) ? 0 : interval;

    // 3. if blinking
    if (this->blinking) {
      Fw::On new_state = this->state;
    // Check for transitions
    if ((0 == this->count) && (this->state == Fw::On::OFF)) {
            new_state = Fw::On::ON;
        } else if (((interval/2) == this->count) && (this->state == Fw::On::ON)) {
            new_state = Fw::On::OFF;
        }

        // A transition has occurred
        if (this->state != new_state) {

            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
            }
            this->state = new_state;
        }



        this->count = ((this->count + 1) >= interval) ? 0 : (this->count + 1);

    if (this->blinking) {
        // 3.1 Check for transition from on to off or off to on
        // TODO


        // 3.2 If a transition occurred, then
        //   3.2.1 Call the gpio to turn on or off based on the new state
        //         TODO

        //   3.2.2 Update component's state (e.g. this->state, this->transition)
        //         TODO

        //   3.2.3 Report transition telemetry
        //         TODO

        //   3.2.4 Report LedState
        //         TODO


        // 3.3 Update this->count
        //         TODO
    }
}
```