# Component Implementation Workshop

The purpose of this exercise is to show you the typical steps in creating and implementing an fprime component. In this exercise you will create and implement an LED-blinking component.

## 1. Create the component

In a terminal, navigate to fprime-workshop-led-blinker/Components and run
```bash
fprime-util new component Led
```

The following steps will occur inside of fprime-workshop-led-blinker/Components/Led.

## 2. FPP

In your preferred text editor, open the Led.fpp file. You will find the following structure:

```
module Components {
    active component Led {

    }
}
```

Commands, telemetry, events, and parameters are added inside of the `active component Led` block. We'll add these in the next sections.

### 2.1. Commands

1. Add a command to enable or disable the blinking LED. The command should take in an argument named `on_off` of type `Fw.On`. This argument will indicate to flight software whether to enable or disable the blinking LED.

### 5.3. Command LED

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


### 2.2. Telemetry

1. Add a telemetry channel of type `U64` to represent the total number of blinks since flight software bootup.

### 2.3. Parameters

1. Add a parameter of type `U32` for how many rate-group cycle ticks should occur before alternating the LED's current on/off state.

### 2.4. Events

1. Add an event for when blinking is turned on or off. The event should report the blinking state as an `Fw.On` type.
1. Add an event for when the LED turns on or off. The event should report the LED state as an `Fw.On` type.
1. Add an event for when the LED blink interval is updated. The event should report the blink interval as a U32 type.

### 2.5 Ports

1. Add a synchronous input port called `run`. Make the port be of type `Svc.Sched`. This port is expected to be called on every rate group cycle and perform the LED-blinking logic.
1. Add an output port called `gpioSet`. Make the port be of type `Drv.GpioWrite`. This port will be used to turn on or off the LED.

## 4. HPP

### 4.1. State

Add the member variables to LED which will help you keep track of state. Add the following:

```c++
    Fw::On state; //! Keeps track if LED is on or off
    U64 transitions; //! The number of on/off transitions that have occurred from FSW boot up
    U32 count; //! Keeps track of how many ticks the LED has been on for
    bool blinking; //! Flag: if true then LED blinking will occur else no blinking will happen
```

### 5. CPP

### 5.1. Initializing Member variables

It's important to initialize all your variables before use. Initialize your member variables:

```c++
Led ::Led(const char* const compName) : LedComponentBase(compName),
    state(Fw::On::OFF),
    transitions(0),
    count(0),
    blinking(false)
{}
```

### 5.2. Updating Parameter

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

### 5.4. Run Handler

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