#include "palette.h"

/**
 * Sets the RGB values for a specific index in the palette.
 *
 * @param index The index in the palette to set the RGB values.
 * @param r The red component value.
 * @param g The green component value.
 * @param b The blue component value.
 */
void setRGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    palette[index][0] = r; // Set red component
    palette[index][1] = g; // Set green component
    palette[index][2] = b; // Set blue component
}

/**
 * Generates a random color palette based on predefined types.
 * The palette is filled with different gradient effects depending on the selected type.
 */
void generatePalette() {
    // seed the random number generator with the current time to ensure different results each time
    srand((unsigned int)time(NULL));

    // randomly select a palette type from 0 to 3
    int paletteType = rand() % 4;

    // generate the palette based on the selected type
    switch (paletteType) {
        case 0: // "purple majik"
            // fill the first 64 colors with a gradient effect
            for (int a = 0; a < 64; a++) {
                setRGB(a, a, a * a / 64, (uint8_t)(sqrtf(a) * 8));
            }
            // set the remaining colors to maximum intensity white
            for (int a = 64; a < PALETTE_SIZE; a++) {
                setRGB(a, MAX_COLOR, MAX_COLOR, MAX_COLOR);
            }
            break;

        case 1: // "green lantern II"
            // fill the first 64 colors with a different gradient effect
            for (int a = 0; a < 64; a++) {
                setRGB(a, a * a / 64, (uint8_t)(sqrtf(a) * 8), a);
            }
            // set the remaining colors to maximum intensity white
            for (int a = 64; a < PALETTE_SIZE; a++) {
                setRGB(a, MAX_COLOR, MAX_COLOR, MAX_COLOR);
            }
            break;

        case 2: // "amber sun"
            // fill the first 64 colors with yet another gradient effect
            for (int a = 0; a < 64; a++) {
                setRGB(a, (uint8_t)(sqrtf(a) * 8), a, a * a / 64);
            }
            // gradually fade the remaining colors to darkness
            for (int a = 64; a < PALETTE_SIZE; a++) {
                uint8_t fadeValue = (uint8_t)((PALETTE_SIZE - a) * MAX_COLOR / (PALETTE_SIZE - 64));
                setRGB(a, fadeValue, fadeValue, fadeValue);
            }
            break;

        case 3: // "frosty"
            // fill the first 64 colors with a cool gradient effect
            for (int a = 0; a < 64; a++) {
                setRGB(a, a * a / 64, a, (uint8_t)(sqrtf(a) * 8));
            }
            // set the remaining colors to maximum intensity white
            for (int a = 64; a < PALETTE_SIZE; a++) {
                setRGB(a, MAX_COLOR, MAX_COLOR, MAX_COLOR);
            }
            break;
    }
}

/**
 * Applies the current palette to the canvas, updating each pixel's color.
 * Regenerates the palette if an energy spike is detected and sufficient time has elapsed.
 *
 * @param currentTime The current time in milliseconds.
 * @param canvas The canvas buffer to apply the palette to.
 * @param width The width of the canvas in pixels.
 * @param height The height of the canvas in pixels.
 */
void applyPaletteToCanvas(size_t currentTime, uint8_t *canvas, size_t width, size_t height) {
    size_t frameSize = width * height;

    // check if it's time to regenerate the palette based on energy spikes and time elapsed
    if ((energySpikeDetected && currentTime - lastPaletteInitTime > 10 * 1000) || lastPaletteInitTime == 0) {
        generatePalette(); // reinitialize the palette
        lastPaletteInitTime = currentTime; // update the last initialization time
    }
    
    // apply the current palette to each pixel in the canvas
    for (size_t i = 0; i < frameSize; i++) {
        uint8_t colorIndex = canvas[i * 4]; // use the red channel as the intensity index

        // map the palette colors to the RGBA format in the canvas
        canvas[i * 4] = palette[colorIndex][0];       // R
        canvas[i * 4 + 1] = palette[colorIndex][1];   // G
        canvas[i * 4 + 2] = palette[colorIndex][2];   // B
        canvas[i * 4 + 3] = 255;                      // A (fully opaque)
    }
}
