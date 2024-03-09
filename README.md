# ESP8266sunsetLED
Design Laboratory - grupa 5

This is a part of the SunsetLED project

We used addressable WS2812B led strip powered from the pins of ESP8266 (temporary solution as it was meant to be powered via external step-down converter). The program calculates the time difference between now and the timestamp sent by android app and after that period it turns on the lights. The program is set to turn the lights off after 3 hours. It was written using VS Code with PlatformIO plugin installed and used with CH340 drivers.

## Authors:
Hubert Kloc, Adrian Cich 
