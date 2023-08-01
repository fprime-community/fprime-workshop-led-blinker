# Environment Setup

In order to run code on reference hardware, we need the ability to cross-compile for the given target architecture. ARM/Linux cross-compilers require a Linux computer in order to build the code. In this guide we will walk through how to set up Linux, cross-compilers, and F´ across the variety of computers students may be using.

> When attending an in-person workshop, please come having worked through the steps below.

### Step 0: Basic Requirements

Students running this tutorial must have a computer that meets the following basic requirements.

1. Computer running Windows 10, Mac OS X, or Ubuntu
2. Administrator access
3. 5GB of free disk space, 8 GB of RAM
4. Knowledge of the command line for your operating system (Bash, PowerShell, Zsh, etc).

Users may consult the [troubleshooting](https://github.com/nasa/fprime/blob/master/docs/INSTALL.md#troubleshooting) section of the installation guide if problems arise.


## Step 1: Setting Up An Ubuntu Machine and Necessary Packages

In this section we will set up Linux to run on each type of laptop. Unless the laptop is already running Linux, this is done through the use of virtual machines and/or emulation. Please follow the instructions in [Section 2](https://github.com/nasa/fprime/blob/devel/docs/Tutorials/CrossCompilationSetup/CrossCompilationSetupTutorial.md#2-installing-dependencies) of the Cross Compilation tutorial for the operating system you run on your laptop. Our goal will be to get up and running with Ubuntu 20.04 or 22.04 on each type of laptop. *This does not require dual booting*.

> Users of other Linux distributions will need to determine the correct packages for their distribution.

## Step 2: Installing the Toolchain

Installing the cross-compilers will use the pre-built packages provided by ARM. Follow the instructions in [Section 3](https://github.com/nasa/fprime/blob/devel/docs/Tutorials/CrossCompilationSetup/CrossCompilationSetupTutorial.md#3-installing-the-toolchain) of the Cross Compilation Setup tutorial for instructions on how to install these tools for the target hardware.