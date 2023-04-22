# Initial Component Integration

In this section, users will create a deployment and perform the initial integration the LED component into that deployment. This deployment will automatically include the basic command and data handling setup needed to interact with the component. Wiring the Led component to the GPIO driver component will be covered in a later section.

> Users must have created the basic Led component in order to run through this section. Users may continue to define commands, events, telemetry, and ports after this initial integration.

TODO: `<channel name="led.LedTransitions"/>`

        <channel name="led.LedTransitions"/>
        <channel name="led.BlinkingState"/>

## Creating the `LedBlinker` Deployment

In order to produce an executable to run the software, users need to create a deployment. A deployment is one software executable that contains the main entrypoint, and F´ system topology.  A new deployment may be created with:

```shell
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
cd LedBlinker
fprime-util generate
fprime-util build
```

## Adding `Led` Component To The Deployment

The component can now be added to the deployment's topology effectively adding this component to the running system. This is done by modifying `instances.fpp` and `topology.fpp` in the `Top` directory.

Add the following to `LedBlinker/Top/instances.fpp`.  Typically this is added to the "Active component instances" section of that document.

```
  instance led: Components.Led base id 0x0E00 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 95
```

This defines an instance of the Led component called `led`. Since the component is active it needs a queue size, stack size, and priority for the thread of the component and the queue that thread serves. We have chosen the topology specified defaults and a priority of 95.

Next, the topology needs to use the above definition. This is done by adding the following to the list of instances defined in `LedBlinker/Top/topology.fpp`.

```
  topology LedBlinker {
    ...
    instance ...
    instance led
```

The topology autocoder will automatically hook up the command, event, and telemetry channel ports for the component so no additional wiring needs to be done.

Now run the deployment and look around. Should commands be defined, try running one!

```shell
fprime-gds
```