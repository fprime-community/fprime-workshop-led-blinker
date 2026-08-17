---
permalink: /timeliness
---

# Extension Lesson: Timeliness and Deadline-Driven Components

This extension lesson builds on the completed LED Blinker tutorial. You will refactor the `Led` component from an **active** component to a **queued** component driven synchronously by a rate group, introducing the concept of **timeliness** — the ability to detect when work exceeds its allotted time.

## Prerequisites

You must have completed the [LED Blinker Tutorial](led-blinker.md) through at least [Step 7: Full System Integration](led-blinker.md#7-led-blinker-full-system-integration).

## Why Timeliness Matters

In the original tutorial, the `Led` component is an active component. Active components own a thread and process work from an internal queue whenever that thread is scheduled by the OS. This approach works well for event-driven tasks (like responding to commands) and for non-deadline timing signals where approximate periodicity is sufficient. However, since there is jitter in the dispatch of the message and there is no way to detect slips, this approach is insufficient for deadline-driven work.

Consider what happens when the rate group invokes the `Led` component's `run` port:

1. The rate group puts a message on `Led`'s queue via the `async` port
2. The rate group **immediately moves on** to its next member — it does not wait for `Led` to finish
3. `Led`'s thread eventually picks up the message and processes it

If `Led`'s work takes longer than one rate group cycle, the rate group has no way of knowing. Messages simply pile up in the queue. In flight software, this is dangerous — a control loop that silently falls behind its deadline can lead to mission-critical failures.

### The Solution: Queued Components with Synchronous Rate Group Dispatch

A **queued** component solves this by using a **synchronous** (`sync`) port for the rate group invocation. This means:

1. The rate group calls `Led`'s `run` handler **directly** — it blocks until the handler returns
2. If the handler takes too long, the rate group detects a **slip** and emits a `WARNING_HI` event
3. Commands and other event-driven work are still accepted via `async` ports, queued up, and dispatched **inside** the synchronous handler

This pattern gives you the best of both worlds: hard deadline enforcement from the rate group, and asynchronous command handling without thread contention.

```
                    Rate Group (sync call)
                         |
                         v
             +-----------+-----------+
             |     Led (queued)      |
             |                       |
             |  run_handler (sync):  |
    cmd ---->|  1. dispatchAvail..() |----> gpioSet
   (async)   |    2. blink logic     |
             +-----------------------+
```

> [!TIP]
> For a deeper discussion of component and port kind selection, see the F Prime documentation on [Selecting Component, Port, and Command Kinds](https://fprime.jpl.nasa.gov/latest/docs/user-manual/framework/component-and-port-selection/).

---

## Step 1: Update the FPP Model

Open `LedBlinker/Components/Led/Led.fpp`. We need to make two changes:

### 1a. Change the Component Kind from `active` to `queued`

```diff
-    active component Led {
+    queued component Led {
```

### 1b. Change the `run` Port from `async` to `sync`

The `run` port must be synchronous so that it executes in the rate group's thread context. This is what enables deadline detection.

```diff
         @ Port receiving calls from the rate group
-        async input port run: Svc.Sched
+        sync input port run: Svc.Sched
```

> [!IMPORTANT]
> The `BLINKING_ON_OFF` command remains `async`. This is intentional — commands are event-driven work and should be queued for processing during the next rate group cycle rather than executing immediately on the command dispatcher's thread. This prevents thread contention and keeps all component state modifications on a single execution context.

---

## Step 2: Update the Component Implementation

### 2a. Add Queue Dispatch to the `run` Handler

Since the `BLINKING_ON_OFF` command is `async`, it will be placed in the component's queue when received. However, a queued component has no thread of its own to process the queue — we must explicitly dispatch queued messages during the synchronous `run` handler. This has the advantage that the time to dispatch the queue is also summed into the rate group, so all work done for the component is accounted for in deadline detection.

Open `LedBlinker/Components/Led/Led.cpp` and add the queue dispatch at the **beginning** of the `run_handler`:

```cpp
void Led ::run_handler(FwIndexType portNum, U32 context) {
    // Dispatch all queued messages (e.g. commands) that arrived since the last cycle
    this->dispatchAvailableMessages();

    // Read back the parameter value
    Fw::ParamValid isValid = Fw::ParamValid::INVALID;
    // ... rest of existing implementation unchanged ...
```

`dispatchAvailableMessages()` is a helper provided by `QueuedComponentBase` that dispatches all currently queued messages in a single call. It iterates over each message in the queue, calling `doDispatch()` for each one, and stops when the queue is empty or an error occurs.

### 2b. Understanding the Dispatch Pattern

In the active component version, the component's internal thread continuously waited for messages and dispatched them one at a time. Now that we have removed the thread, nothing will process the queue unless we do it explicitly.

By placing `dispatchAvailableMessages()` at the beginning of `run_handler`, we ensure that:
1. Commands received between cycles are processed at a well-defined point
2. All work (command handling and blink logic) runs in the rate group's thread context
3. If the total work (dispatch + blink logic) exceeds the cycle time, the rate group detects the slip

---

## Step 3: Update the Topology Instance

Since the component is no longer active, it does not need its own thread. However, it still needs a queue to buffer the async commands.

Open `LedBlinker/LedBlinkerDeployment/Top/instances.fpp` and find the `led` instance definition.

```diff
  instance led: LedBlinker.Led base id 0x10005000 \
-    queue size Default.QUEUE_SIZE \
-    stack size Default.STACK_SIZE \
-    priority 95
+    queue size Default.QUEUE_SIZE
```

Move the `led` instance from the "Active component instances" section to the "Queued component instances" section:

```
  # ----------------------------------------------------------------------
  # Queued component instances
  # ----------------------------------------------------------------------

  instance led: LedBlinker.Led base id 0x10005000 \
    queue size Default.QUEUE_SIZE
```

> [!NOTE]
> We keep `queue size` because the component still has an async command port that requires a queue. We remove `stack size` and `priority` because those configure a thread, which a queued component does not have.

Build the deployment to verify the changes:
```shell
# In LedBlinker/LedBlinkerDeployment
fprime-util build
```

---

## Step 4: Update Unit Tests

Since the `run` port is now `sync` instead of `async`, invoking it in unit tests will execute the handler **immediately** — there is no queue message to dispatch. We need to remove the `doDispatch()` calls that follow `invoke_to_run()`.

Open `LedBlinker/Components/Led/test/ut/LedTester.cpp` and update the test cases.

```diff
     this->invoke_to_run(0, 0);     // invoke the 'run' port to simulate running one cycle
-    this->component.doDispatch();   // Trigger execution of async port
```

Remove all `this->component.doDispatch()` lines that follow `invoke_to_run` calls. The `run` port is now `sync`, so calling it executes the handler immediately — there is no queued message to dispatch.

> [!IMPORTANT]
> Commands are still `async`, so `this->component.doDispatch()` is still required after `this->sendCmd_BLINKING_ON_OFF(...)`. Only the run port dispatch changes.

Verify the tests pass:
```shell
# In LedBlinker/Components/Led
fprime-util check
```

---

## Step 5: Observe Deadline Detection

Now that the `Led` component runs synchronously in the rate group, the rate group will detect if the component's work takes longer than one cycle.

To test this, you could temporarily add a simulated delay to the `run_handler`:

```cpp
// TEMPORARY: Simulate slow work to demonstrate rate group slip detection
Os::Task::delay(Fw::TimeInterval(2, 0));  // Sleep 2 seconds in a 1Hz rate group
```

Run the deployment with the GDS:
```shell
# In LedBlinker/LedBlinkerDeployment
fprime-util build
fprime-gds --ip-client
```

You should see a `WARNING_HI` event from the rate group indicating a **cycle slip** — the rate group detected that `Led` did not complete within its allotted time.

> [!WARNING]
> Remove the simulated delay after testing! This is for demonstration purposes only.

---

## What Changed: A Summary

| Aspect | Before (Active) | After (Queued) |
|---|---|---|
| Component kind | `active` | `queued` |
| `run` port kind | `async` | `sync` |
| `BLINKING_ON_OFF` command | `async` | `async` (unchanged) |
| Thread | Owns a dedicated thread | No thread — runs on rate group's thread |
| Queue | Dispatched by internal thread | Dispatched explicitly in `run_handler` |
| Deadline detection | None — rate group cannot detect slips | Rate group emits `WARNING_HI` on cycle slip |
| Instance definition | `queue size`, `stack size`, `priority` | `queue size` only |

---

## Key Takeaways

1. **Active components hide deadline violations.** Because the rate group sends an async message and moves on, it cannot detect if the component falls behind.

2. **Queued components with sync rate group ports enforce deadlines.** The rate group blocks on the component's work and detects slips when the cycle time is exceeded.

3. **Async commands should be dispatched inside the sync handler.** This keeps all state modifications on a single execution context (the rate group's thread) and avoids concurrency issues.

4. **This is how flight software typically works.** Components performing cyclic work are driven by rate groups and run synchronously. The `Svc::Health` component in F Prime itself uses this exact pattern.

> [!TIP]
> For more details on rate groups and timeliness, see:
> - [Rate Groups and Timeliness](https://fprime.jpl.nasa.gov/latest/docs/user-manual/design-patterns/rate-group/)
> - [Component and Port Selection](https://fprime.jpl.nasa.gov/latest/docs/user-manual/framework/component-and-port-selection/)
> - [F Prime on Baremetal Systems](https://fprime.jpl.nasa.gov/latest/docs/user-manual/framework/run-baremetal/)
