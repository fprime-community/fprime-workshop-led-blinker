# LED Blinker: Component Design and Implementation, Continued

In this section, we will complete the component design and implementation by adding telemetry, parameters, and ports; and implementing the behavior of the `run` port, which is called by the rate-group.

> Refer back to the [component design](./component-implementation-1.md#component-design) for explanations of what each of these items is intended to do.

## Continued Component Design
### Telemetry

Telemetry channels represent the state of the system. Typically, telemetry channels are defined for any states that give crucial insight into the component's behavior.

Inside your `led-blinker/Components/Led` directory, open the `Led.fpp` file. After the events you added in the previous section, add a telemetry channel of type `Fw.On` to report the blinking state.

```
        @ Telemetry channel to report blinking state.
        telemetry BlinkingState: Fw.On
```

### Do it Yourself

Below is a table with a task you must complete before moving on to the next section. This task require you to update the component's fpp file.

| Task | Solution |
|-------|-------------|
| 1. Add a telemetry channel `LedTransitions` of type `U64` to Led.fpp. | <details><summary>Answer</summary>`telemetry LedTransitions: U64`</details> |

### Parameters

Parameters are ground-controllable settings for the system. Parameters are used to set settings of the system that the ground may need to change at some point during the lifetime of the system. This tutorial sets one parameter, the blink interval.

For each parameter you define in your fpp, the F´ autocoder will autogenerate a SET and SAVE command. The SET command allows ground to update the parameter. The SAVE command tells your parameter database to stage this new parameter value for saving. To save the parameter for use on a FSW reboot, ground will need to send the `PRM_SAVE_FILE` command.

In your `led-blinker/Components/Led` directory, open the `Led.fpp` file. After the telemetry channels you added previously, add a parameter for the blinking interval. Give the parameter the name `BLINK_INTERVAL`, type `U32`, and a default value. It is good practice to assign parameters a valid default value.

```
        @ Blinking interval in rate group ticks
        param BLINK_INTERVAL: U32 default 1
```

### Additional Ports

Any communication between components should be accomplished through F´ ports. Thus far we have been using a set of standard ports for handling Commands, Telemetry, Events, and Parameters. This section will add two specific ports to our component: input `run` to be called from the rate group, and output `gpioSet` to drive the GPIO driver.

In your `led-blinker/Components/Led` directory, open the `Led.fpp` file. After the parameters you added previously, add the following two ports:

```
        @ Port receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port sending calls to the GPIO driver
        output port gpioSet: Drv.GpioWrite
```

> Input and output ports can be given any name that you choose. In this example, we choose `run` and `gpioSet` since these names capture the behavioral intent. The types of `Svc.Sched` and `Drv.GpioWrite` are significant as these types must match the remote component.

## Continued Component Implementation

### Input Port Implementation

In your `led-blinker/Components/Led` directory, run the following to autogenerate stub functions for the `run` input port we just added.

```bash
# In led-blinker/Components/Led
fprime-util impl
```

In your `led-blinker/Components/Led` directory, open `Led.template.hpp` file and copy this block over to `Led.hpp`.

```cpp
  PRIVATE:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    //! Port receiving calls from the rate group
    void run_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                     NATIVE_UINT_TYPE context  //!< The call order
                     ) override;
```

In your `led-blinker/Components/Led` directory, open `Led.template.cpp` file and copy this block over to `Led.cpp`.
```cpp
// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Led ::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    // TODO
}
```

> Copying from the template file and pasting into your implementation file is a pattern in F Prime that is often used when adding new input ports or commands.

The `run` port will be invoked repeatedly on each cycle of the rate group. Each invocation will call into the `run_handler` function such that the component may perform behavior on each cycle.

Here we want to turn the LED on or OFF based on a cycle count to implement the "blinking" behavior we desire.

Copy the run_handler implementation below into your run_handler. Try filling in the TODOs based on what you learned and defined in previous sections.

>Don't forget to read the code and comments to understand more about how to use F´.

```cpp
void Led ::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context) {
    // Read back the parameter value
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
    FW_ASSERT((isValid != Fw::ParamValid::INVALID) && (isValid != Fw::ParamValid::UNINIT),
              static_cast<FwAssertArgType>(isValid));

    // Only perform actions when set to blinking
    if (this->m_blinking && (interval != 0)) {
        // If toggling state
        if (this->m_toggleCounter == 0) {
            // Toggle state
            this->m_state = (this->m_state == Fw::On::ON) ? Fw::On::OFF : Fw::On::ON;
            this->m_transitions++;
            // TODO: Report the number of LED transitions (this->m_transitions) on channel LedTransitions

            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, (Fw::On::ON == this->m_state) ? Fw::Logic::HIGH : Fw::Logic::LOW);
            }

            // TODO: Emit an event LedState to report the LED state (this->m_state).
        }

        this->m_toggleCounter = (this->m_toggleCounter + 1) % interval;
    }
    // We are not blinking
    else {
        if (this->m_state == Fw::On::ON) {
            // Port may not be connected, so check before sending output
            if (this->isConnected_gpioSet_OutputPort(0)) {
                this->gpioSet_out(0, Fw::Logic::LOW);
            }

            this->m_state = Fw::On::OFF;
            // TODO: Emit an event LedState to report the LED state (this->m_state).
        }
    }
}
```
In the terminal, run the following to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```

> Fix any errors that occur before proceeding with the rest of the tutorial.

### Command Implementation Continued

Inside your `led-blinker/Components/Led` directory, open `Led.cpp`, and navigate to the `BLINKING_ON_OFF` command. Report the blinking state via the telemetry channel we just added. To do so, replace the following:

```cpp
      // TODO: Report the blinking state (onOff) on channel BlinkingState.
      // NOTE: This telemetry channel will be added during the "Telemetry" exercise.
```

with the function to send the telemetry channel:

```cpp
    this->tlmWrite_BlinkingState(onOff);
```

In the terminal, run the following to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```

> Fix any errors that occur before proceeding with the rest of the tutorial.

### Parameter Implementation

When ground updates a component's parameter, the user may want the component to react to the parameter update. F Prime provides a function called `parameterUpdated` where your component can react to each parameter update. Implementing `parameterUpdated` for a component is optional but we'll implement it for this tutorial.

In your `led-blinker/Components/Led` directory, open the file `Led.hpp` and add the following function signature in the `PRIVATE:` scope:

```cpp
    //! Emit parameter updated EVR
    //!
    void parameterUpdated(FwPrmIdType id  //!< The parameter ID
                          ) override;
```

> This function is called when a parameter is updated via the auto generated SET command. Although the value is updated automatically, this function gives developers a chance to respond to changing parameters. This tutorial uses it to emit an event.

Save file and in your `led-blinker/Components/Led` directory, open `Led.cpp` and add the implementation for `parameterUpdated`:

```cpp
void Led ::parameterUpdated(FwPrmIdType id) {
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    switch (id) {
        case PARAMID_BLINK_INTERVAL: {
            // Read back the parameter value
            const U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
            // NOTE: isValid is always VALID in parameterUpdated as it was just properly set
            FW_ASSERT(isValid == Fw::ParamValid::VALID, static_cast<FwAssertArgType>(isValid));

            // Emit the blink interval set event
            // TODO: Emit an event with, severity activity high, named BlinkIntervalSet that takes in an argument of
            // type U32 to report the blink interval.
            break;
        }
        default:
            FW_ASSERT(0, static_cast<FwAssertArgType>(id));
            break;
    }
}
```

In the terminal, run the following to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```
> Resolve any errors before continuing

### Do it Yourself

Below is a table with tasks you must complete. These tasks require you to go back into the component's code and add the missing function calls.

| Task | Solution |
|------|----------|
| Inside the `parameterUpdated` function, emit an activity high event named `BlinkIntervalSet` that takes in an argument of type `U32` to report the blink interval. | <details><summary>Answer</summary>`this->log_ACTIVITY_HI_BlinkIntervalSet(interval);`</details> |
| Inside the `run_handler` port handler, report the number of LED transitions (this->m_transitions) on channel LedTransitions. | <details><summary>Answer</summary>`this->tlmWrite_LedTransitions(this->m_transitions);`</details> |
| Inside the `run_handler` port handler, emit an event LedState to report the LED state (this->m_state). There are two places to add this event. | <details><summary>Answer</summary>`this->log_ACTIVITY_LO_LedState(this->m_state);`</details> |

> NOTE: Emitting an event follows this pattern: `this->log_<severity>_<eventName>(<argument_if_any>);`  
> NOTE: Emitting a telemetry channel follows this pattern: `this->tlmWrite_<telemetryChannel>(<telemetryValue>);`

## Conclusion

Congratulations! You just completed the implementation of your component. It is time to unit test!

### Next Step: [Unit Testing](./unit-testing.md).
