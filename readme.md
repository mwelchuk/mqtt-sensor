# ESP8266 based temperature and humidity sensor firmware.

This project is to create a low powered temperature and humidity sensor. It is
hoped to achieve ~9 months life from either a small LiPo battery or a good time
with a couple of AAA cells.

The ESP8266 has been chosen for this project for the main microcontroller due
to it's WiFi communications. The 433MHz radio chips may provide a lower power
solution, however being able to use the existing WiFi infrastructure was seen
as a worthwhile compromise.

The firmware supports using an ATtiny2313 as an optional power controller as
this is thought to reduce overall power consumption.
