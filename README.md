# fiveTargetGame

Set of two applications targetting Microchip AVR chips, implementing shooting game.
The game is made of two parts: the *gun* and the *targets*.
There is up to four players, trying to shoot five targets in a row, using 5 tries.

At some point, games ends and the player with best shot/hit ratio wins the game.

## target
The **target** application manages player points and communicate with an Android application implemented on top of [Bluetooth Electronics](https://www.keuwl.com/apps/bluetoothelectronics/) application. It manages the game logic and player points.


## Gun
The **Gun** application manages the gun. Using two buttons, it allows:

| Press |      Button 1    |             Button 2           |
|-------|------------------|--------------------------------|
| short | power on / shoot | Begin new round (give 5 shots) |
| long  |     power off    | continuous laser (calibration) |

Gun application manages following outputs:
* Laser
* vibrator
* led

For more details of gun logic, see test cases.

