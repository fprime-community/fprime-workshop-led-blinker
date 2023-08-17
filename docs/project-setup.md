# LED Blinker: Project Setup

First, make sure to read the tutorial's [readme](../README.md) and follow the [prerequisites section](../README.md#prerequisites). Then set up a new F´ project using the `fprime-util new --project` command. Please select a project name of `led-blinker`, an F´ version of `devel`, and to create a project virtual environment.

> Remember to activate the virtual environment you created when installing F´.

```
$ fprime-util new --project
  [1/2] Project name (MyProject): led-blinker
  [2/2] Install F´ development tools in current virtual environment?
    1 - yes
    2 - no
    Choose from [1/2] (1): 1
```
>  If you bootstrapped the F´ tools in a virtual environment `deactivate` that environment before activating the new one below.

Next, activate the new virtual environment and generate a build cache using the following commands:

```
cd led-blinker
fprime-util generate
```

### Next Step: [Component Design and Initial Implementation](./component-implementation-1.md).
