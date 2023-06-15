#include <Arduino.h>
#include "allocateMem.h"
#include "commandmessenger.h"
#include "LCDDisplayCustom.h"
#include "MFBoards.h"
#include "KAV_A3XX_FCU_LCD.h"
#include "KAV_A3XX_EFIS_LCD.h"
namespace LCDDisplayCustom
{
    KAV_A3XX_FCU_LCD  *lcd_FCU[MAX_MFLCD_SPI];
    KAV_A3XX_EFIS_LCD *lcd_EFIS[MAX_MFLCD_SPI];
    uint8_t lcds_Registered = 0;

    void Add(uint8_t TYPE)
    {
        if (TYPE == 1) {
            AddFCU(); // If no parameters, default values from LCDDisplayCustom.h file will be used
        }
        if (TYPE == 2) {
            AddEFIS(); // If no parameters, default values from LCDDisplayCustom.h file will be used
        }
        if (TYPE == 3) {
            AddEFIS(35, 36, 37); // If no parameters, default values from LCDDisplayCustom.h file will be used
        }
    }

    void AddFCU(uint8_t CS, uint8_t CLK, uint8_t DATA)
    {
        if (lcds_Registered == MAX_MFLCD_SPI)
            return;

        if (!FitInMemory(sizeof(KAV_A3XX_FCU_LCD))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
            return;
        }
        lcd_FCU[lcds_Registered] = new KAV_A3XX_FCU_LCD(CS, CLK, DATA);      // Pass the values to the constructor for the HT1621 chip to be setup
        lcd_FCU[lcds_Registered]->attach(CS, CLK, DATA);                      // Pass the values to the instance to set up the global variables and initialise the LCD
        lcds_Registered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added FCU Display"));
#endif
    }
    void AddEFIS(uint8_t CS, uint8_t CLK, uint8_t DATA)
    {
        if (lcds_Registered == MAX_MFLCD_SPI)
            return;

        if (!FitInMemory(sizeof(KAV_A3XX_EFIS_LCD))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("LCD does not fit in Memory!"));
            return;
        }
        lcd_EFIS[lcds_Registered] = new KAV_A3XX_EFIS_LCD(CS, CLK, DATA);    // Pass the values to the constructor for the HT1621 chip to be setup
        lcd_EFIS[lcds_Registered]->attach(CS, CLK, DATA);                     // Pass the values to the instance to set up the global variables and initialise the LCD
        lcds_Registered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added EFIS 1 Display"));
#endif
    }

    void Clear()
    {
        for (int i = 0; i < lcds_Registered; i++) {
            if(lcd_FCU[i]) lcd_FCU[i]->detach();
            if(lcd_EFIS[i]) lcd_EFIS[i]->detach();
        }
        lcds_Registered = 0;
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

        lcd_FCU[0]->handleMobiFlightRaw(output);        // Pass the command to the FCU
        if (address == 1)                               // EFIS 1 must be the second device in the config
        {
            lcd_EFIS[1]->handleMobiFlightRaw(output2);      // Pass the command to the EFIS 1
        }
        if (address == 2)                               // EFIS 2 must be the third device in the config
        {
            lcd_EFIS[2]->handleMobiFlightRaw(output3);      // Pass the command to the EFIS 2
        }
        setLastCommandMillis();
    }
} // end of namespace LCDDisplayCustom
