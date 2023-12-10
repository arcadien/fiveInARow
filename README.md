# Five In A Row

Set of two applications targetting Atmel/Microchip [ATMega328P](https://www.microchip.com/en-us/product/atmega328p), implementing shooting game.

Using a *gun*, users tries to hit *targets*.
The game is up to four players: yellow, green, red and blue.
There are 5 targets.

### Game mode

#### Biathlon like
Each player turn gives 5 shots. Player tries to hit each target. Total number of shoots is counted, as well as successfull hits.

## Functional details
### target

The **target** application manages player points and turns. It communicates with an Android application implemented on top of [Bluetooth Electronics](https://www.keuwl.com/apps/bluetoothelectronics/) application. It manages the game logic and player points.


### Gun
The **Gun** application manages the gun. Using two buttons, it allows:

| Press |      Button 1    |             Button 2           |
|-------|------------------|--------------------------------|
| short |       shoot      | Begin new round (give 5 shots) |
| long  |     power off    | continuous laser (calibration) |

Gun application manages following outputs:
* Laser
* vibrator
* 128x32 I²C display 

## Architecture details

Some parts of the projet, implementing logic, can be used on both native and target platform.
All that code is represented in the "domain" package below.

Both **target** and **Gun** application are run both native and cross environment. The current
target hardware for both is the [ATMega328p](https://www.microchip.com/en-us/product/atmega328p) chip from Atmel/Microchip.
To facilitate testing and extension, user interface (Gui) and hardware abstraction (HAL) are provided using interfaces. Implementation exists for native and cross (i.e. testing) environments.

Note: The *BTEGui* implementation can be used in native environment. Based on plain string exchange, if it may be difficult to interpret.

![Architecture overview](architecture.png)
