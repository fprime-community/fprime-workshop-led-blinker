# Unit Testing Guide

This exercise will walk through development of unit tests of the Led component created in the component implementation workshop.

TODO: coverage
TODO: closing section
TODO test UTs on tlm and EVR

## Unit Test Generation

To start off, use `fprime-util` to generate a unit test outline for the Led component.

First, register unit tests for the Led component by adding these lines to the component `CMakeLists.txt` file.

```
set(UT_SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/Led.fpp"
)

register_fprime_ut()
```

Next, generate a unit test build cache by running the following terminal commands:

```shell
#In: fprime-workshop-led-blinker/Components/Led
$ fprime-util generate --ut
```

To generate unit test template files, run the implementation command on the terminal:


```shell
#In: fprime-workshop-led-blinker/Components/Led
$ fprime-util impl --ut
```

This step should create the files `Tester.cpp`, `Tester.hpp`, `TesterHelpers.cpp`, and `TestMain.cpp` in your current directory.

Now, create a `test/ut` directory and copy those files into them.

```shell
#In: fprime-workshop-led-blinker/Components/Led
$ mkdir -p test/ut
$ cp Test* test/ut/
```

Finally, test the skeleton unit tests with:

```shell
#In: fprime-workshop-led-blinker/Components/Led
$ fprime-util check
```

## Add a New Test Case

Now that unit tests have been written, we can our first unit test case.

First, remove the default `ToDo` test and add a new test case called `testBlinking`.

In `Tester.cpp`:

```c++
  void Tester ::
    testBlinking()
  {

  }
```

In `Tester.hpp`:

```c++
public:
    ... other methods ...
    void testBlinking();
```

In `TestMain.cpp`:

```c++
TEST(Nominal, TestBlinking) {
    Components::Tester tester;
    tester.testBlinking();
}
```

Use `fprime-util check` to make sure the new check builds and passes.

## Write a Test Case

The first test we will write is to test that the LED doesn't blink when blinking is disabled.

Add the following code to the `testBlinking` method:

```c++
// Ensure LED stays off when blinking is disabled
// The Led component defaults to blinking off
this->invoke_to_run(0,0); // invoke the 'run' port to simulate running one cycle
ASSERT_EVENTS_LedState_SIZE(0); // ensure no LedState change events we emitted
ASSERT_from_gpioSet_SIZE(0); // ensure gpio LED wasn't set
ASSERT_TLM_LedTransitions_SIZE(0); // ensure no LedTransitions were recorded
```

The `this->invoke_to_PORTNAME()` methods are used to call input ports on the component under test.

The fprime unit testing framework generates a series of history buffers to store a fixed amount of events, telemetry, and output ports emitted from the component.

The `ASSERT_<>_SIZE(size)` (ex: `ASSERT_EVENTS_LedState_SIZE(0)`) macros are used to assert the size of the history buffer matches your expectations
The `ASSERT_<>(index, <arg 1>, <arg 1>, <arg N>)` macros are used to check that items in the history buffer match expectations.

Use `fprime-util check` to make sure the test case builds and passes.

Next, enable blinking, then step through 3 cycles to verify the LED component blinks the LED on, off, and the on again.

```c++
// Send command to enable blinking
this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
this->component.doDispatch(); // Trigger execution of async command
```

The fprime unit test framework provides `this->sendCmd_COMMAND_NAME(args)` that allows calling a command on the component under test.
Because `BLINKING_ON_OFF` is an `async` command, it's not dispatched immediately, but instead added to an execution queue.
To dispatch a queued command, unit tests must explicitly call the `doDispatch()` command to execute the first command on the queue.

Now, check that the state of the component matches expectations after each of three cycles.
Write assertions to fill in the todo comments.

```c++
// Step through 3 run cycles to observe LED turning on and off 3 times
// Cycle 1: LED initalization->On
this->invoke_to_run(0,0);
ASSERT_EVENTS_LedState_SIZE(1);
ASSERT_EVENTS_LedState(0, Fw::On::ON);
ASSERT_from_gpioSet_SIZE(1);
ASSERT_from_gpioSet(0, Fw::Logic::HIGH);
ASSERT_TLM_LedTransitions_SIZE(1);
ASSERT_TLM_LedTransitions(0, 1);

// Cycle 2: LED On->Off
this->invoke_to_run(0,0);
ASSERT_EVENTS_LedState_SIZE(2);
ASSERT_EVENTS_LedState(1, Fw::On::OFF);
ASSERT_from_gpioSet_SIZE(2);
ASSERT_from_gpioSet(1, Fw::Logic::LOW);
//TODO: Add assertions for LedTransitions telemetry

// Cycle 3: LED Off->On
this->invoke_to_run(0,0);
//TODO: Write assertions for third cycle
```

Run `fprime-util check` and make sure the new assertions pass.

## Adding a Parameter Test Case

This second test will test that adjusting `BLINK_INTERVAL` will impact the blink interval.

Add a new test case called `testBlinkInterval` and use the following code as a starting point:

```c++
  void Tester ::
    testBlinkInterval()
  {
    // Enable LED Blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch(); // Trigger execution of async command

    // Adjust blink interval to 4 cycles
    U32 blinkInterval = 4;
    this->paramSet_BLINK_INTERVAL(blinkInterval, Fw::ParamValid::VALID);
    this->paramSend_BLINK_INTERVAL(0, 0);
    ASSERT_EVENTS_BlinkIntervalSet_SIZE(1);

    // TODO: Add logic to test adjusted blink interval
  }
```
