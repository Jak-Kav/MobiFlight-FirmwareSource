#include "Open_A3XX_FCU_LCD.h"

// Define the rows (Each row has 8 bits)
#define SPD_HUN   0
#define SPD_TEN   1
#define SPD_UNIT  2
#define HDG_HUN   3
#define HDG_TEN   4
#define HDG_UNIT  5
#define ALT_TTH   6
#define ALT_THO   7
#define ALT_HUN   8
#define ALT_TEN   9
#define ALT_UNIT  10
#define VRT_THO   11
#define VRT_HUN   12
#define VRT_TEN   13
#define VRT_UNIT  14
#define SPECIALS  15
// Define the buffer bits
#define SET_BUFF_BITS(addr, bitMask, enabledMask)   buffer[addr] = (buffer[addr] & (~(bitMask))) | (enabledMask)
#define SET_BUFF_BIT(addr, bit, enabled)            buffer[addr] = (buffer[addr] & (~(1<<(bit)))) | (((enabled&1))<<(bit))

/**
 * Method to begin the LCD.
 * Sets up the driver chip, clears the LCD, initialises the buffer to all '0'.
 * Sets up the backlight and turns it on (This will need to be changed for release, to ensure this only happens if a backlight is setup).
 * Initialises the labels that need to be always on.
 */
void Open_A3XX_FCU_LCD::begin() {
  // Set up the driver chip
  ht.begin();
  ht.sendCommand(HT1621::RC256K);
  ht.sendCommand(HT1621::BIAS_THIRD_4_COM);
  ht.sendCommand(HT1621::SYS_EN);
  ht.sendCommand(HT1621::LCD_ON);
  // This clears the LCD
  for (uint8_t i = 0; i < ht.MAX_ADDR; i++)
      ht.write(i, 0);      
  // Initialises the buffer to all 0's.
  memset(buffer, 0, BUFFER_SIZE_MAX);
  pinMode(_B_LIGHT, OUTPUT);
  digitalWrite(_B_LIGHT, HIGH);
  setStartLabels();
}

/**
 * Function in the setup to declare the pins, set it to initialised and trigger to 'begin' method.
 * @variable CS       The declared 'CS' pin
 * @variable CLK      The declared 'CLK' pin
 * @variable DATA     The declared 'DATA' pin
 * @variable B_LIGHT  The declared 'B_LIGHT' pin
 */
void Open_A3XX_FCU_LCD::attach(byte CS, byte CLK, byte DATA, byte B_LIGHT)
{
  _CS = CS;
  _CLK = CLK;
  _DATA = DATA;
  _B_LIGHT = B_LIGHT;
  _initialised = true;
  begin();
}

/**
 * Method to detach the device.
 */
void Open_A3XX_FCU_LCD::detach()
{
  if (!_initialised)
    return;
  _initialised = false;
}

/**
 * Method to refresh the LCD at request. Normally after a change has been made.
 */
void Open_A3XX_FCU_LCD::refreshLCD(uint8_t address) {
  ht.write(address*2, buffer[address], 8);
}

/**
 * Method to clear the LCD and set it to black.
 */
void Open_A3XX_FCU_LCD::clearLCD() {
  for (uint8_t i = 0; i < ht.MAX_ADDR; i++)
      ht.write(i, 0);
  memset(buffer, 0, BUFFER_SIZE_MAX);
}

// ***********************
// **** SPEED METHODS ****
// ***********************
void Open_A3XX_FCU_LCD::setSpeedLabel(bool enabled) {
  SET_BUFF_BIT(SPECIALS, 7, enabled);
  refreshLCD(SPECIALS);
}
void Open_A3XX_FCU_LCD::setMachLabel(bool enabled) {
  SET_BUFF_BIT(SPECIALS, 6, enabled);
  SET_BUFF_BIT(SPD_TEN, 0, enabled); // Decimal-point
  refreshLCD(SPECIALS);
}
void Open_A3XX_FCU_LCD::setSpeedDot(int8_t state) {
  bool enabled;
  if (state == 0) enabled = false;
  else enabled = true;
  SET_BUFF_BIT(HDG_HUN, 0, enabled);
  refreshLCD(HDG_HUN);
}
void Open_A3XX_FCU_LCD::showSpeedValue(uint16_t value) {
  if (value > 999) value = 999;
  displayDigit(SPD_UNIT, (value % 10));
  value = value / 10;
  displayDigit(SPD_TEN, (value % 10));
  displayDigit(SPD_HUN, (value / 10)); 
}

