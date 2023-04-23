# Environment Setup

In order to run code on reference hardware, we need the ability to cross-compile for the given target architecture. ARM/Linux cross-compilers require a Linux computer in order to build the code. In this guide we will walk through how to set up Linux, cross-compilers, and F´ across the variety of computers students may be using.

> When attending an in-person workshop, please come having worked through the steps below.

### Step 0: Basic Requirements

Students running this tutorial must have a computer that meets the following basic requirements.

1. Computer running Windows 10, Mac OS X, or Ubuntu
2. Administrator access
3. 5GB of free disk space, 8 GB of RAM
4. Knowledge of the command line for your operating system (Bash, PowerShell, Zsh, etc).

We include a breakdown of setup for each common OS type. Users may consult the [troubleshooting](https://github.com/nasa/fprime/blob/master/docs/INSTALL.md#troubleshooting) section of the installation guide if problems arise.


## Step 1: Setting Up An Ubuntu Machine and Necessary Packages

In this section we will set up Linux to run on each type of laptop. Unless the laptop is already running Linux, this is  done through the use of virtual machines and/or emulation. Please follow the instructions for the operating system you run on your laptop. Our goal will be to get up and running with Ubuntu 20.04 or 22.04 on each type of laptop. *This does not require dual booting*.

> Users of other Linux distributsions will need to determine the correct packages for their distribution.


### Microsoft Windows 10

Windows 10 ships with a technology known as WSL. WSL allows users to run Linux virtual machines transparently within the Windows 10 operating system. In order to install WSL please run the following commands *in an Administrator PowerShell console*.

**PowerShell: Install WSL with Default Ubuntu**
```
wsl --install
```

To start Ubuntu under WSL, search for Ubuntu in the start menu and select the "Ubuntu on Windows" APP. All class commands should be run these Ubuntu terminals.

> Full instructions and troubleshooting help is available in the [Microsoft documentation](https://learn.microsoft.com/en-us/windows/wsl/install).

Once WSL is running, follow the below section [Ubuntu 20.04 / 22.04 / Generic Linux](#ubuntu-2004--2204--generic-linux) to install Ubuntu packages and cross-compilers.

Finally, Windows users must open up a firewall port and forward that port to WSL to ensure the hardware can call back into F´ ground data system running in WSL. First we'll need to note the IP address of the WSL machine. This is done with the following command *in an administrator PowerShell*.

```
wsl hostname -I
```
> Record the output of this command for the next step. For this guide, we will use the value `127.0.0.1`.

Next, we will add a firewall rule and forward it to the WSL instance. This is done with the following commands in *an administrator PowerShell*. Remember, to change `127.0.0.1` to the address you noted above.

> Warning: these commands work with the Windows firewall. Security and antivirus tools can run extra firewalls. Users  must allow the port `50000` or disable these extra firewalls.

**PowerShell: Add and Forward External Firewall Rule**
```bash
New-NetFirewallRule -DisplayName "fprime" -Direction inbound -Profile Any -Action Allow -LocalPort 50000 -Protocol TCP

netsh interface portproxy add v4tov4 listenport=50000 listenaddress=0.0.0.0 connectport=50000 connectaddress=127.0.0.1
```
> Remember to change `127.0.0.1` to your recorded ip address as discovered with `wsl hostname -I`. Users are advised to remove this rule after the class has been completed.

Windows users should now continue to install packages in their WSL instance following the next section.

### Ubuntu 20.04 / 22.04 / Generic Linux

On Ubuntu, users just need to ensure that the basic packages are installed. This can be done with the following commands:

```bash
sudo apt update
sudo apt install build-essential git g++ gdb cmake python3 python3-venv python3-pip 
```

Installing the cross-compilers will use the pre-built packages provided by ARM. Follow these instructions to install these tools for the target hardware into the `/opt/toolchains` directory.

```bash
sudo mkdir -p /opt/toolchains
sudo chown $USER /opt/toolchains
# For in-person workshops, and users running on 64-bit ARM
curl -Ls https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-x86_64-aarch64-none-linux-gnu.tar.xz | tar -JC /opt/toolchains --strip-components=1 -x
# For users running on 32-bit ARM
curl -Ls https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf.tar.xz | tar -JC /opt/toolchains --strip-components=1 -x
```

### Mac OS X

MacOS like Linux is a unix system and thus may be used directly for most of this class. However, Mac users must install
the following utilities *and ensure they are available on the command line path*.

1. [Python 3](https://www.python.org/downloads/release/python-3913/)
2. [CMake](https://cmake.org/download/)
3. GCC/CLang typically installed with xcode-select

**Installing GCC/CLang on macOS**
```bash
xcode-select --install
```

Installing Python and running the above command to install gcc/CLang should ensure that those tools are on the path.
CMake requires one additional step to ensure it is on the path:

```bash
sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
```

In order to cross-compile for the ARM chips for this class, a Linux box is essential. Mac users may choose to pair with
team members who have Linux/WSL setup, or may choose to follow the instructions in [Appendix I](./appendix-1.md)
to install a docker container including the necessary tools.

