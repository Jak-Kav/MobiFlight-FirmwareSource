# Jak Kav Custom Firmware   
This is my custom firware for the MobiFlight modules.   
It allows the user to use my custom LCD's for the FCU and EFIS.   
<b>There is a pre-built HEX file hardcoded with the pins detailed below</b>
Current release is inline with MobiFlight V2.4, and you should use the file named `mobiflight_mega_2_4_0_Custom (1FCU_2EFIS).hex`, regardless of whether you're using just 1 FCU, 1 FCU and 1 EFIS or 1 FCU and 2 EFIS.    

You need to put the `arduino_mega_kav.board.json` in your MobiFlight connector directory manually.    
`C:\...AppData\Local\MobiFlight\MobiFlight Connector\Boards\` 

It should be setup as a normal `LCD DISPLAY` from the 'Add Device' menu in the MobiFlight Modules settings page.    
![Device Choice](https://cdn.shopify.com/s/files/1/0736/3588/9464/files/Capture2.png?v=1686832847)    
It will send a string as it's LCD output, and that will be converted to a method and value in the custom LCD library.   
To use the precompiled HEX version, please connect to the following arduino mega pins:   
<b>FCU</b>    
CS = 12   
CLK = 11   
DATA = 8   
Backlight = +5V   

<b>EFIS LEFT</b>    
CS = 32   
CLK = 33   
DATA = 34   
Backlight = +5V   

<b>EFIS RIGHT</b>    
CS = 35   
CLK = 36   
DATA = 37   
Backlight = +5V   

If you wish to compile your own version with different pin numbers, you should change the values in `LCDDisplayCustom.cpp` in the `Add` function.    
We have only compiled and tested for the Arduino Mega 2560 board.

In `MFBoards.h`, we have created this as a new board type, and this should be reflected in the MobiFlight Connector when setting up.    
The devices should always be in this order, otherwise the data will be sent to the wrong device. <b>This is critical!</b>    
![New Board Type](https://cdn.shopify.com/s/files/1/0736/3588/9464/files/Capture1.jpg?v=1686832848)       

The original readme is below:
- - -

# MobiFlight-FirmwareSource

PlatformIO version of the MobiFlight firmware source.

To build:

1. Install [Visual Studio Code](https://code.visualstudio.com/Download)
2. Install the [PlatformIO extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide). Visual Studio Code will automatically
   suggest this extension if you don't already have it installed.
3. From the Command Palette select `PlatformIO: Build` to build or `PlatformIO: Upload` to build and upload to your connected board

If you want to speed up local development and only build for one of the supported platforms then click on the `Default (MobiFlight-FirmwareSource)` label
in the Visual Studio Code status bar and then select the specific target platform you want to build for.

## Publishing a release

To publish a new release:

1. Go to the [GitHub Releases page](https://github.com/MobiFlight/MobiFlight-FirmwareSource/releases) and
click the `Draft a new release` button.
2. Click `Choose a tag` and create a new version tag, e.g. `1.14.0`
3. Enter a title and release notes.
4. Optionally check `This is a pre-release` if the release should be for testing purposes and not be marked
as the latest published release.
5. Click `Publish release`.

The release build process will automatically run and after a few minutes attach firmware binaries
for the release to the release page.

If a release was marked as pre-release you can go back later and edit it to remove the pre-release designation
which will automatically promote it to the latest published release.
