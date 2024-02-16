# LED Blinker: Project Setup

> Note: if you have followed the [HelloWorld tutorial](https://fprime-community.github.io/fprime-tutorial-hello-world/) previously, this should feel very familiar...

An F´ Project ties to a specific version of tools to work with F´. In order to create
this project and install the correct version of tools, you should perform a bootstrap of F´.

To do this you should follow the following steps from the [F´ installation guide](https://nasa.github.io/fprime/INSTALL.html):

1. Ensure you meet the [F´ System Requirements](https://nasa.github.io/fprime/INSTALL.html#requirements)
2. [Bootstrap your F´ project](https://nasa.github.io/fprime/INSTALL.html#creating-a-new-f-project) with the name `MathProject`

Bootstrapping your F´ project created a folder called `MathProject` (or any name you chose) containing the standard F´ project structure as well as the virtual environment up containing the tools to work with F´.

Next, generate a build cache using the following commands:

```
cd led-blinker
fprime-util generate
```

### Next Step: [Requirements Specification](./requirements.md).
