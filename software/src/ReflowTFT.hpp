#ifndef __REFLOWTFT_H_INCLUDED__
#define __REFLOWTFT_H_INCLUDED__

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

// Colour definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

// Rotation constants for the Adafruit TFT library
static const uint8_t ROTATE_0_DEGREES = 0;
static const uint8_t ROTATE_90_DEGREES = 1;
static const uint8_t ROTATE_180_DEGREES = 2;
static const uint8_t ROTATE_270_DEGREES = 3;

// Locations
static const int16_t TEMP_TEXT_X_POS = 5;
static const int16_t TEMP_TEXT_Y_POS = 100;

// Pre-defined strings
static const char TEMP_END_C_STRING[] = { (char) 247, 'C', '\0'}; // degrees celcius

class ReflowTFT : Adafruit_ST7735 {

public:
  // Constructor
  ReflowTFT(int8_t cs, int8_t dc, int8_t rst) : Adafruit_ST7735(cs, dc, rst) {};
  
  // Initialise the display
  void init();
  // Update the temperature text display
  void updateScreenTempText(int temp);  
};

#endif // __REFLOWTFT_H_INCLUDED__