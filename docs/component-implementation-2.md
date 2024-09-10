# LED Blinker: Continuing Component Implementation

In this section, we will complete the component implementation by transmitting a telemetry channel, and implementing the behavior of the `run` port, which is called by the rate-group.

> Refer back to the [component design](./component-implementation-1.md#component-design) for explanations of what each of these items is intended to do.

## Telemetry Channels

Telemetry channels represent the state of the system. Typically, telemetry channels are defined for any states that give crucial insight into the component's behavior. This tutorial defines two channels: one will be shown, and the other is left up to the student.

Inside your `led-blinker/Components/Led` directory, open the `Led.fpp` file. After the events you added in the previous implementation section, add a telemetry channel of type `Fw.On` to report the blinking state.

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

## Try it yourself

Below is a table with tasks you should complete. These tasks require you to go back into the component's files and add the missing lines.

| Task | Missing lines |
|-------|-------------|
| 1. Add a telemetry channel `LedTransitions` of type `U64` to Led.fpp. You will emit this telemetry channel in a further section. | `telemetry LedTransitions: U64` |

Save all files and in the terminal, run the following to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```

> Resolve any `fprime-util build` errors before continuing

## Parameters

Parameters are ground-controllable settings for the system. Parameters are used to set settings of the system that the ground may need to change at some point during the lifetime of the system. This tutorial sets one parameter, the blink interval.

For each parameter you define in your fpp, the F´ autocoder will autogenerate a SET and SAVE command. The SET command allows ground to update the parameter. The SAVE command tells your parameter database to stage this new parameter value for saving. To save the parameter for use on a FSW reboot, ground will need to send the `PRM_SAVE_FILE` command.

In your `led-blinker/Components/Led` directory, open the `Led.fpp` file. After the telemetry channels you added previously, add a parameter for the blinking interval. Give the parameter the name `BLINK_INTERVAL` and type `U32`.

```
        @ Blinking interval in rate group ticks
        param BLINK_INTERVAL: U32
```

Save the file. In the terminal, run the following to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```

In your `led-blinker/Components/Led` directory, open the file `Led.hpp` and add the following function signature in the `PRIVATE:` scope:

```cpp
    //! Emit parameter updated EVR
    //!
    void parameterUpdated(FwPrmIdType id /*!< The parameter ID*/
    ) override;
```

> This function is called when a parameter is updated via the generated SET command. Although the value is updated automatically, this function gives developers a chance to respond to changing parameters. This tutorial uses it to emit an updated Event.

Save file and in your `led-blinker/Components/Led` directory, open `Led.cpp` and add the implementation for `parameterUpdated`:

```cpp
void Led ::parameterUpdated(FwPrmIdType id) {
    Fw::ParamValid isValid;
    switch(id) {
        case PARAMID_BLINK_INTERVAL: {
            // Read back the parameter value
            U32 interval = this->paramGet_BLINK_INTERVAL(isValid);
            // NOTE: isValid is always VALID in parameterUpdated as it was just properly set
            FW_ASSERT(isValid == Fw::ParamValid::VALID, isValid);

            // Emit the blink interval set event
            // TODO: Add an event with, severity activity high, named BlinkIntervalSet that takes in an argument of type U32 to report the blink interval.
            break;
        }
        default:
            FW_ASSERT(0, id);
    }
}
```

When you are done, save the file. In the terminal, run the following to verify your component is building correctly.

```bash
# In led-blinker/Components/Led
fprime-util build
```
> Resolve any errors before continuing

## Additional Ports

Any communication between components should be accomplished through F´ ports. Thus far we have been using a set of standard ports for handling Commands, Telemetry, Events, and Parameters. This section will add two specific ports to our component: input `run` to be called from the rate group, and output `gpioSet` to drive the GPIO driver.

In your `led-blinker/Components/Led` directory, open the `Led.fpp` file. After the parameters you added previously, add the following two ports:

```
        @ Port receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port sending calls to the GPIO driver
        output port gpioSet: Drv.GpioWrite
```

> Input and output ports can be given any name that you choose. In this example, we choose `run` and `gpioSet` since these names capture the behavioral intent. The types of `Svc.Sched` and `Drv.GpioWrite` are significant as these types must match the remote component.

In your `led-blinker/Components/Led` directory, run the following to autogenerate stub functions for the `run` input port we just added.

```bash
# In led-blinker/Components/Led
fprime-util impl
```

In your `led-blinker/Components/Led` directory, open `Led.template.hpp` file and copy this block over to `Led.hpp` as we did before

```cpp
    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      void run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );
```

In your `led-blinker/Components/Led` directory, open `Led.template.cpp` file and copy this block over to `Led.cpp`.
```cpp
  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void Led ::
    run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }
```

The `run` port will be invoked repeatedly on each cycle of the rate group. Each invocation will call into the `run_handler` function such that the component may perform behavior on each cycle.

Here we want to turn the LED on or OFF based on a cycle count to implement the "blinking" behavior we desire.


In your `led-blinker/Components/Led` directory, open `Led.cpp`, copy in the following block of code, and try filling-in the TODOs based on what you learned and defined in previous sections.

>Don't forget to read the code and comments to understand more about how to use F´.

```cpp
    void Led ::
        run_handler(
            const NATIVE_INT_TYPE portNum,
            NATIVE_UINT_TYPE context)
    {
              // Read back the parameter value
      Fw::ParamValid isValid;
      U32 interval = 0; // TODO: Get BLINK_INTERVAL parameter value
    
      // Force interval to be 0 when invalid or not set
      interval = ((Fw::ParamValid::INVALID == isValid) || (Fw::ParamValid::UNINIT == isValid)) ? 0 : interval;
    
      // Only perform actions when set to blinking
      bool is_blinking = this->m_blinking;
      if (is_blinking)
      {   
          // Check for transitions
          Fw::On new_state = this->m_state;
          switch(this->m_state) {
            case Fw::On::OFF:
              if(0 == this->m_count) {
                new_state = Fw::On::ON;
              }
              break;
            case Fw::On::ON: 
              if((interval / 2) == this->m_count) {
                new_state = Fw::On::OFF;
              }
              break;
            default:
              FW_ASSERT(0, this->m_blinking);
          }   
          
          // A transition has occurred
          if (this->m_state != new_state)
          {   
              this->m_transitions = this->m_transitions + 1;
              // TODO: Add an channel to report the number of LED transitions (this->m_transitions)
              
              // Port may not be connected, so check before sending output
              if (this->isConnected_gpioSet_OutputPort(0))
              {   
                  this->gpioSet_out(0, (Fw::On::ON == new_state) ? Fw::Logic::HIGH : Fw::Logic::LOW);
              }
              
              // TODO: Add an event to report the LED state (new_state).
              this->m_state = new_state;
          }
          
          this->m_count = ((this->m_count + 1) >= interval) ? 0 : (this->m_count + 1);
      }
      else
      {
        if(this->m_state == Fw::On::ON)
        {
          // Port may not be connected, so check before sending output
          if (this->isConnected_gpioSet_OutputPort(0))
          {
            this->gpioSet_out(0, Fw::Logic::LOW);
          }

          this->m_state = Fw::On::OFF;
          // TODO: Add an event to report the LED state (this->m_state).
        }
      }
    }
```
Save the file and in the terminal, run the following to verify your component is building correctly.
```bash
# In led-blinker/Components/Led
fprime-util build
```
> Resolve any errors and finish any TODOs before continuing.

## Try it yourself

Below is a table with tasks you should complete. These tasks require you to go back into the component's code and add the missing function calls.

| Task | Missing function calls |
|-------|-------------|
| Inside the `parameterUpdated` function, add a severity activity high event named `BlinkIntervalSet` that takes in an argument of type `U32` to report the blink interval. | Left as an exercise for the reader. |
| Inside the `run_handler` port handler, get the `BLINK_INTERVAL` parameter value. | Left as an exercise for the reader. |
| Inside the `run_handler` port handler, add a telemetry channel to report the number of LED transitions. | Left as an exercise for the reader. |
| Inside the `run_handler` port handler, add an event to report the new LED state. | Left as an exercise for the reader. |

## Conclusion

Congratulations! You just completed the implementation of your component. It is time to unit test!

### Next Step: [Unit Testing](./unit-testing.md).
