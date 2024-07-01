## ESP32-WROOM-32 CH340

Install additional drivers:

 - For Windows: http://www.wch-ic.com/downloads/CH341SER_ZIP.html
 - For MacOS: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads

In Arduino IDE add custom bords source: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Connect as **DOIT ESP32 DEVKIT V1**

=======

To connect ST7789 w/o CS pin need to modify TFT_eSPI lib:

In `../libraries/TFT_eSPI/User_Setups/Setup24_ST7789.h`:

uncomment this:
```
// Generic ESP32 setup
#define TFT_MISO -1
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS -1 // Not connected
#define TFT_DC 2
#define TFT_RST 4 // Connect reset to ensure display initialises
```
comment this:
```
// For NodeMCU - use pin numbers in the form PIN_Dx where Dx is the NodeMCU pin
// designation
// #define TFT_CS   -1      // Define as not used
// #define TFT_DC   PIN_D1  // Data Command control pin
// #define TFT_RST  PIN_D4  // TFT reset pin (could connect to NodeMCU RST, see
// next line) #define TFT_RST  -1    // TFT reset pin connect to NodeMCU RST,
// must also then add 10K pull down to TFT SCK
```

In `../libraries/TFT_eSPI/User_Setup.h`:

Replace 
```
// #include <User_Setup.h>           // Default setup is root library folder
```
with 
```
#include <User_Setups/Setup24_ST7789.h> // Setup file for DSTIKE/ESP32/ESP8266 configured for ST7789 240 x 240
```
