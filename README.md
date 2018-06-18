# Werwoelfe-vom-Duesterwald
Small Project for an Arduino Micro based playing Card replacement.
This is my first Arduino project at all and the first coding exercise for a very long period. This is just for fun and to play again a little with ESPs and some code.

## Introduction
Werwoelfe vom Duesterwald (https://en.wikipedia.org/wiki/The_Werewolves_of_Millers_Hollow) is a role play game that needs the players to secretly receive a role card assignment. With this project the physical cards are going to be replaced and the game moderator always has the possibility to review the assigned roles.
The device is placed in a chassis with a LCD display and 3 buttons (either Back/OK/Next, -/OK/+, or whatever the screen currently needs as input choices.

## Schematics
The LCD Pins SCL and SDA are connected to GPIO 20 & 4 of the Arduino. VCC and GND are beeing connected to the VIN and GND Pins.
The GPIOs 9, 10, 11 of the Arduino are each defined as Input and have a pulldown resistor against ground and a push button against 3,3V supply of the Arduino board.

## Parts list
* 1x Arduino Micro
* 3x Push button
* 1x 4x20 LCD (I2C)
* 3x 1kOhm Resistors

## Pictures
![Prototype](/images/Prototype.jpeg)
![Breadboard1](/images/20180610_141651.jpg)
![Breadboard2](/images/20180610_141658.jpg)
