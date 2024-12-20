THIS LIBRARY IS STILL UNDER DEVELOPMENT The code may be messy at best and may not work and is likly to change without notice.

Using the PIO on the Raspberry Pi Pico this code reads the clock and data outputs from a digital scale or caliper (from reading around the internet I believe it is outputting bin6 data format (24 bit)).
The data is in six words of four bits, The first four words (2 bytes) gives the measurement as an integer the fifth word appears to be null (it might be an overflow for the measurement but my scale didn't need it if it is.) the sixth word gives two booleans. The first bit is for if the number is positive or negative and the last bit is for if the number is metric or imperial.

![Datagram example](https://github.com/user-attachments/assets/f7995fb9-89e6-450b-a9dc-31992b5326b1)

This code uses one state machine for each scale connected. Most of the PIO code identify the start bit and clears the output shift register.  As a result the code reads the data from the scale every other datagram (approximately four times a second).

I have tested this code on a M-SURE MS-276-300 scale running on 3 volts. The M-sure scales have a mini usb plug but do not use the standard usb wiring, instead it used the USB vbus for data, D- for clock D+ for Gnd and ID for 3 volts in. 

![P1200118](https://github.com/user-attachments/assets/bd9da444-4988-48bf-8299-891a625fcde1)


The data lines and clock lines need to be adjacent to each other on the pico's gpio lines with the data line first.
Level shifting might also be needed if you are using a scale that is outputting 1.5 vols.

How to use:

Add Scale.h, Scale.c and Scale.pio to your project folder.

In your CMakeList.txt add: 
"Scale.c" to the add_executable:  add_executable(myproject main.c Scale.c) 
Generate the PIO headder file with: pico_generate_pio_header (myproject ${CMAKE_CURRENT_LIST_DIR}/Scale.pio)
make sure you have the pico standard library, hardware GPIO library and  hardware PIO library: target_link_libraries(myproject pico_stdlib hardware_gpio hardware_pio)

In your main program file add:

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

In the main function int main()
Initialize the struct for storing the data with:

ScaleRawData reading;

To read the scale:

        scale_read(pio, sm, &reading); // Read scale data and process it

This tells the pico to read the state machine's output FIFO and store the data in a struct. 

    float distance; 
    
    bool isImperial;
    
    bool isPositive;
    
These are raw measurement values used for conversion into useful data.
    
    float millimeters;
    
    float inches;
    
This is the converted data, into a useful format. Depending on the scale's output one unit will be converted from the raw data and the other will be calculated by the pico. If you want to find out if your scale is out putting metric or imperial data check the isImperial boolean.

The function scale_read() will store the data in a struct it also returns a value of 1 if the scale was read or 0 if there was no data to read.

    int result = scale_read(pio, sm2, &reading);

returns a 1 or 0.

To acces the data:

    printf("%.2f mm   %.3f inches\n", reading.millimeters, reading.inches);
    
This outputs both the millimetre and inch data to the serial console. 

    float Scale_Reading = reading.millimeters;

This puts the data in to a float for further processing by your sketch
