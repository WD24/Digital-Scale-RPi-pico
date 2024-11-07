// scale.h

#ifndef SCALE_H
#define SCALE_H

#include "hardware/pio.h" // For PIO related functions

// Initializes the PIO for the scale with the given pins and state machine
void scale_init(PIO pio, uint sm, uint gpio_pin);


// Constants for unit conversions
#define MM_PER_INCH 25.4f
#define MM_DIVISOR 100.0f
#define INCH_DIVISOR 2000.0f

// Structure to hold the scale measurement data
typedef struct {
    float distance;  // Raw measurement value
    float millimeters;
    float inches;
    bool isImperial;
    bool isPositive;
} ScaleRawData;


// Reads the scale and updates the provided `ScaleRawData` structure
void scale_read(PIO pio, uint sm, ScaleRawData *reading);

#endif // SCALE_H
