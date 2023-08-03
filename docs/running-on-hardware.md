# LED Blinker: Running on Hardware

In order to run on hardware, the deployment needs to be built for the given hardware target (a process called cross-compiling). Then the executable needs to be uploaded to the hardware, and the executable needs to be run connecting back to the host running the F´ GDS. This section will walk the user through cross-compiling, uploading, and running on hardware.

## Cross-Compiling for Embedded Arm Linux

In the prerequisites for this tutorial, the ARM Linux cross-compilers were installed. In this section, we will use these compilers to cross-compile for ARM Linux by utilizing integration files called "CMake Toolchain". Follow the steps in the [ARM cross compilation tutorial](https://github.com/nasa/fprime/blob/devel/docs/Tutorials/CrossCompilationSetup/CrossCompilationTutorial.md) on how to compile for ARM.

> During the step for running the `fprime-util generate` and `fprime-util build` commands, macOS users must run from within the Docker container in the directory `/project/LedBlinker`

## Running on Hardware

Now it is time to run on hardware. For this tutorial, the assumption is that the Arm Linux machine is available on the network, is running SSH, and the username, password, device address, and host address are known to the student. Without this configuration, users should skip to the next section of the tutorial. 

Follow the [F´ Running on ARM Linux Tutorial](https://github.com/nasa/fprime/blob/devel/docs/Tutorials/CrossCompilationSetup/ArmLinuxTutorial.md) for step-by-step instructions on how to upload the software to the hardware platform, launching F´ GDS, and for running the uploaded software.

# Conclusion

Congratulations you've now run on hardware. It is time to unit test!


### Next Step: [Unit Testing](./unit-testing.md).
