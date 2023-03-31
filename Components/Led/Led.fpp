module Components {
    @ Component to blink an LED driven by a rate group
    active component Led {
        @ Command to turn on or off the blinking LED
        async command BLINKING_ON_OFF(on_off: Fw.On) opcode 0

        @ Blinking interval in rate group ticks
        param BLINK_INTERVAL: U32

        @ Event logged when blinking is turned on or off
        event BlinkingState(on_off: Fw.On) severity activity high id 0 format "Blinking state set to {}"

        @ Event logged when the LED turns on or off
        event LedState(on_off: Fw.On) severity activity low id 1 format "LED is {}"

        @ Event logged when the LED blink interval is updated
        event BlinkIntervalSet(interval: U32) severity activity high id 2 format "LED blink interval set to {}"

        @ Telemetry channel counting LED transitions
        telemetry LedTransitions: U64

        @ Port receiving calls from the rate group
        sync input port run: Svc.Sched

        @ Port sending calls to the GPIO driver
        output port gpioSet: Drv.GpioWrite

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut
    }
}