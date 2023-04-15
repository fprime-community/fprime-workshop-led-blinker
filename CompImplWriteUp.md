# Component Implementation Workshop

The purpose of this exercise is to walk you through the creation and implemention of an F Prime component. We will implement the component to control the blinking of an LED.

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

# Need to run this in each new terminal in which you want to run fprime-util
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
component_namespace [Led]: Led
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
Your new component is located in `Led`.

### Commands

Commands are used for ground to command the component. We will add a command to turn on or off the blinking LED.
Add a command to `Led.fpp` named `BLINKING_ON_OFF`. This command should take in an argument named `on_off` of type `Fw.On`.


```
@ Command to turn on or off the blinking LED
async command BLINKING_ON_OFF(
    on_off: Fw.On @< Indicates whether the blinking should be on or off
)
```


### Command LED

Implement the command handler. This handler will be called when ground issues the BLINKING_ON_OFF command.

```c++
void Led ::BLINKING_ON_OFF_cmdHandler(const FwOpcodeType opCode, const U32 cmdSeq, Fw::On on_off) {

    // 1. Check the command input before processing
    FW_ASSERT(on_off == Fw::On::ON or on_off == Fw::On::OFF, on_off.e);

    // 2. Reset count
    //TODO

    // 3. Set blinking state to commanded state
    //TODO

    // 4. Send out event
    // Call event here

    // 4. Send out command response
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}
```


### Telemetry

1. Add a telemetry channel of type `U64` to represent the total number of blinks since flight software bootup.

### Parameters

1. Add a parameter of type `U32` for how many rate-group cycle ticks should occur before alternating the LED's current on/off state.

### Events

1. Add an event for when blinking is turned on or off. The event should report the blinking state as an `Fw.On` type.
1. Add an event for when the LED turns on or off. The event should report the LED state as an `Fw.On` type.
1. Add an event for when the LED blink interval is updated. The event should report the blink interval as a U32 type.

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