#ifndef GLOBAL_H
#define GLOBAL_H

const String nName = "LWClockESP32_"; 
const String nVersion = "v2.0";
#define USE_RTC false //USE RTC chip DS3231 
#define USE_DHT false //USE sensor DHT
#define USE_DS18B20 false //USE sensor DS18B20
#define USE_OC_MODE // OPEN/CLOSE mode
//#define TEST_YOUTUBE
#define USE_ASINCWS true //use ESPAsyncWebServer instead standart

#include <WiFi.h>   
#if USE_ASINCWS == true
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include "ssdpAWS.h"
#else
  #include <WebServer.h> 
  #include <ESP32SSDP.h>
#endif
#include <NetBIOS.h>
#include <HTTPClient.h>   
#include <Update.h>//for update firmware and SPIFFS
#include <WiFiUdp.h>
#include <DNSServer.h> // Captive_Portal
#include <ESPmDNS.h> // Captive_Portal
//#include <LittleFS.h>
#include "SPIFFS.h"
#include "StreamString.h" //need FOR OUTPUT ERROR when update firmware
#include <ArduinoJson.h>        //https://github.com/bblanchon/ArduinoJson.git 
#include <TimeLib.h>            //https://github.com/PaulStoffregen/Time Michael Margolis
#include <Timezone.h>           //https://github.com/JChristensen/Timezone
#include "NetCrtESP.h"
#include "ESPTimeFunc.h"
#include "weatherAp.h"
#include "newsAp.h"
#define USE_ADAFRUIT_GFX_LAYERS
#include <MatrixHardware_ESP32_V0.h>                // This file contains multiple ESP32 hardware configurations, edit the file to define GPIOPINOUT (or add #define GPIOPINOUT with a hardcoded number before this #include)
#include <SmartMatrix.h>
#if USE_DHT == true
  #include <Adafruit_Sensor.h>                           // https://github.com/adafruit/Adafruit_Sensor
  #include "DHT.h"                                      //https://github.com/adafruit/DHT-sensor-library
  //#define DHTTYPE DHT21   // DHT 21 (AM2301)
  #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
  #define TEMPHUM_PIN 14    // D5 
  DHT dht(TEMPHUM_PIN, DHTTYPE);
#endif //DHT

//For dispalay name of device
#if USE_RTC == true
  const String rtsN = " +RTC";
#else  
  const String rtsN = " no RTC";
#endif
#if USE_DHT == true
  const String senN =  " DHT22";
#elif USE_DS18B20 == true 
  const String senN = " DS18B20";
#else 
  const String senN = " no sensor";  
#endif //BME280 
String cVersion = nName + nVersion + rtsN + senN;

#if USE_ASINCWS == true
  AsyncWebServer HTTP(80);
  ssdpAWS mySSDP(&HTTP);
#else
  WebServer HTTP; 
#endif 
const uint8_t DNS_PORT = 53;
DNSServer dnsServer;
File fsUploadFile; 
WiFiClient ESPclient;
//MDNSResponder mdns;        //назначаем экземпляр класса ESP8266mDNS
//AsyncUDP Udp;
WiFiUDP Udp;
NetCrtESP myWIFI;
ESPTimeFunc myESPTime(false);
weatherAP myWeather(&ESPclient);
HTTPClient ESPhttp;
newsAp myNews(&ESPclient);
//newsAp myNews(&ESPhttp);

bool testYOUTUBE = false; //ON/OFF disp fake stat YT /testyoutube?test=1&testper=30000 (0)
uint16_t testper = 30000;
String filePath = "/myconfig.json"; //File for config
String jsonConfig = "{}";
// wifi
String ssid = "lightwell";
String password = "0877447225";
String ssidAP = "LWMC01";   // SSID AP точки доступа
String passwordAP = ""; // пароль точки доступа
String SSDP_Name = "LWMC01"; // Имя SSDP
//IPAddress apIP(192, 168, 4, 1);
String apIP = "192.168.4.1";
//UDP
uint16_t localUdpPort = 4210;
char incomingPacket[255];
//char replyPacket[] = "Hi there! Got the message :-)";

uint8_t lang = 2; //0-RU, 1-BG, 2 -EN, 
//Time
uint8_t typeSync = 1; //type of synchronuzation 0 - manual, 1 - NTP, 2 - GPS, 
int8_t timezone = 2;               // часовой пояс GTM
bool isDayLightSaving = true; //Summer time use
String sNtpServerName = "us.pool.ntp.org";
const char sNtpServerName2[] = "pool.ntp.org";
const char sNtpServerName3[] = "time.nist.gov";
TimeChangeRule localCEST = {"CEST", Last, Sun, Mar, 2, 180};     // LOCAL Summer Time
TimeChangeRule localCET = {"CET ", Last, Sun, Oct, 3, 120};       // LOCAL Standard Time
Timezone localCE(localCEST, localCET);
const uint32_t PERIOD_TIME_SEND = 1000*60*90;

