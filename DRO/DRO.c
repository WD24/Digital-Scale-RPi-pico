#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "Scale.pio.h"

typedef struct
{
    float Distance;
    float milimeters;
    float inch;
    bool Imperial;
    bool Positive;
} ScaleRawData;

void ScaleRead(PIO ScalePIO, uint sm, ScaleRawData *reading)
{
        // get measurment
        uint32_t GetMeasurment = pio_sm_get_blocking(ScalePIO, sm);

        uint8_t byte1 = (GetMeasurment >> 16 & 0xFF);// get the measurment second byte
        uint8_t byte2 = (GetMeasurment >> 8 & 0xFF);// get the measurment first byte
        uint8_t byte3 = (GetMeasurment >> 24 & 0xFF); // get the imperial and positive/negative bit as one byte
        
        reading->Distance = (float) ((byte1 << 8) | byte2);
        reading->Imperial = (byte3 >> 7);// if true measurment in inches divide by 200.00
        reading->Positive = (byte3 >> 4 );// if false measurment is negative
        bool Imp = (byte3 >> 7);
        bool Pos = (byte3 >> 4);

        if (Imp == 0)
        {
            reading->milimeters = (float) ((byte1 << 8) | byte2) / 100.00; // create the measurment in mm
            // add a conversion to inch else the struct will be using an old inch measurment
            reading->inch = reading->milimeters / 25.4;
            if (Pos == 0)
            {
                reading->milimeters = -reading->milimeters;
                reading->inch = -reading->inch;
            }
        }
        else
        {
            reading->inch = (float) ((byte1 << 8) | byte2) / 2000.00; // create the measurment in inch
            // add a conversion to mm else the struct will be using an old mm measurment
            reading->milimeters = reading->inch * 25.4;
            if (Pos == 0)
            {
                reading->inch = -reading->inch;
                reading->milimeters = -reading->milimeters;
            }
        }
}


int main()
{
    stdio_init_all();

    // initialise GPIO (Green LED connected to pin 25)
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1); // Set pin 25 to high

    // Initalize PIO
    uint sm1 = 0; //Statemachine for the first instance
    uint gpio1 = 4; //Pins for the first instance
    uint sm2 = 1; //Statemachine for the second instance
    uint gpio2 = 6; //Pins for the second instance
    PIO pio = pio0;
    
    //first statemachine 
    uint offset = pio_add_program(pio, &bin6Data_program);
    bin6Data_program_init(pio, sm1, offset, gpio1);

    //second statemachine 
    //bin6Data_program_init(pio, sm2, offset, gpio2);
        
    ScaleRawData reading; //initalize struct for reading data into

    //Main Loop 
    while(1){
        
        // flash led just for testing
        gpio_put(25, 1); // Set pin 25 to high
        sleep_ms(100); // 0.5s delay
        gpio_put(25, 0); // Set pin 25 to low
        sleep_ms(100); // 0.5s delay
        
        ScaleRead(pio, sm1, &reading);
        printf("%.2fmm   %.3fInch\n", reading.milimeters, reading.inch);

        sleep_ms(10);

    }
}
