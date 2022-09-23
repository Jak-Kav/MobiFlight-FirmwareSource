/**
 * Open A3XX EFIS LCD
 * Written by: James Kavanagh & Keith Greenwood
 * This library has been written to drive the 'OpenA3XX' EFIS LCD Display.
 */

#ifndef _Open_A3XX_EFIS_LCD_h
#define _Open_A3XX_EFIS_LCD_h

#define BUFFER_SIZE_MAX 16

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "HT1621.h"

class Open_A3XX_EFIS_LCD {
private:
  // Fields
  HT1621 ht_efis;
  uint8_t buffer[BUFFER_SIZE_MAX];
  bool _initialised;
  byte _CS;
  byte _CLK;
  byte _DATA;

  // Methods
  void handleMobiFlightCmd(char *string);
  void displayDigit(uint8_t address, uint8_t digit);
  //void setBufferBit(uint8_t address, uint8_t bit, uint8_t enabled);
  void refreshLCD(uint8_t address);
  
public:
  // Constructor
  // 'CLK' is sometimes referred to as 'RW'
  Open_A3XX_EFIS_LCD(uint8_t CS = 2, uint8_t CLK = 3, uint8_t DATA = 4) : ht_efis(CS, CLK, DATA) { };

  void begin();
  void clearLCD();
  void attach(byte CS, byte CLK, byte DATA);
  void detach();
  void handleMobiFlightRaw(char *string);

  // Set QFE or QNH functions
  void setQFE(bool enabled);
  void setQNH(bool enabled);
  // Set Dot Function
  void setDot(bool enabled);
  // Show Std function
  void showStd(int16_t state);

  // Show QFE function
  void showQFEValue(int16_t value);
  // Show QNH function
  void showQNHValue(int16_t value);
};

#endif //Open_A3XX_EFIS_LCD_h