uint8_t modeDisplay = 2; //
String startLogo = nName + nVersion + " ";

//Setup for LED
uint8_t fontUsed = 0; //fonts
bool brauto = false;
uint8_t dmodefrom = 8, dmodeto = 20; //DAY MODE
uint8_t brightd = 230, brightn = 100; //brightness day and night
uint16_t speedTicker = 250; // speed of creeping line
float global_start = 0, global_stop = 24; //Working time
bool isBigClock = false;
float clockBigFrom = 0; float clockBigTo = 24;
bool showSec = false;
//ZONE 3
uint16_t dispStatZ3 =             0b0000000001011111;
const uint16_t isLedDateZ3 =      0b0000000000000001;
const uint16_t isTxtOn0 =         0b0000000000000010;
const uint16_t isTxtOn1 =         0b0000000000000100;
const uint16_t isTxtOn2 =         0b0000000000001000;
const uint16_t isTxtOn3 =         0b0000000000010000;
const uint16_t isNews   =         0b0000000000100000;
const uint16_t isLedWeatherZ3 =   0b0000000001000000;
const uint16_t isLedForecastZ3 =  0b0000000010000000;

float newsFromZ3 = 0; float newsToZ3 = 24;
float dateFromZ3 = 0; float dateToZ3 = 24; float weathFromZ3 = 0; float weathToZ3 = 24; float fcastFromZ3 = 0; float fcastToZ3 = 24;
bool isCrLineDateZ3 = 1;
String strText[4] = {"Lighgwell", "Screen", "ЯЗЫК", ""};
bool isCrLine[4] = {1, 0, 0, 1}; 
float txtFrom[4] = {0.3, 1.3, 0, 1}; float txtTo[4] = {24, 24, 23.3, 22};
uint8_t colorZ3R = 0x40, colorZ3G = 0xff, colorZ3B = 0x00;
rgb24 colorZ3 = {colorZ3R, colorZ3G, colorZ3B};
//ZONE 1
float clockFromZ1=0; float clockToZ1=24;
uint8_t colorZ1R = 0, colorZ1G = 0xff, colorZ1B = 0xff;
rgb24 colorZ1 = {colorZ1R, colorZ1G, colorZ1B};
//ZONE 2
uint16_t dispStatZ2 =           0b1100000000000001;
const uint16_t isSecZ2 =        0b0000000000000001;
const uint16_t isTempZ2 =       0b1000000000000000;
const uint16_t isWindZ2 =       0b0100000000000000;
uint8_t colorZ2R = 0xff, colorZ2G = 0xff, colorZ2B = 0x00;
rgb24 colorZ2 = {colorZ2R, colorZ2G, colorZ2B};

//WEATHER
uint8_t updInter = 10; //for web interface optim 10!
String strWeather = "", strWeatherFcast = ""; 
const uint32_t PERIOD_WEATHER_SEND = updInter*60000;
const uint32_t PERIOD_FORECAST_SEND = updInter*60000*2;
String weatherHost  = "api.openweathermap.org";
String  weatherKey    = "*********"; 
String  cityID     = "732770"; // Other city code http://bulk.openweathermap.org/sample/city.list.json.gz
const char* overboard[] PROGMEM = {"За бортом ", "Извън борда ", "Overboard "};
const char* temper[] PROGMEM = {". Темп.", ". Темп.", ". Temp "};
const char* hum[] PROGMEM = {"~С. Влаж.", "~С. Влаж", "~С. Hum "};
const char* pres[] PROGMEM = {"% Давл.", "% Наляг.", "% Press "};
const char* wind[] PROGMEM = {"мм. Ветер ", "мм. Вятър ", "mm. Wind "};
const char* windsp[] PROGMEM = {" м/с ", " м/с ", " m/s "};
const char* windir_ru[] PROGMEM = {"С-В ", "В ", "Ю-В ", "Ю ", "Ю-З ", "З ", "С-З ", "С "};
const char* windir_bg[] PROGMEM = {"С-И ", "И ", "Ю-И ", "Ю ", "Ю-З ", "З ", "С-З ", "С "};
const char* windir_en[] PROGMEM = {"N-E ", "E ", "S-E ", "S ", "S-W ", "W ", "N-W ", "N "};
const char** windir_table[] PROGMEM = {windir_ru, windir_bg, windir_en};
const char* windir[] PROGMEM = {"NE", "E", "SE", "S", "SW", "W", "NW", "N"};
const char* cloudstxt[] PROGMEM = {" Обл.", " Обл.", " Cloud "};
const char* forecast[] PROGMEM = {"Завтра  ", "Утре ", "Tomorrow "};
const char* tempermin[] PROGMEM = {". Темп.мин ", ". Темп.мин ", ". Temp.min "};
const char* tempermax[] PROGMEM = {"~С макс ", "~С макс ", "~С max "};
const char* tempersea[] PROGMEM = {"Темп.моря ", "Морската вода ", "Sea temp "};
uint8_t weatherIcon = 1;
String weatherSpeed = "", weatherDirection = "";
String weatherTemp = "";
//WEATHER

