# Smart_Arduino-Alarm-System
Create a clock Box with Multiple Function using Arduino and DS3231 Module

Note: Pictures including circuit diagram and Test_picture are placed in a separate folder while the Test Video Link is provided below....

This projct make use of Arduino Pro Mini as the heart of the system, and DS3231 as the clock module with the help of 128x64 Adafruit Monochrome OLED for the display, 5 big push buttons for settings the function and setup of the system, this simple mini box displays day and time in real mode as well as the Alarm time been set also make use of 5v relay board which is used for control of appliances that are meant to be timed, and two leds for indication of the system status (Mode), the system can be manually controlled (Alarm Mode and Relay status) it supports free use of setting up when operating and it can be powered by 9-24V DC adapter with the of the buck converter inside it for Voltage regulator, also with a possession of a 3-12V buzzer for Alarm time reminder and mode selection notification.

>>>Components Used

1. [Arduino ProMini](https://docs.arduino.cc/retired/getting-started-guides/ArduinoProMini)
2. [DS3231](https://lastminuteengineers.com/ds3231-rtc-arduino-tutorial/)
3. [SSD1306](https://lastminuteengineers.com/oled-display-arduino-tutorial/)
4. [5v Relay Module](https://www.instructables.com/5V-Relay-Raspberry-Pi/)
5. [3-12V Buzzer](https://www.cuidevices.com/blog/buzzer-basics-technologies-tones-and-driving-circuits)
6. 9v DC Adapter
7. [LM2596](https://www.instructables.com/The-Introduction-of-LM2596-Step-Down-Power-Module-/) Buck Converter
8. LEDs
9. [Push Buttons](https://en.wikipedia.org/wiki/Push-button)

Library Used (They are in the file or you check on the link for more information)

1. Adafruit [SSD11306](https://www.arduino.cc/reference/en/libraries/ssd1306/) Library
2. [Adafruit GFX](https://www.arduino.cc/reference/en/libraries/adafruit-gfx-library/) Library
3. [TonePitch](https://www.arduino.cc/reference/en/libraries/tonepitch/) Library
4. [RTCLib]() Library

Software/Language used:

1. [Fritzing](https://fritzing.org/) for Circuit Diagram design
2. [Proteus](https://www.labcenter.com/) for Simulation
3. Arduino for programming


Methodolgy (System Workout and control)

The system uses 4 buttons for all setup and one button for Home selection and Alarm mode selection. one of the button is used for Date and time button while one is also used for Alarm setup and the remaining two buttons were for Increment and decrement and also as Relay Status control when not in setup mode. the home buttons can be used to disable/enable the alarm for the Buzzer to beep or remain silent. beside the box was a connector which is well labelled for the connect of the relay and the load to be connected to the system. other information can be derived from the video.


