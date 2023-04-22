# Full System Integration

 Now it is time to add a GPIO driver to our system and attach it to the `Led` component, and configure the driver to manage the GPIO 13 pin. Once this section is finished, the system is ready for running on hardware.

## Adding the GPIO Driver to the Topology

F´ provides a GPIO driver for Linux systems called `Drv.LinuxGpioDriver`. This should be added to both the instance definition list and the topology instance list. Since the GPIO driver is a passive component, its definition is simple.

Add to "Passive Component" section of `LedBLinker/Top/instance.fpp`:
```
  instance gpioDriver: Drv.LinuxGpioDriver base id 0x4C00
```

Add to the instance list of `LedBLinker/Top/topology.fpp`:
```
    instance gpioDriver
```

These steps are fairly similar to adding the `Led` component instance.

## Wiring The Led Component Instance to the LinuxGpioDriver Component Instance

The `Led` component defines the `gpioSet` output port and the `LinuxGpioDriver` defines the `gpioWrite` input port. These two ports need to be connected from output to input. We can create a named connections block in the topology and connect the two ports.

To do this, add the following lines to `LedBLinker/Top/topology.fpp`:
```
    connections LedConnections {
      # Led driver drives the GPIO
      led.gpioSet -> gpioDriver.gpioWrite
    }
```

## Configuring The GPIO Driver

So far the GPIO driver has been instantiated and wired, but has not been tool what GPIO pin to control. For this tutorial, GPIO pin 13 will be used. To configure this, the `open` function needs to be called in the topology C++ implementation.

This is done by adding the following line to the `configureTopology` function defined in `LedBLinker/Top/LedBLinkerTopology.cpp`.


```
void configureTopology() {
    ...
    bool gpio_success = gpioDriver.open(13, Drv::LinuxGpioDriver::GpioDirection::GPIO_OUT);
    if (gpio_success) {
        printf("[ERROR] Failed to open GPIO pin\n");
    }
}
```

This code tells the GPIO driver to open pin 13 as an output pin. If this fails, an error is printed to the console, but the system continues to start.