// *************************
// **** HEADING METHODS ****
// *************************
void Open_A3XX_FCU_LCD::setHeadingLabel(bool enabled) {
  SET_BUFF_BIT(SPECIALS, 5, enabled);
  SET_BUFF_BIT(ALT_TEN, 0, enabled);
  refreshLCD(SPECIALS);
  refreshLCD(ALT_TEN);
}
void Open_A3XX_FCU_LCD::setTrackLabel(bool enabled) {
  SET_BUFF_BIT(SPECIALS, 4, enabled);
  SET_BUFF_BIT(ALT_HUN, 0, enabled);
  refreshLCD(SPECIALS);
  refreshLCD(ALT_HUN);
}
void Open_A3XX_FCU_LCD::setLatitudeLabel(bool enabled) {
  SET_BUFF_BIT(HDG_TEN, 0, enabled);
  refreshLCD(HDG_TEN);
}
void Open_A3XX_FCU_LCD::setHeadingDot(int8_t state) {
  bool enabled;
  if (state == 0) enabled = false;
  else enabled = true;
  SET_BUFF_BIT(HDG_UNIT, 0, enabled);
  refreshLCD(HDG_UNIT);
}
void Open_A3XX_FCU_LCD::showHeadingValue(uint16_t value) {
  if (value > 999) value = 999;
  displayDigit(HDG_UNIT, (value % 10));
  value = value / 10;
  displayDigit(HDG_TEN, (value % 10));
  displayDigit(HDG_HUN, (value / 10));
}

// **************************
// **** ALTITUDE METHODS ****
// **************************
void Open_A3XX_FCU_LCD::setAltitudeLabel(bool enabled) {
  SET_BUFF_BIT(SPECIALS, 0, enabled);
  refreshLCD(SPECIALS);
}
void Open_A3XX_FCU_LCD::setLvlChLabel(bool enabled) {
  SET_BUFF_BIT(SPECIALS, 1, enabled);
  refreshLCD(SPECIALS);
}
void Open_A3XX_FCU_LCD::setAltitudeDot(int8_t state) {
  bool enabled;
  if (state == 0) enabled = false;
  else enabled = true;
  SET_BUFF_BIT(VRT_THO, 0, enabled);
  refreshLCD(VRT_THO);
}
void Open_A3XX_FCU_LCD::showAltitudeValue(uint32_t value) {
  if (value > 99999) value = 99999;
  displayDigit(ALT_UNIT, (value % 10));
  value = value / 10;
  displayDigit(ALT_TEN, (value % 10));
  value = value / 10;
  displayDigit(ALT_HUN, (value % 10));
  value = value / 10;
  displayDigit(ALT_THO, (value % 10));
  displayDigit(ALT_TTH, (value / 10));
}

// ********************************
// **** VERTICAL SPEED METHODS ****
// ********************************
void Open_A3XX_FCU_LCD::setVrtSpdLabel(bool enabled) {
  SET_BUFF_BIT(SPECIALS, 2, enabled);
  SET_BUFF_BIT(ALT_THO, 0, enabled);
  refreshLCD(SPECIALS);
  refreshLCD(ALT_THO);
}
void Open_A3XX_FCU_LCD::setFPALabel(bool enabled) {
  SET_BUFF_BIT(SPECIALS, 3, enabled);
  SET_BUFF_BIT(ALT_TTH, 0, enabled);
  refreshLCD(SPECIALS);
  refreshLCD(ALT_TTH);
}
void Open_A3XX_FCU_LCD::setSignLabel(bool enabled) {
  vertSignEnabled = enabled;
}
void Open_A3XX_FCU_LCD::showVerticalValue(int16_t value) {
  if (value > 9999) value = 9999;
  if (value < -9999) value = -9999; 
  if (value < 0) {
    value = -value;
    SET_BUFF_BIT(VRT_TEN, 0, false);
    SET_BUFF_BIT(VRT_UNIT, 0, vertSignEnabled);
    SET_BUFF_BIT(VRT_HUN, 0, false);
  } else if (value == 0) {
    SET_BUFF_BIT(VRT_TEN, 0, false);
    SET_BUFF_BIT(VRT_UNIT, 0, false);
    SET_BUFF_BIT(VRT_HUN, 0, false);
  } else {
    SET_BUFF_BIT(VRT_TEN, 0, vertSignEnabled);
    SET_BUFF_BIT(VRT_UNIT, 0, vertSignEnabled);
    SET_BUFF_BIT(VRT_HUN, 0, false);
  }  
  displayDigit(VRT_UNIT, 12);
  value = value / 10;
  displayDigit(VRT_TEN, 12);
  value = value / 10;
  displayDigit(VRT_HUN, (value % 10));
  displayDigit(VRT_THO, (value / 10));
}
void Open_A3XX_FCU_LCD::showFPAValue(int8_t value) {
  if (value > 99) value = 99;
  if (value < -99) value = -99; 
  if (value < 0) {
    value = -value;
    SET_BUFF_BIT(VRT_TEN, 0, false);
    SET_BUFF_BIT(VRT_UNIT, 0, vertSignEnabled);
    SET_BUFF_BIT(VRT_HUN, 0, true);
  } else {
    SET_BUFF_BIT(VRT_TEN, 0, vertSignEnabled);
    SET_BUFF_BIT(VRT_UNIT, 0, vertSignEnabled);
    SET_BUFF_BIT(VRT_HUN, 0, true);
  }
  displayDigit(VRT_HUN, (value % 10));
  displayDigit(VRT_THO, (value / 10));
  SET_BUFF_BITS(VRT_TEN, 0b11111110, 0);
  SET_BUFF_BITS(VRT_UNIT, 0b11111110, 0);
  refreshLCD(VRT_TEN);
  refreshLCD(VRT_UNIT);
}

