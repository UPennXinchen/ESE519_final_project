# ESE519_final_project
We hereby propose to design a system that could automatically detect, display the real-time temperature with a temperature sensor. With two Arduino Unos, the system not only senses and environment
temperature with the sensor, but also sends proper visual warning or alarm to users under certain
circumstances. A mini fan is introduced to the system, which helps cool down the environment temperature when the sensor senses temperature is too high. Users could seize control the fan with their
mobile devices which means users have the highest priority. And the connection is through a WIFI
module ESP8266. When there’s no users’ intervention, the mini fan can run by itself if the temperature
goes high.

########
First_arduino sense the ADC, control the LEDs, buzzer and LCD display.
Second_arduino sense the ADC, control the FAN with PWM and connect to Node MCU.
