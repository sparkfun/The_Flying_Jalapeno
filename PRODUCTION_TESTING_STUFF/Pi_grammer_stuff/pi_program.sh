#!/bin/bash

echo "Programming beginning..."

# get firmware file name
firmware=$(find /home/pi/*.hex)
$firmware .= "/home/pi/$firmware"

#FUSE BITS
HIGH_FUSE=0xD8
LOW_FUSE=0xFF
EXT_FUSE=0xFD # due to masking, 0x05 = 0xFD (FD was the original ext fuse for the mega pro) *** Note, the mega verifies back as FD
LOCK=0x0F

#to verify that avrdude can connect to the chip
#sudo avrdude -p atmega328p -C ~/avrdude_gpio.conf -c pi_1 -v

#erase and fuse bits, note the -i 100 to add delay and slow down initial com to fresh chips
sudo avrdude -p atmega2560 -C /home/pi/avrdude_gpio.conf -c pi_1 -i 1000 -D -v -e -u -U hfuse:w:$HIGH_FUSE:m -u -U lfuse:w:$LOW_FUSE:m -u -U efuse:w:$EXT_FUSE:m 2>/home/pi/fuse_results.txt

#program flash and lock bits
# extended should be 0xFD, but they only verify at 0x05, 
# also lock fuses should be 0xCF, but they only verify at 0x0F
sudo avrdude -p atmega2560 -C /home/pi/avrdude_gpio.conf  -c pi_1 -D -v -u -U flash:w:$firmware:i -u -U lock:w:$LOCK:m 2>/home/pi/flash_results.txt


