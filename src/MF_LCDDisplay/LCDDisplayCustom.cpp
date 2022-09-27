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
    Open_A3XX_FCU_LCD  *lcd_SPI[MAX_MFLCD_SPI];
    Open_A3XX_EFIS_LCD *lcd_SPI2[MAX_MFLCD_SPI];
    // MFLCDDisplay *lcd_SPI[MAX_MFLCD_SPI];
    uint8_t lcd_SPIRegistered = 0;

    void Add(uint8_t TYPE)
    {
        if (TYPE == 1) {
            AddFCU(); // If no parameters, default values from .h file will be used
        }
        if (TYPE == 2) {
            AddEFIS(); // If no parameters, default values from .h file will be used
        }
    }

    void AddFCU(uint8_t CS, uint8_t CLK, uint8_t DATA)
    {
        if (lcd_SPIRegistered == MAX_MFLCD_SPI)
            return;

        if (!FitInMemory(sizeof(Open_A3XX_FCU_LCD))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
            return;
        }
        lcd_SPI[lcd_SPIRegistered] = new Open_A3XX_FCU_LCD(CS, CLK, DATA);      // Pass the values to the constructor for the HT1621 chip to be setup
        lcd_SPI[lcd_SPIRegistered]->attach(CS, CLK, DATA);                      // Pass the values to the instance to set up the global variables and initialise the LCD
        lcd_SPIRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
    }
    void AddEFIS(uint8_t CS, uint8_t CLK, uint8_t DATA)
    {
        if (lcd_SPIRegistered == MAX_MFLCD_SPI)
            return;

        if (!FitInMemory(sizeof(Open_A3XX_EFIS_LCD))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
            return;
        }
        lcd_SPI2[lcd_SPIRegistered] = new Open_A3XX_EFIS_LCD(CS, CLK, DATA);    // Pass the values to the constructor for the HT1621 chip to be setup
        lcd_SPI2[lcd_SPIRegistered]->attach(CS, CLK, DATA);                     // Pass the values to the instance to set up the global variables and initialise the LCD
        lcd_SPIRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
    }

    void Clear()
    {
        for (int i = 0; i < lcd_SPIRegistered; i++) {
            if(lcd_SPI[i]) lcd_SPI[i]->detach();
            if(lcd_SPI2[i]) lcd_SPI2[i]->detach();
        }
        lcd_SPIRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Cleared lcdDisplays"));
#endif
    }

    void OnSet()
    {
        int   address = cmdMessenger.readInt16Arg();    // Not needed for the current implementation of the LCD
        (void) (address);                               // But we have to read it, so here we declare it void.

        char *output  = cmdMessenger.readStringArg();   // Read the command from MobiFlight
        char output2[32]={0};                           // Create a new peice of memory bigger than the size of `output`
        strncpy(output2, output, 31);                   // Create a unique copy of `output` in a new peice of memory

        lcd_SPI[0]->handleMobiFlightRaw(output);        // Pass the command to the FCU
        lcd_SPI2[1]->handleMobiFlightRaw(output2);      // Pass the command to the EFIS
        setLastCommandMillis();
    }
} // end of namespace LCDDisplayCustom
