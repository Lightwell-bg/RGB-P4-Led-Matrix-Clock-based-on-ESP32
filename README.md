# RGB P4 Led Matrix Clock  based on ESP32
“RGB P4 Led Matrix Clock  based on ESP32” is a multifunctional clock with functions of displaying weather, weather forecast, receiving news feed, displaying any given information at the appointed time. Management is carried out via a web interface or a mobile application. The watch has low power consumption and at the same time provides a high display thanks to the use of RGB matrix panel P4 (62x32).

------------
### What is a RGB P4 Led Matrix Clock  based on ESP32?
**RGB P4 Led Matrix Clock** is a device, based on ESP32 (Mini 32 or 30P WROOM-32) microcontroller is used to output the information received RGB matrix panel P4 256*128mm (62x32) 1/16 Scan.

An Internet connection via Wi-Fi module helps to synchronize time with NTP server, to receive and display weather and weather forecast, weather icons, today's date in various formats, news, exchange rates, etc. Clock can display predetermined information (reminders, greetings, advertisements, time-schedules etc.) according on schedule.
RGB matrix panel P4 allows you to display various **colorful graphics** and **animations**.

Clock adjustment and manipulation is performed via WEB-interface or APP [LWDevices](https://play.google.com/store/apps/details?id=eu.ledlightwell.lwdevices "LWDevices") or [LWDevices2](https://play.google.com/store/apps/details?id=eu.ledlightwell.lwdevices2 "LWDevices2").

To control RGB matrix panel P4 (62x32) the [SmartMatrix by Pixelmatix](https://github.com/pixelmatix/SmartMatrix/tree/teensylc "SmartMatrix by Pixelmatix") library is used.

To power the RGB matrix panel P4 and the controller, you must use any power supply with a voltage of 5V and a power of more than 15W, since the consumption of one panel when displaying white is about 3 A.

### Features include:
- Always precise time because of synchronization with NTP time server
- High display brightness 
- Getting a weather information and forecast for your city (temperature, barometric pressure, humidity, cloud and wind structure) 
- Output of any predetermined information in the form of text, creeping line, graphic icons
- No buttons – control the clock via WEB-interface from PC, tablet or smartphone
- Brightness and output speed adjustment
- Firmware update via Wi-Fi
- Information display on 3 languages: English, Russian, Bulgarian
- An option to install different alternative Open Source firmware, designed to RGB matrix panel control
- Hackable: Open source hardware and firmware, Displays information read via UART

![](https://github.com/Lightwell-bg/RGB-P4-Led-Matrix-Clock-based-on-ESP32/blob/main/images/P4LMClock02-1.jpg)

### Required Parts:
- WeMos MiniID1 ESP-32 Wireless WiFi Bluetooth Module [http://alii.pub/4d0l9o](http://alii.pub/4d0l9o "http://alii.pub/4d0l9o") 
- P4 LED screen panel module 256*128mm 64*32 pixels 1/16 Scan Indoor [http://alii.pub/5zupa1](http://alii.pub/5zupa1 "http://alii.pub/5zupa1")
- 10uF 25V Capacitor
- 16 pin male header (2x8)
- Power supply 5V 8A - $11 from Aliexpress [http://got.by/4d0ngf](http://got.by/4d0ngf "http://got.by/4d0ngf")

**Note:** Using the links provided here help to support these types of projects. Thank you for the support.

You can purchase a PCB with soldered elements and connectors for connecting the indicator and sensors, or just a PCB and a set of elements for installation [RGB Matrix Panel Shield for ESP32](https://diy-led.eu/product/rgb-matrix-panel-shield-for-esp32/ "RGB Matrix Panel Shield for ESP32")

### Schematics / Assembly
| 16 pin male header  |   | WeMos MiniID1 ESP-32  |  Comment |
| ------------ | ------------ | ------------ | ------------ |
| 1  | R0  | IO2  | Red Data  |
| 2  | G0  |  IO15 (TDO) | Green Data   |
| 3  |  B0 |  IO4 | Blue Data  |
| 4  | GND  |GND   | Ground  |
| 5  |  R1 |  IO16/Rx2 | Red Data  |
|  6 | G1  | IO27  | Green Data   |
|  7 | B1  | IO17/Tx2  | Blue Data  |
| 8  | E  | IO12 (TDI)  | Demux Input E for 64x64 panels  |
|9   | A  |  IO5 | Demux Input A0  |
|10   | B  | IO18  | Demux Input A1  |
| 11  | C  |  IO19 | Demux Input A2  |
|  12 | D  | IO21  | Demux Input E1, E3 (32x32 panels only)  |
|  13 | CLK  |IO22   | LED Drivers' Clock  |
|  14 | STB  | IO26  | LED Drivers' Latch  |
|  15 | OE  |  IO25 | LED Drivers' Output Enable  |
|  16 | GND  |  GND | Ground  |

1. Solder the elements on the printed circuit board according to the circuit diagram, instructions and markings placed on the printed circuit board.
2. Connect the WeMos MiniID1 ESP-32, 16 pin male header using soldering or connectors.
3. Insert the ribbon cable that came with the display into the "Pout" connector on the PCB, insert the other end into the "Output" connector of the panel.
4. Plug the power wire that came with the display into the power pins of the panel. Attach the power wires to the 2Pin screw terminals on the shield (Pay close attention to the polarity!)
5. To download the sketch and files connect the device to the computer via USB cable.

### Loading Supporting Library Files in Arduino
Use the Arduino guide for details on how to installing and manage libraries [https://www.arduino.cc/en/Guide/Libraries](https://www.arduino.cc/en/Guide/Libraries "https://www.arduino.cc/en/Guide/Libraries") 

The following packages and libraries are used (download and install):
- ArduinoJson by Benoit Blanchon [https://github.com/bblanchon/ArduinoJson.git](https://github.com/bblanchon/ArduinoJson.git "https://github.com/bblanchon/ArduinoJson.git")
- TimeLib by Michael Margolis [https://github.com/PaulStoffregen/Time](https://github.com/PaulStoffregen/Time "https://github.com/PaulStoffregen/Time") 
- Timezone [https://github.com/JChristensen/Timezone](https://github.com/JChristensen/Timezone "https://github.com/JChristensen/Timezone")
- RTClib by Adafruit [https://github.com/adafruit/RTClib](https://github.com/adafruit/RTClib "https://github.com/adafruit/RTClib") 
- SmartMatrix by Pixelmatix [https://github.com/pixelmatix/SmartMatrix/tree/teensylc](https://github.com/pixelmatix/SmartMatrix/tree/teensylc "https://github.com/pixelmatix/SmartMatrix/tree/teensylc")
- FastLED  [https://github.com/FastLED/FastLED](https://github.com/FastLED/FastLED "https://github.com/FastLED/FastLED")
- Adafruit GFX Library [https://github.com/adafruit/Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library "https://github.com/adafruit/Adafruit-GFX-Library")
- ESP32SSDP [https://github.com/luc-github/ESP32SSDP](https://github.com/luc-github/ESP32SSDP "https://github.com/luc-github/ESP32SSDP")
- NetCrtESP by Lightwell [https://github.com/Lightwell-bg/NetCrtESP](https://github.com/Lightwell-bg/NetCrtESP "https://github.com/Lightwell-bg/NetCrtESP")
- ESPTimeFunc by Lightwell [https://github.com/Lightwell-bg/ESPTimeFunc](https://github.com/Lightwell-bg/ESPTimeFunc "https://github.com/Lightwell-bg/ESPTimeFunc")
- weatherAP by Lightwell [https://github.com/Lightwell-bg/weatherAp](https://github.com/Lightwell-bg/weatherAp "https://github.com/Lightwell-bg/weatherAp")

**Note:** There could be some issues if using different versions of the libraries.

### Initial Configuration
Editing the Config.h file is optional. SSID and password for Wi-Fi, all API Keys are managed in the Web Interface. It is not required to edit the Config.h file before loading and running the code.
Open Weather Map free API key: [http://openweathermap.org/](http://openweathermap.org/ "http://openweathermap.org/")  -- this is used to get weather data.
**NOTE:** The settings in the Config.h are the default settings for the first loading. After loading you can change the settings via the Web Interface or APP. If you want to change settings again in the Config.h, you will need to erase the file system on the Wemos or use the “Reset Settings” option in the Web Interface.

During the first time run MiniID1 ESP-32 will try to connect to your Wi-Fi and fail (because it doesn't have any previously saved Wi-Fi credentials). This will cause it to start up an access point, which serves up a captive configuration portal.
You can connect to this access point to select and enter credentials for your network. Save configuration and restart device. (MiniID1 ESP-32 will restart and connect to your Wi-Fi network).

### Web Interface
All clock management is carried out using a web browser.  On first power-up, RGB P4 Led Matrix Clock will serve a hotspot called **LWMC01**. For clock adjustment and manipulation open a webpage with the **192.168.4.1** address. Within this hotspot you can manually override the time and date, set text messages for display, set timecodes of messages output and update software.

For Wi-Fi network connection please input its SSID and password.  After successful connection P4 Led Matrix Clock will begin to receive and transmit information over the Internet. NTP servers are used to get accurate time. To get the current weather and forecast use the service [https://openweathermap.org](https://openweathermap.org "https://openweathermap.org") (you need to register and get an API key).

![](https://github.com/Lightwell-bg/RGB-P4-Led-Matrix-Clock-based-on-ESP32/blob/main/images/index.png)
![](https://github.com/Lightwell-bg/RGB-P4-Led-Matrix-Clock-based-on-ESP32/blob/main/images/P4LMClock01-1.jpg) ![](https://github.com/Lightwell-bg/RGB-P4-Led-Matrix-Clock-based-on-ESP32/blob/main/images/P4LMClock03-1.jpg)
![](https://github.com/Lightwell-bg/RGB-P4-Led-Matrix-Clock-based-on-ESP32/blob/main/images/wifi.png)
![](https://github.com/Lightwell-bg/RGB-P4-Led-Matrix-Clock-based-on-ESP32/blob/main/images/time.png)
![](https://github.com/Lightwell-bg/RGB-P4-Led-Matrix-Clock-based-on-ESP32/blob/main/images/setup.png)
![](https://github.com/Lightwell-bg/RGB-P4-Led-Matrix-Clock-based-on-ESP32/blob/main/images/help.png)

