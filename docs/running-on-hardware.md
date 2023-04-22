# Running on Hardware

In order to run on hardware, the deployment needs to be built for the given hardware (a process called cross-compiling), the executable needs to be uploaded to the hardware, and the executable needs to be run. This section will walk the user through cross-compiling, uploading, and running on hardware.

## TODO> Add notes for macOS
## Update F´ baseline

## Acquiring Cross-Compilers

In the setup for this tutorial the Arm Linux cross-compilers were installed. However, the F´ CMake system still needs the appropriate integration files called "CMake Toolchain Files". There is an F´ package that provides cross-compiler integration for standard ARM Linux cross-compilers (i.e. those provided as binary downloads from ARM).

To use the package, add it as a submodule:
```shell
# In `LedProject`
git submodule add https://github.com/fprime-community/fprime-arm-linux.git
git submodule update --init --recursive
```

Then add an entry to the deployment `settings.ini` file. In `LedProject/LedBlinker/settings.ini` add the following to the `[fprime]` section:

```
[fprime]
...
library_locations: ../fprime-arm-linux
```

## Cross-Compiling for Embedded Arm Linux

Now cross-compiling is as easy as building the deployment for a specific platform. For users running on 64-bit arm the platform is called `aarch64-linux`, and for users on 32-bit linux use `arm-hf-linux`.

> Users need to generate for each platform

Here is how to build for the 64-bit Arm Linux platform:

```shell
# In LedProject/LedBLinker
fprime-util generate aarch64-linux
fprime-util build aarch-linux
```

## Running on Hardware

Now it is time to run on hardware. For this tutorial, the assumption is that the Arm Linux machine is available on the network, is running SSH, and the username, password, device address, and host address is known. Without this configuration, users should stop the tutorial at this point.

First, in a terminal upload the software to device and run the F´ GDS.
```
# In LedProject/LedBlinker
scp -r build-artifacts/aarch64-linux <username>@<device-address>:
fprime-gds -n --dictionary build-artifacts/aarch64-linux/dict/*.xml
```
> User should fill in the username and device address above

In another terminal SSH into the device and run the uploaded software:
```shell
ssh <username>@<device-address>
./aarch64-linux/bin/LedBlinker -a <host-address> -p 50000
```
> User should fill in the username and device address above and ensure the execution of `LedBlinker` is supplied the address of the host computer (that ran the GDS).
