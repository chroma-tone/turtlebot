##
Using micropython to build a bot
##

http://micropython.org/download/#esp8266

1. Erasing old firmware

```
sudo ./esptool.py --port /dev/ttyUSB0 erase_flash
```

1. flashing new firmware

```
sudo ./esptool.py --port /dev/ttyUSB0 write_flash --flash_size=8m 0 esp8266-20160809-v1.8.3.bin
```

1. connect a seriala port terminal such as minicom or realterm, connect using 115200 baudrate

Confirm you receive a python prompt

1. install ampy: pip install adafruit-ampy

```
sudo ampy --port /dev/ttyUSB0 ls 
```
should return ['boot.py']

```
sudo ampy --port /dev/ttyUSB0 put 