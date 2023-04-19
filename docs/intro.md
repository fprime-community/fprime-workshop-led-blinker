#  LED Blinker: An F´ Tutorial on Physical Hardware

This is designed to be an extended introductory F´ tutorial taking the user through the basics of creating components,
using events, telemetry, commands, and parameters, and integrating topologies with the goal of running F´ on embedded
hardware. Users will be guided through the process of software development and testing on embedded Linux running on an
ARM processor (e.g. RaspberryPI, Odroid, etc).

This tutorial will cover in-depth:

1. Designing and implementing an LED blink component
2. Integrating the LED component into the topology
3. Cross-compiling and running on embedded Linux
4. Developing unit and system tests for the LED component

## Prerequisites

In order to run through this tutorial, users must do the following:

1. Follow the [Hello World Tutorial](https://github.com/nasa/fprime/blob/devel/docs/Tutorials/HelloWorld/Tutorial.md) to
understand F´ basics
2. Set up a [cross-compiling environment](./environment.md) for ARM processors
3. Acquire and set up the appropriate [hardware](./hardware.md) used by this tutorial

> Attendees to an in-person F´ workshop will have access to the hardware indicated in prerequisite 3. Attendees should
> ensure correct wiring before use.