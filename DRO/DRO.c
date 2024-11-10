#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/pio.h"

#include "Scale.pio.h"
#include "scale.h"  // Include the scale library header

/* useful functions
// find the current level of the RX FIFO
static unit pio_sm_get_rx_fifo_level (PIO pio, uint sm);

//get data from the RX FIFO
static uint32_t pio_sm_get_blocking (PIO pio, uint sm);
static uint32_t pio_sm_get (PIO pio, uint sm);
*/


int main()
{
    stdio_init_all();

    // initialise GPIO (Green LED connected to pin 25)
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1); // Set pin 25 to high

    // Initalize PIO
    uint sm1 = 0; //Statemachine for the first instance
    uint gpio1 = 2; //Pins for the first instance: pin 2 dor data and 3 for clock
    uint sm2 = 1; //Statemachine for the second instance
    uint gpio2 = 12; //Pins for the second instance: //Pins for the first instance pin 12 dor data and 13 for clock
    PIO pio = pio0;
    
    //first statemachine 
    // Initialize scale with PIO and state machine
    scale_init(pio, sm1, gpio1);

    //second statemachine 
    // Initialize scale with PIO and state machine 
    //scale_init(pio, sm2, gpio2);
        
    ScaleRawData reading; //initalize struct for reading data into

    //Main Loop 
    while(1)
    {
        
        // flash led just for testing
        gpio_put(25, 1); // Set pin 25 to high
        sleep_ms(100); // 0.5s delay
        gpio_put(25, 0); // Set pin 25 to low
        sleep_ms(100); // 0.5s delay
        
        //Example with a single scale 
        // Read scale data and process it
        int result;

        result = scale_read(pio, sm1, &reading);
        if (result == 1) 
        {
            // Print the result in mm and inches
            printf("%.2f mm   %.3f inches\n", reading.millimeters, reading.inches);
        }
        
        /* 
        //Example with two scales
        // Read scales and process it
        int result;

        result = scale_read(pio, sm1, &reading);
        if (result == 1) 
        {
            float Xreading = reading.millimeters;
            printf("X scale %.2f mm   ", Xreading);
        }

        result = scale_read(pio, sm2, &reading);
        if (result == 1) 
        {
            float Yreading = reading.millimeters;
            printf("Y scale %.2f mm\n", Yreading);
        }
        */

        sleep_ms(10);

    }
}
