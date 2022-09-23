#include <Arduino.h>
//#include "MFLCDDisplay.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include "LCDDisplayCustom.h"
#include "MFBoards.h"
#include "Open_A3XX_FCU_LCD.h"
#include "Open_A3XX_EFIS_LCD.h"
namespace LCDDisplayCustom
{
Open_A3XX_FCU_LCD *lcd_SPI[MAX_MFLCD_SPI];
Open_A3XX_EFIS_LCD *lcd_SPI2[MAX_MFLCD_SPI];
//MFLCDDisplay *lcd_SPI[MAX_MFLCD_SPI];
uint8_t lcd_SPIRegistered = 0;

void Add(uint8_t CS) {
  if (CS == 1) {
    AddFCU();
  }
  if (CS == 2) {
    AddEFIS();
  }
}

void AddFCU(uint8_t CS = 13, uint8_t CLK = 12, uint8_t DATA = 8)
{
  if (lcd_SPIRegistered == MAX_MFLCD_SPI)
    return;

  if (!FitInMemory(sizeof(Open_A3XX_FCU_LCD)))
	{
		// Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
		return;
	}
  lcd_SPI[lcd_SPIRegistered] = new Open_A3XX_FCU_LCD();
  lcd_SPI[lcd_SPIRegistered]->attach(CS, CLK, DATA);
  lcd_SPIRegistered++;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
}
void AddEFIS(uint8_t CS = 2, uint8_t CLK = 3, uint8_t DATA = 4)
{
  if (lcd_SPIRegistered == MAX_MFLCD_SPI)
    return;

  if (!FitInMemory(sizeof(Open_A3XX_EFIS_LCD)))
	{
		// Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
		return;
	}
  lcd_SPI2[lcd_SPIRegistered] = new Open_A3XX_EFIS_LCD();
  lcd_SPI2[lcd_SPIRegistered]->attach(CS, CLK, DATA);
  lcd_SPIRegistered++;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
}

void Clear()
{
  for (int i = 0; i != lcd_SPIRegistered; i++)
  {
    lcd_SPI[i]->detach();
  }
  lcd_SPIRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Cleared lcdDisplays"));
#endif
}

void OnSet()
{
  int address = cmdMessenger.readInt16Arg();
  char *output = cmdMessenger.readStringArg();

  lcd_SPI[0]->handleMobiFlightRaw(output);
  lcd_SPI2[1]->handleMobiFlightRaw(output);
  setLastCommandMillis();
}
}   // end of namespace LCDDisplayCustom