// ********************************
// **** PRESET & OTHER METHODS ****
// ********************************
void Open_A3XX_FCU_LCD::setSpeedDashes(int8_t state) {
  uint8_t val;
  bool enabled;
  if (state == 0) enabled = false;
  else enabled = true;
  if (enabled) {
    val = 10;
  } else {
    val = 11;
  }
  displayDigit(SPD_HUN, val);
  displayDigit(SPD_TEN, val);
  displayDigit(SPD_UNIT, val);
  SET_BUFF_BIT(SPD_TEN, 0, false); // Clear Mach Decimal-point
  refreshLCD(SPECIALS);    
}
void Open_A3XX_FCU_LCD::setHeadingDashes(int8_t state) {
  uint8_t val;
  bool enabled;
  if (state == 0) enabled = false;
  else enabled = true;
  if (enabled) {
    val = 10;
  } else {
    val = 11;
  }
  displayDigit(HDG_HUN, val);
  displayDigit(HDG_TEN, val);
  displayDigit(HDG_UNIT, val);
}
void Open_A3XX_FCU_LCD::setAltitudeDashes(int8_t state) {
  uint8_t val;
  bool enabled;
  if (state == 0) enabled = false;
  else enabled = true;
  if (enabled) {
    val = 10;
  } else {
    val = 11;
  }
  displayDigit(ALT_TTH, val);
  displayDigit(ALT_THO, val);
  displayDigit(ALT_HUN, val);
  displayDigit(ALT_TEN, val);
  displayDigit(ALT_UNIT, val);
}
void Open_A3XX_FCU_LCD::setVrtSpdDashes(int8_t state) {
  uint8_t val;
  bool enabled;
  if (state == 0) enabled = false;
  else enabled = true;
  if (enabled) {
    val = 10;
    SET_BUFF_BIT(VRT_UNIT, 0, true); // Set the plus/minus to minus
    SET_BUFF_BIT(VRT_TEN, 0, false);  // Remove the plus segment
  } else {
    val = 11; 
    SET_BUFF_BIT(VRT_UNIT, 0, false); // Turn it off
  }
  displayDigit(VRT_THO, val);
  displayDigit(VRT_HUN, val);
  displayDigit(VRT_TEN, val);
  displayDigit(VRT_UNIT, val);
}
void Open_A3XX_FCU_LCD::setStartLabels() {
  setAltitudeLabel(true);
  setLvlChLabel(true);
  setLatitudeLabel(true);
}
void Open_A3XX_FCU_LCD::toggleTrkHdgMode(int8_t state) {
  if (state == 0) setHeadingMode();
  else setTrackMode();
}
void Open_A3XX_FCU_LCD::setHeadingMode() {
  setHeadingLabel(true);
  setTrackLabel(false);
  setVrtSpdLabel(true);
  setFPALabel(false);
  trkActive = false;
}
void Open_A3XX_FCU_LCD::setTrackMode() {
  setHeadingLabel(false);
  setTrackLabel(true);
  setVrtSpdLabel(false);
  setFPALabel(true);
  trkActive = true;
}
void Open_A3XX_FCU_LCD::setSpeedMode(uint16_t value) {
  setSpeedLabel(true);
  setMachLabel(false);
  showSpeedValue(value);
}
void Open_A3XX_FCU_LCD::setMachMode(uint16_t value) {
  setSpeedLabel(false);
  setMachLabel(true);
  showSpeedValue(value);
}

