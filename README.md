# LED Blinker: An F´ Tutorial on Physical Hardware

This is designed to be an extended introductory F´ tutorial taking the user through the basics of creating components,
using events, telemetry, commands, and parameters, and integrating topologies with the goal of running F´ on embedded
hardware. Users will be guided through the process of software development and testing on embedded Linux running on an
ARM processor (e.g. RaspberryPI, Odroid, etc).

## Prerequisites

In order to run through this tutorial, you must first do the following:

1. Meet the [F´ System Requirements](https://nasa.github.io/fprime/INSTALL.html#requirements)
2. Install an IDE or text editor supporting copy-paste. [VSCode](https://code.visualstudio.com/) has [plugins](https://marketplace.visualstudio.com/items?itemName=unlv-team5.fpptools) to work with FPP.
3. Attempt the [Hello World Tutorial](https://fprime-community.github.io/fprime-tutorial-hello-world/)

To run on hardware with cross-compiling, you must also:
1. Acquire and set up the appropriate [hardware](docs/hardware.md) for this tutorial
2. Set up a [cross-compiling environment](https://github.com/nasa/fprime/blob/devel/docs/Tutorials/CrossCompilationSetup/CrossCompilationSetupTutorial.md) for their ARM processor
> Attendees to an in-person F´ workshop will have access to 64-bit ARM hardware and should set up the 64-bit cross compiling environment.

## Tutorial Steps

This tutorial is composed of the following steps:

1. [Project Setup](docs/project-setup.md)
2. [Requirements Specification](docs/requirements.md)
3. [Component Design and Initial Implementation](docs/component-implementation-1.md)
4. [Initial Component Integration](docs/initial-integration.md)
5. [Continuing Component Implementation](docs/component-implementation-2.md)
6. [Full System Integration](docs/full-integration.md)
7. [Running on Hardware](docs/running-on-hardware.md)
8. [Unit-Testing](docs/unit-testing.md)
9. [System Testing](docs/system-testing.md)

### First Step: [Project Setup](./docs/project-setup.md)
