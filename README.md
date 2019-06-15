# jazcom-car-bluetooth
Arduino implementation of JazCom Car bluetooth processor
Designed to pair with https://github.com/jazeee/jazcom-car

This displays messages from Bluetooth module HC06 on a 0.96" I2C display.
Arduino Nano or Uno code.

## Wiring
* OLED GND – Arduino GND
* OLED VCC – Arduino 5V
* OLED SCL – Arduino Uno A5
* OLED SDA – Arduino Uno A4

* HC06 TX - Arduino RX (Pin 0)
* HX06 RX - using voltage divider (1K, 2K) Arduino TX (Pin 1). Must feed 3.3V to HC06
* HC06 VCC - Arduino 5V
* HC06 GND - Arduino GND

Note - When programming the Arduino, must disconnect Serial Pins to HC06.

## Messaging

Since the Bluetooth module pumps serial data out regardless of source, the code
parses and splits the data based on prefix.

`SerialStatusBytes` -> top yellow bar
`JAZ:DataBytes` -> bottom blue area. 
  If `DataBytes` is 3 chars or less, it will be large
  If `DataBytes` is 2 chars long, it will have one space pad on the left.
    The intention is to display 2 or 3 digit values right aligned.

Any message longer than 10 characters will be split and displayed in chunks with a delay.