//NEWS
const uint32_t PERIOD_NEWS_SEND = updInter*60000*4;
String newsURL = "newsapi.org";
String newsAPI = "********";
String strNews = ""; 
//NEWS

const char* day_ru[] PROGMEM = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
const char* day_bg[] PROGMEM = {"Неделя", "Понеделник", "Вторник", "Сряда", "Четвертък", "Петък", "Събота"};
const char* day_en[] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char** day_table[] PROGMEM = {day_ru, day_bg, day_en};
const char* month_ru[] PROGMEM = {"Января", "Февраля", "Марта", "Апреля", "Мая", "Июня", "Июля", "Августа", "Сентября", "Октября", "Ноября", "Декабря"};
const char* month_bg[] PROGMEM = {"Януари", "Февруари", "Март", "Април", "Май", "Юни", "Юли", "Август", "Септември", "Октомври", "Ноември", "Декември"};
const char* month_en[] PROGMEM = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char** month_table[] PROGMEM = {month_ru, month_bg, month_en};
const char* dayS_ru[] PROGMEM = {"Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"};
const char* dayS_bg[] PROGMEM = {"Нд", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"};
const char* dayS_en[] PROGMEM = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
const char** dayS_table[] PROGMEM = {dayS_ru, dayS_bg, dayS_en};
const char* monthS_ru[] PROGMEM = {"Янв", "Фев", "Мар", "Апр", "Май", "Июн", "Июл", "Авг", "Сен", "Окт", "Ноя", "Дек"};
const char* monthS_bg[] PROGMEM = {"Яну", "Фев", "Мар", "Апр", "Май", "Юни", "Юли", "Авг", "Сеп", "Окт", "Ное", "Дек"};
const char* monthS_en[] PROGMEM = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char** monthS_table[] PROGMEM = {monthS_ru, monthS_bg, monthS_en};

//YOUTUBE
String host = "api.diy-led.eu";
String googleAPI = "********";  // API KEY выданный Google 
String ytChannelID = "*********"; // ID YouTube канала my 
String ytVideoID = "gmar5g7X_z4"; //my AcInk0otyjk 
bool channelOn = false, videoOn = true;
const uint32_t PERIOD_YT_SEND = 60000;
struct channelStatistics{
  long viewCount;
  long commentCount;
  long subscriberCount;
  bool hiddenSubscriberCount;
  long videoCount;
};
struct videoStatistics{
  long viewCount;
  long likeCount;
  long dislikeCount;
  long favoriteCount;
  long commentCount;
};
channelStatistics channelStats, oldchannelStats;
videoStatistics videoStats, oldvideoStats;
//oldchannelStats = (channelStatistics) {0,0,0,0,0};
//oldvideoStats = (videoStatistics) {0,0,0,0,0};
const char* youtube_ru[] PROGMEM = {"КАНАЛ подписчики: ", ", просмотры: ", ", комментарии: ", "ВИДЕО лайки: "};
const char* youtube_bg[] PROGMEM = {"КАНАЛ подписчики: ", ", просмотры: ", ", комментарии: ", "ВИДЕО лайки: "};
const char* youtube_en[] PROGMEM = {"CHANNEL subscribers: ", ", views: ", ", comments: ", "VIDEO likes: "};
const char** youtube_table[] PROGMEM = {youtube_ru, youtube_bg, youtube_en};
enum statTypeDisp {
  CHANNEL_SUBCR,
  CHANNEL_VIEW,
  CHANNEL_COMMENT,
  VIDEO_VIEW,
  VIDEO_LIKE,
  VIDEO_COMMENT,
};
statTypeDisp curTypeDisp = VIDEO_LIKE;

// OPEN/CLOSE
#ifdef USE_OC_MODE
const char* ocmode_ru[] PROGMEM = {"ОТКРЫТО", "ЗАКРЫТО"};
const char* ocmode_bg[] PROGMEM = {"ОТВОРЕНО", "ЗАТВОРЕНО"};
const char* ocmode_en[] PROGMEM = {"OPEN", "CLOSE"};
const char** ocmode_table[] PROGMEM = {ocmode_ru, ocmode_bg, ocmode_en};
bool statOC = true;
float openFrom = 9.3, openTo = 18.3; //, closeFrom = 18.3, closeTo = 9.3;
rgb24 colorOpen = {0x00, 0xff, 0x00}, colorClose = {0xff, 0x00, 0x00};
#endif


#endif
