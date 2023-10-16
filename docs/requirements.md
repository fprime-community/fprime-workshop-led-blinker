# Specifying Requirements

In this addendum to the tutorial, you will learn a bit about specifying requirements. Software requirements are derived from higher-level system requirements and represent the detail needed to implement the software.

## System Requirements

For this tutorial we have severl higher-level system requirements. These requirements would be defined by requirements specified by the electronics subsystem which are themselves derived by requirements defined at the full system level.

| Requirement     | Description                                              |
|-----------------|----------------------------------------------------------|
| ELECTRONICS-001 | The system shall blink an LED in response to a command.  |
| ELECTRONICS-002 | The blink rate of the LED shall be changeable in-flight. |

## Software Requirements

These are the requirements that the software team cares about. These dictate how the software should operate and trace to the higher-level system requirements. These also come with a verification method to ensure the softare meets these requirements.

Ideally, the software engineer would be handed these requirements, however; in-practice this is often a discussion between the software engineer and their system engineers. A sample requirement is provided below.

| Requirement     | Description                                                     | Derives From    | Verification |
|-----------------|-----------------------------------------------------------------|-----------------|--------------|
| LED-BLINKER-001 | The software shall start LED blinking in response to a command. | ELECTRONICS-001 | Unit Test    |

> Take a moment to identify some other requirements you might derive from the above electronics requirements. Do this before moving on to the next section.

## LED Blinker

Here we list a number of requirements for our led software to implement.

| Requirement     | Description                                                     | Derived From    | Verification |
|-----------------|-----------------------------------------------------------------|-----------------|--------------|
| LED-BLINKER-001 | The software shall start LED blinking in response to a command. | ELECTRONICS-001 | Unit Test    |
| LED-BLINKER-002 | The software shall stop LED blinking in response to a command.  | ELECTRONICS-001 | Unit Test    |
| LED-BLINKER-003 | The software shall telemeter the current LED blinking state.    | ELECTRONICS-001 | Unit Test    |
| LED-BLINKER-004 | The software shall emit events when the blinking state changes. | ELECTRONICS-001 | Unit Test    |
| LED-BLINKER-005 | The software shall store the blink interval as a parameter.     | ELECTRONICS-002 | Unit Test    |
| LED-BLINKER-006 | The software shall blink the LED using GPIO pin 13              | Electrical ICD  | Unit Test    |

> Notice how the software also includes a requirement that derived from the Electrical Interface Control Document. This captures the details of the software/hardware interface and is captured here as a requirement.