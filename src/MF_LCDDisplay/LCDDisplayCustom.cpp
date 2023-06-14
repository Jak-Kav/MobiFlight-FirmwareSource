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
    Open_A3XX_EFIS_LCD *lcd_SPI3[MAX_MFLCD_SPI];
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
        if (TYPE == 3) {
            AddEFIS2(); // If no parameters, default values from .h file will be used
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
        cmdMessenger.sendCmd(kDebug, F("Added FCU Display"));
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
        cmdMessenger.sendCmd(kDebug, F("Added EFIS 1 Display"));
#endif
    }
    void AddEFIS2(uint8_t CS, uint8_t CLK, uint8_t DATA)
    {
        if (lcd_SPIRegistered == MAX_MFLCD_SPI)
            return;

        if (!FitInMemory(sizeof(Open_A3XX_EFIS_LCD))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
            return;
        }
        lcd_SPI3[lcd_SPIRegistered] = new Open_A3XX_EFIS_LCD(CS, CLK, DATA);    // Pass the values to the constructor for the HT1621 chip to be setup
        lcd_SPI3[lcd_SPIRegistered]->attach(CS, CLK, DATA);                     // Pass the values to the instance to set up the global variables and initialise the LCD
        lcd_SPIRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added EFIS 2 Display"));
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
        int   address = cmdMessenger.readInt16Arg();    // This is needed to determine which device to send the command to
                                                        // Device 1 must be the FCU, Device 2 must be EFIS 1, Device 3 must be EFIS 2
        
        char *output  = cmdMessenger.readStringArg();   // Read the command from MobiFlight
        char output2[32]={0};                           // Create a new peice of memory bigger than the size of `output` for EFIS 1
        char output3[32]={0};                           // Create a new peice of memory bigger than the size of `output` for EFIS 2
        strncpy(output2, output, 31);                   // Create a unique copy of `output` in a new peice of memory for EFIS 1
        strncpy(output3, output, 31);                   // Create a unique copy of `output` in a new peice of memory for EFIS 2

        lcd_SPI[0]->handleMobiFlightRaw(output);        // Pass the command to the FCU
        if (address == 1)                               // EFIS 1 must be the second device in the config
        {
            lcd_SPI2[1]->handleMobiFlightRaw(output2);      // Pass the command to the EFIS 1
        }
        if (address == 2)                               // EFIS 2 must be the third device in the config
        {
            lcd_SPI3[2]->handleMobiFlightRaw(output3);      // Pass the command to the EFIS 2
        }
        setLastCommandMillis();
    }
} // end of namespace LCDDisplayCustom
