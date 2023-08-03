# LED Blinker: Project Setup

First, make sure to read the tutorial's [readme](../README.md) and follow the [prerequisites section](../README.md#prerequisites). Then set up a new F´ project using the `fprime-util new --project` command. Please select a project name of `led-blinker`, an F´ version of `devel`, and to create a project virtual environment.

```
$ fprime-util new --project
Project name [MyProject]: led-blinker
F´ version (select branch or tag) [devel]: devel
Install virtual environment?:
1 - yes
2 - no
Choose from 1, 2 [1]: 1
Virtual environment install path [./venv]: 
```
>  If you bootstrapped the F´ tools in a virtual environment `deactivate` that environment before activating the new one below.

Next, activate the new virtual environment and generate a build cache using the following commands:

```
cd MyProject
. ./venv/bin/activate
fprime-util generate
```

> Remember to source the virtual environment in any new terminal you open.

### Next Step: [Component Design and Initial Implementation](./component-implementation-1.md).
