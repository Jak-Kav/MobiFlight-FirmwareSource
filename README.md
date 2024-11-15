# IMPORTANT! NEW REPOSITORY!
We are now transitioning over to a new repository: [https://github.com/Kav-Simulations](https://github.com/Kav-Simulations)     
This is to accomdate our new natural integration with MobiFlight, and there is no longer any need for you to install our custom firmware or configuration files outside of MobiFlight.

All of our devices and firmware are now included in MobiFlight, and so the only need for our GitHub repository access is to contribute to development. If you are an end user, and don't need to contribute code, then you no longer need to follow our original installation guides.     
Please visit our YouTube [www.youtube.com/@kavsimulations5855](https://www.youtube.com/@kavsimulations5855/) for the latest video tutorial on getting setup with our devices.

</br>
<i>The original firmware will still be available for those that wish to continue using it, but it will no longer be supported or updated going forward. You can see the details below:</i>

- - - - - - - - - -

# Jak Kav Custom Firmware   
This is my custom firware for the MobiFlight modules.   
It allows the user to use my custom LCD's for the FCU and EFIS.   
<b>There is a pre-built HEX file hardcoded with the pins detailed below</b>    
Current release is inline with MobiFlight V2.4.1, and you should use the file named `mobiflight_mega_kav_2_4_1.hex`, regardless of whether you're using just '1 FCU', '1 FCU and 1 EFIS' or '1 FCU and 2 EFIS'.    

You need to put the `arduino_mega_kav.board.json` in your MobiFlight connector directory manually.    
`C:\...AppData\Local\MobiFlight\MobiFlight Connector\Boards\`     
You should then place the `mobiflight_mega_kav_2_4_1.hex` file in the `C:...\AppData\Local\MobiFlight\MobiFlight Connector\firmware\` direcctory.    
You can now simply click the `Update Firmware` button and ensure that the device shows `Firmware 2.4.1` and the module type and name is `MobiFlight Mega Kav`.    
![Firmware Update](https://cdn.shopify.com/s/files/1/0736/3588/9464/files/Capture4.jpg?v=1686923609)  

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
