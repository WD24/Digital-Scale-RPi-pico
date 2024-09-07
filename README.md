Using the PIO on the Raspberry Pi Pico this code reads the clock and data outputs from a digital scale or caliper (from reading around the internet I believe it is outputting bin6 data format (24 bit)).
The data is in six words of four bits, The first four words (2 bytes) gives the measurment as an integer the fith word appears to be null (it might be an overflow for the measurment but my scale didnt need it if it is.) the sixth word gives two booeans. The first bit is for if the number is positive or negative and the last bit is for if the number is metric or imperial.

This code uses one statemachine for each scale connected. Most of the PIO code identifys the start bit and clears the output shift register.  As a result the code reads the data from the scale every other datagram (approximatly four times a second).

I have tested this code on a M-SURE MS-276-300 scale running on 3 volts. The M-sure scales have a mini usb plug but do not use the standard usb wireing, instead it used the USB vbus for data, D- for clock D+ for Gnd and ID for 3volts in. 

The data lines and clock lines need to be adjacent to each other on the pico's gpio lines with the data line first.
Level shifting might also be needed if you are using a scale that is outputting 1.5 vols.
