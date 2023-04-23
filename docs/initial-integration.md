# LED Blinker: Initial Component Integration

In this section, users will create a deployment and perform the initial integration the LED component into that deployment. This deployment will automatically include the basic command and data handling setup needed to interact with the component. Wiring the `Led` component to the GPIO driver component will be covered in a later section after the component implementation has finished.

> Users must have created the [initial Led component implementation](./component-implementation-1.md) in order to run through this section. Users may continue to define commands, events, telemetry, and ports after this initial integration.

## Creating the `LedBlinker` Deployment

In order to produce an executable to run the software, users need to create a deployment. A deployment is one software executable that contains the main entrypoint, and an F´ system topology.

Create a new deployment in the `led-blinker` directory with:

```shell
#In led-blinker
fprime-util new --deployment
```

This will ask for some input, respond with the following answers:
```shell
[INFO] Cookiecutter: using builtin template for new deployment
deployment_name [MyDeployment]: LedBlinker
path_to_fprime [./fprime]:
```
> Use the default response for any other questions asked.

In order to check that the deployment was created successfully, the user can generate a build cache and build the deployment. This will generate and build the code for the current host system, not the remote embedded hardware allowing a local test during development. 

```shell
# In led-blinker
cd LedBlinker
fprime-util generate
fprime-util build
```

## Adding `Led` Component To The Deployment

The component can now be added to the deployment's topology effectively adding this component to the running system. This is done by modifying `instances.fpp` and `topology.fpp` in the `Top` directory.

Add the following to `led-blinker/LedBlinker/Top/instances.fpp`.  Typically, this is added to the "Active component instances" section of that document.

```
  instance led: Components.Led base id 0x0E00 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 95
```

This defines an instance of the `Led` component called `led`. Since the component is active it needs a queue size, stack size, and priority for the thread of the component and the queue that thread serves. We have chosen the topology specified defaults and a priority of 95.

Next, the topology needs to use the above definition. This is done by adding the following to the list of instances defined in `led-blinker/LedBlinker/Top/topology.fpp`.

```
  topology LedBlinker {
    ...
    instance ...
    instance led
```

> No port connections need to be added because thus far the component only defines standard ports and those are connected automatically.

## Adding `Led` Channels To the Packet Specification

Some users choose to send telemetry packets instead of raw channels to the ground system. Although this tutorial will not use telemetry packets, it is best practice to keep the packet definitions up-to-date to make switching to telemetry packets seamless should the user choose to do so.

Add the following to `led-blinker/LedBlinker/Top/LedBlinkerPackets.xml`:

```xml
    <packet name="LedChannels" id="8" level="1">
        <channel name="led.LedTransitions"/>
        <channel name="led.BlinkingState"/>
    </packet>
```
> Add this after the opening `<packet>` tag and before the closing `</packet>` tag as a separate block.

Now that this has been added, build the topology:

```shell
```shell
# In led-blinker/LedBlinker
fprime-util build
```
> Fix any errors before continuing

## Testing the Topology

The topology may now be run. This can be done with the `fprime-gds` command. Since we are currently building for the host platform, that command will run the ground data system (GDS) and the deployment executable automatically in-tandem.

> Make sure to build the deployment first with `fprime-util build`

```shell
fprime-gds
```
This will likelt open up your browser and show the running flight software.  If it does not open a browser, navigate to `http://localhost:5000`.

Test the component integration with the following steps:
1. **Verify connection**: confirm that there is a green circle and not a red X in the upper right corner.
2. **Send a Command**: select the 'Commanding' tab, search for `led.BLINKING_ON_OFF` and send it with the argument set to `ON`.
3. **Verify Channels**: select the 'Channels' tab and verify that the `led.BlinkingState` channel reads `ON`.

`CTRL-C` to stop the F´ GDS program.

## Conclusion

Congratulations! You have now integrated your component and tested that integration.

Return to the `led-blinker/LedBlinker` and run the following commands to test whenever you desire.

```
#In led-blinker/LedBlinker
fprime-util build
fprime-gds

# CTRL-C to exit
```

This tutorial will return to the component implementation before finishing the integration of the component and testing on hardware.

### Next Step: [Continuing Component Implementation](./component-implementation-2.md).