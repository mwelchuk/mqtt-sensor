# Power Monitor

This is the code for the optional power monitor. If running from a battery even
sleeping between measurements requires any voltage regulation to be active,
which will need to be able to provide enough current to run the ESP8266 at peak
power and thus is unlikely to be very efficiently providing the small amount
of current required to run the ESP8266 during sleep mode. Unless explicitly
wired to control the power to the ancillary circuitry (such as the temperature
and humidity sensor), this will also be constantly drawing current.

One option to mitigate this is to utilise a second, low-power microcontroller
that does controls power to the rest of device, is responsble for removing
power from it when requested and re-applying power after the required sleep
period. In this implementation an ATtiny2313 is used, running using the 128kHz
watchdog clock for it's system clock. This allows timer1 to be used to sleep
the device for much longer than the 15mins originally planned.

## Programming routine

First flash the firmware:

  make flash

Then will need to configure fuses to use 128kHz clock:

  avrdude -p attiny2313 -c usbtiny -U lfuse:w:0x66:m


### Reprogramming

To reprogram, first switch back to 8MHz clock:

  avrdude -B 250 -p attiny2313 -c usbtiny -U lfuse:w:0x64:m

Then follow instructions for programming.
