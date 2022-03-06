#include <Arduino.h>
//#include "MFLCDDisplay.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include "LCDDisplayCustom.h"
#include "MFBoards.h"
#include "Open_A3XX_FCU_LCD.h"
namespace LCDDisplayCustom
{
Open_A3XX_FCU_LCD *lcd_SPI[MAX_MFLCD_SPI];
uint8_t cust_LCDs_Registered = 0;


void Add(uint8_t CS = 13, uint8_t CLK = 12, uint8_t DATA = 8, uint8_t B_LIGHT = 10)
{
  if (cust_LCDs_Registered == MAX_MFLCD_SPI)
    return;

  if (!FitInMemory(sizeof(Open_A3XX_FCU_LCD)))
	{
		// Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("Custom LCD does not fit in Memory!"));
		return;
	}
  lcd_SPI[cust_LCDs_Registered] = new Open_A3XX_FCU_LCD(CS, CLK, DATA, B_LIGHT);
  lcd_SPI[cust_LCDs_Registered]->attach(CS, CLK, DATA, B_LIGHT);
  cust_LCDs_Registered++;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
}

void Clear()
{
  for (int i = 0; i != cust_LCDs_Registered; i++)
  {
    lcd_SPI[i]->detach();
  }
  cust_LCDs_Registered = 0;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Cleared lcdDisplays"));
#endif
}

void OnSet()
{
  int address = cmdMessenger.readInt16Arg();
  char *output = cmdMessenger.readStringArg();

  lcd_SPI[0]->handleMobiFlightRaw(output); // This needs to be set to lcd_SPI[address]->... for release.
  setLastCommandMillis();
}
}   // end of namespace LCDDisplayCustom
