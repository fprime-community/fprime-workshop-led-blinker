# LED Blinker: Running on Hardware

In order to run on hardware, the deployment needs to be built for the given hardware target (a process called cross-compiling). Then the executable needs to be uploaded to the hardware, and the executable needs to be run connecting back to the host running the F´ GDS. This section will walk the user through cross-compiling, uploading, and running on hardware.

## Acquiring Cross-Compilers

In the [environment setup](./prerequisites.md) for this tutorial the Arm Linux cross-compilers were installed. However, the F´ CMake system still needs the appropriate integration files to build. These files are called "CMake Toolchain". There is an F´ package that provides cross-compiler integration for standard ARM Linux cross-compilers (i.e. those provided as binary downloads from ARM).

To install the F´ ARM package, add it as a submodule via `git`:
```shell
# In `led-blinker`
git submodule add https://github.com/fprime-community/fprime-arm-linux.git
git submodule update --init --recursive
```

Next add the package to the `LedBlinker` deployment's `settings.ini` file. In `led-blinker/LedBlinker/settings.ini` add the following to the `[fprime]` section of the file:

```
[fprime]
...
library_locations: ../fprime-arm-linux
```

> Now is a good time to ensure that the ARM toolchains were installed properly. To test run the following command:
> ```shell
> # For in-person workhops and 64-bit ARM hardware
> /opt/toolchains/bin//aarch64-none-linux-gnu-gcc -v
> 
> # For 32-bit ARM hardware
> /opt/toolchains/bin//aarch64-none-linux-gnu-gcc -v
> ```
>
> macOS users must run these commands from within the Docker container described in [Appendix I](./appendix-1.md). Ensure this [script](https://github.com/fprime-community/fprime-workshop-led-blinker/main/bin/macos-docker) was downloaded to `led-blinker/bin/macos-docker` and run it in the terminal.

## Cross-Compiling for Embedded Arm Linux

Now cross-compiling is as easy as building the deployment for a specific platform. For users running on 64-bit arm the platform is called `aarch64-linux`, and for users on 32-bit linux use `arm-hf-linux`. This package expects the environment variable `ARM_TOOLS_PATH` to point to the installation directory of the ARM cross-compilers.

> Users need to generate for each platform they wish to run on.  We previously generated for our host machine.

Here is how to build for the 64-bit Arm Linux platform:

```shell
# In led-blinker/LedBLinker
export ARM_TOOLS_PATH=/opt/toolchains
fprime-util generate aarch64-linux
fprime-util build aarch-linux
```
> macOS users must run from within the Docker container in the directory `/project/LedBlinker`

## Running on Hardware

Now it is time to run on hardware. For this tutorial, the assumption is that the Arm Linux machine is available on the network, is running SSH, and the username, password, device address, and host address are known to the student. Without this configuration, users should skip to the next section of the tutorial.

First, in a terminal upload the software to hardware platform. This is done with:

```shell
# In led-blinker/LedBlinker

# For in-person workshops and ARM 64-bit hardware
scp -r build-artifacts/aarch64-linux <username>@<device-address>:LedBlinker

# For ARM 32-bit hardware
scp -r build-artifacts/arm-hf-linux <username>@<device-address>:LedBlinker
```
> Users must fill in the username and device address above.


Next run the F´ GDS without launching the native compilation (`-n`) and with the dictionary build above (`--dictionary ./build-artifacts/<platform name>/LedBlinkerAppDictionary.xml`).

```
# In led-blinker/LedBlinker

# For in-person workshops and ARM 64-bit hardware
fprime-gds -n --dictionary build-artifacts/aarch64-linux/dict/LedBlinkerAppDictionary.xml

# For ARM 32-bit hardware
fprime-gds -n --dictionary build-artifacts/aarch64-linux/dict/LedBlinkerAppDictionary.xml
```

>  The green circle will not appear until the software is run in the next step.

In another terminal SSH into the device and run the uploaded software:
```shell
ssh <username>@<device-address>
sudo LedBlinker/bin/LedBlinker -a <host-address> -p 50000
```
> User should fill in the username and device address above and ensure the execution of `LedBlinker` is supplied the address of the host computer (that ran the GDS).

> If the device does not connect, ensure that the firewall port `50000` is open on the host computer.

# Conclusion

Congratulations you've now run on hardware. It is time to unit test!


### Next Step: [Unit Testing](./unit-testing.md).
