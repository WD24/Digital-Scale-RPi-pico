// scale.c

#include "scale.h"
#include "Scale.pio.h"  // PIO program binary


// Initializes the PIO for the scale with the given pins and state machine
void scale_init(PIO pio, uint sm, uint gpio_pin) 
{
    // Load the program to the PIO
    uint offset = pio_add_program(pio, &bin6Data_program);
    // Initialize the PIO state machine with the appropriate pin
    bin6Data_program_init(pio, sm, offset, gpio_pin);
}

// Reads the scale and updates the provided ScaleRawData structure
void scale_read(PIO pio, uint sm, ScaleRawData *reading)
{
    // Get the raw measurement from the PIO
    uint32_t measurement = pio_sm_get_blocking(pio, sm);

    // Extract bytes
    uint8_t byte1 = (measurement >> 16) & 0xFF;
    uint8_t byte2 = (measurement >> 8) & 0xFF;
    uint8_t byte3 = (measurement >> 24) & 0xFF;

    // Combine byte1 and byte2 to get the distance value
    reading->distance = (float)((byte1 << 8) | byte2);

    // Check if the measurement is in Imperial (inches) or Metric (millimeters)
    reading->isImperial = (byte3 >> 7) & 1;
    // Check if the measurement is positive or negative
    reading->isPositive = (byte3 >> 4) & 1;

    // Process the data based on whether it's Imperial or Metric
    if (reading->isImperial) {
        // Convert from inches to millimeters
        reading->inches = reading->distance / INCH_DIVISOR;
        reading->millimeters = reading->inches * MM_PER_INCH;
    } else {
        // Convert from millimeters to inches
        reading->millimeters = reading->distance / MM_DIVISOR;
        reading->inches = reading->millimeters / MM_PER_INCH;
    }

    // If the measurement is negative, adjust both units
    if (!reading->isPositive) {
        reading->inches = -reading->inches;
        reading->millimeters = -reading->millimeters;
    }
}