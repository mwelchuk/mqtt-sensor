# ESP8266 Temperature and Humidity Sensor

This is the main firmware for the ESP8266, implementing a temperature and
humidity sensor which reports its readings via MQTT. This firmware can be built
to support either the DHT22 or HTU21D sensors. The right libraries will need to
be installed to build depending on the choice of sensor.

It is fairly adaptable being able to run on a number of different ESP8266
boards.

The firmware is compiled through the Arduino IDE.
