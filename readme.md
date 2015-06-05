The Flying Jalapeno
=======

This is a generic testbed board that will act as the "brain and usual hardware" for 99% of testbeds. We currently design a ton of testbeds using the pro mini, but this requires us to also put more circuitry on the testbed itself. Since we use basically the same extra stuff on every testbed, it makes sense to have a custom board that includes all of this.

It will be strictly used in QC and production (a sub assembly). 

The title "The Flying Jalapeno". I know I'm a dork, but hey, might as well name it something fun. And why not be in honor of the best AVC team name ever??

The extra hardware necessary for most testbeds:

-2 adjustable vregs with high-side switches
-2 resistor groups used for pre-testing for shorts to GND
-2 IOs setup for capsense (basically just 10M resistors for each "send" line)
-Resistors in place for PASS/FAIL leds
-Larger Vreg for VCC, to make more current available as needed
-Universal connector (2/18 header) that will mate with future pogobeds. (The idea here is that most future pogobeds will be comprised of two things: (2) the FLying Jalapeno and (2) the custom pogopin hardware.
-1x6 SMD FTDI RA header for easy programming and debug

License Information
-------------------

All of the designs in this repo are public domain but you buy me a beer if you use this and we meet someday ([Beerware license](http://en.wikipedia.org/wiki/Beerware)).