// ************************************
// **** GLOBAL & UNIVERSAL METHODS ****
// ************************************

/**
 * Set up the binary patterns for each character required on the '7-Segment' sections
 */
uint8_t digitPattern[13] = {
  0b11111010, // 0
  0b01100000, // 1
  0b10111100, // 2
  0b11110100, // 3
  0b01100110, // 4
  0b11010110, // 5
  0b11011110, // 6
  0b01110000, // 7
  0b11111110, // 8
  0b11110110, // 9
  0b00000100, // -
  0b00000000, // blank
  0b11001100, // small 0 (For V/S)
};

/**
 * A method to display a digit/character
 * @variable address  The address of the '7-segment' to display the digit on
 * @variable digit    The digit to be displayed
 */
void Open_A3XX_FCU_LCD::displayDigit(uint8_t address, uint8_t digit) {
  // This ensures that anything over 12 is turned to 'blank', and as it's unsigned anything less than 0 will become 255, and therefore, 'blank'.
  if (digit > 12) digit = 11;
  buffer[address] = (buffer[address] & 1) | digitPattern[digit];  
  refreshLCD(address);
}

/**
 * A function to handle raw data from Mobiflight.
 * @variable *cmds The raw string sent from MobiFlight
 */
void Open_A3XX_FCU_LCD::handleMobiFlightRaw(char *cmds) {

  if (!_initialised)
    return;
  // Split string into individual commands
  char *cmd = strtok(cmds, ",");
  while(cmd) {
      handleMobiFlightCmd(cmd);
      cmd = strtok(NULL, ",");
  }
}

/**
 * A function to handle a single command passed from MobiFlight.
 * Commands should be passed to this method via the 'handleMobiFlightRaw' method.
 * @variable *cmd The command to be processed
 */
void Open_A3XX_FCU_LCD::handleMobiFlightCmd(char *cmd) {
  // Split the whole command into 'cmd' & 'data'. If no data, set data to '0'.
  // Example: "setSpd=180" -> 'cmd'   = "setSpd" 
  //                          'data'  = "180"
  char *p = strchr(cmd, '=');
  int32_t data;
  if (p) {
    *p = '\0';      // Convert '=' to end of string
    p++;            // Move 'p' to start of data string
    data = atoi(p); // Convert the string value of the data to Integer (assuming the data passed is always an Integer).
  } else {
    // No data - set to '0'
    data = 0;
  }
  // Send the data to the appropriate function
       if (strcmp(cmd, "setSpd")==0) setSpeedMode((uint16_t)data);
  else if (strcmp(cmd, "setMach")==0) setMachMode((uint16_t)data);     
  else if (strcmp(cmd, "setHdg")==0) showHeadingValue((uint16_t)data);
  else if (strcmp(cmd, "setAlt")==0) showAltitudeValue((uint32_t)data);
  else if (strcmp(cmd, "setVS")==0) showVerticalValue((int16_t)data);
  else if (strcmp(cmd, "setFPA")==0) showFPAValue((int8_t)data);
  else if (strcmp(cmd, "setSpdDash")==0) setSpeedDashes((int8_t)data);
  else if (strcmp(cmd, "setHdgDash")==0) setHeadingDashes((int8_t)data);
  else if (strcmp(cmd, "setAltDash")==0) setAltitudeDashes((int8_t)data);
  else if (strcmp(cmd, "setVSDash")==0) setVrtSpdDashes((int8_t)data);
  else if (strcmp(cmd, "setSpdDot")==0) setSpeedDot((int8_t)data);
  else if (strcmp(cmd, "setHdgDot")==0) setHeadingDot((int8_t)data);
  else if (strcmp(cmd, "setAltDot")==0) setAltitudeDot((int8_t)data);
  else if (strcmp(cmd, "toggleTrkHdg")==0) toggleTrkHdgMode((int8_t)data); 
}

// FBW LVars Site
// https://docs.flybywiresim.com/fbw-a32nx/feature-guides/autopilot-fbw/#speed