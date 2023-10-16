# LED Blinker: Project Setup

First, make sure to read the tutorial's [readme](../README.md) and follow the [prerequisites section](../README.md#prerequisites). Then set up a new F´ project using the `fprime-util new --project` command. Please select a project name of `led-blinker`, and select `yes` to installing the development tools.

> Remember to activate the virtual environment you created when installing F´.

```
$ fprime-util new --project
  [1/2] Project name (MyProject): led-blinker
  [2/2] Install F´ development tools in current virtual environment?
    1 - yes
    2 - no
    Choose from [1/2] (1): 1
```

Next, generate a build cache using the following commands:

```
cd led-blinker
fprime-util generate
```

### Next Step: [Requirements Specification](./requirements.md).
