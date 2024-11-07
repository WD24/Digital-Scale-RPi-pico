THIS LIBRARY IS STILL UNDER DEVELOPMENT The code may be messy at best and may not work.

Using the PIO on the Raspberry Pi Pico this code reads the clock and data outputs from a digital scale or caliper (from reading around the internet I believe it is outputting bin6 data format (24 bit)).
The data is in six words of four bits, The first four words (2 bytes) gives the measurement as an integer the fifth word appears to be null (it might be an overflow for the measurement but my scale didn't need it if it is.) the sixth word gives two booleans. The first bit is for if the number is positive or negative and the last bit is for if the number is metric or imperial.

This code uses one state machine for each scale connected. Most of the PIO code identify the start bit and clears the output shift register.  As a result the code reads the data from the scale every other datagram (approximately four times a second).

I have tested this code on a M-SURE MS-276-300 scale running on 3 volts. The M-sure scales have a mini usb plug but do not use the standard usb wiring, instead it used the USB vbus for data, D- for clock D+ for Gnd and ID for 3 volts in. 

The data lines and clock lines need to be adjacent to each other on the pico's gpio lines with the data line first.
Level shifting might also be needed if you are using a scale that is outputting 1.5 vols.

To use:

Add Scale.h, Scale.c and Scale.pio to your project folder.

In your CMakeList.txt add: 
"Scale.c" to the add_executable:  add_executable(myproject main.c Scale.c) 
Generate the PIO headder file with: pico_generate_pio_header (myproject ${CMAKE_CURRENT_LIST_DIR}/Scale.pio)
make sure you have the pico standard library, hardware GPIO library and  hardware PIO library: target_link_libraries(myproject pico_stdlib hardware_gpio hardware_pio)

In our main program file add:
#include "Scale.pio.h"
#include "scale.h"  // Include the scale library header 

and ensure you have added the hardware PIO and GPIO libraries:
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"


To Initialize the PIO state machine:
    scale_init(pio, sm, gpio);
    example: scale_init(0, 2, 5);

Where: pio is the PIO (0 or 1 (or 2 on the new pico2), sm is the state machine you wish to use, and gpio is the first pin for data transfer the adjacent pin will be used for clock. In the above example it s using PIO0, state machine 2 and pin 5 for data and pin 6 for clock.

To read the scale:

        scale_read(pio, sm, &reading); // Read scale data and process it

This tells the pico to read the state machine's output FIFO and store the data in a struct. 
    float distance; 
    bool isImperial;
    bool isPositive;
    // Raw measurement values used for conversion into useful data.
    
    float millimeters;
    float inches;
    //This is the converted data, into a useful format. Depending on the scale's output one unit will be converted from the raw data and the other will be calculated by the pico. Inorder to find out if your scale is out putting metric or imperial data check the isImperial boolean.

To acces the data:
    printf("%.2f mm   %.3f inches\n", reading.millimeters, reading.inches);
This outputs both the millimetre and inch data to the serial console. 
