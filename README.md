Using the PIO on the Raspberry Pi Pico this code reads the clock and data outputs from a digital scale or caliper (from reading around the internet I believe it is outputting bin6 data format (24 bit)).

The data lines and clock lines need to be adjacent to each other on the pico's gpio lines with the data line first.
Level shifting might also be needed if you are using a scale that is outputting 1.5 vols.
