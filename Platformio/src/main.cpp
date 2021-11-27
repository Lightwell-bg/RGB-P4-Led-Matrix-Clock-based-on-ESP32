//Multifunctional clock based on ESP8266 и MAX79xxx for Home Automation (IoT)
#include <Arduino.h>
#include "config.h"
#include "configMP.h"
#include "configFile.h"
#include "display.h"
#include "weather.h"
#if USE_ASINCWS == true
  #include "httpAWServer.h"
  #include "fwareupdAWS.h"
#else
  #include "httpServer.h"
  #include "fwareupd.h"
#endif
#include "displayZone.h"
#include "youtubeDisp.h"
#include "youtube.h"


void setup() {
  Serial.begin(115200);
  char host[16];
  if(!SPIFFS.begin()) { // Initialize SPIFFS
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  } 
  loadConfig(); Serial.println(F("FileConfig init"));
  myWIFI.begin(ssid.c_str(), password.c_str(), SSDP_Name.c_str(), ssidAP.c_str(), passwordAP.c_str(), apIP.c_str()); Serial.println(F("Wi-Fi init"));
  if (typeSync == 1) { //if use GPS we cant set timezone on ESP
        myESPTime.begin(timezone, isDayLightSaving, sNtpServerName, sNtpServerName2, sNtpServerName3, false, false);
        Serial.println(F("Start Time"));
    }
    init_firmWareUpdate(); Serial.println(F("Start init FirmWare update"));
    SSDP_Name = myWIFI.getNameSSDP(); Serial.println(SSDP_Name);
    if (WiFi.status() == WL_CONNECTED)  {
#if USE_ASINCWS == true
        mySSDP.begin(SSDP_Name.c_str(), "000000001", nName.c_str(), nVersion.c_str(), "LIGHTWELL", "https://diy-led.eu"); Serial.println(F("Start init SSDP"));
#else        
        init_SSDP(); Serial.println(F("Start SSDP"));   //Run SSDP
#endif
    }
    else {
        dnsServer.setErrorReplyCode(DNSReplyCode::NoError); // Captive_Portal
        dnsServer.start(DNS_PORT, F("*"), WiFi.softAPIP()); // Captive_Portal
    }
    init_HTTPServer(); Serial.println(F("HTTPServer init"));
    SSDP_Name.toCharArray(host, SSDP_Name.length()+1);
    NBNS.begin(SSDP_Name.c_str());
    Serial.printf("Ready! Open http://%s in your browser\n", host);
    Serial.println(myWIFI.getDevStatusIP()); 
    myWIFI.beginUDP(localUdpPort); //init_udp();
    init_display(); Serial.println(F("init_display"));
    myWeather.setWeatherServer(weatherHost.c_str(), weatherKey.c_str(), cityID.c_str());
    myNews.begin(newsURL.c_str(), newsAPI.c_str());
    startLogo += myWIFI.getDevStatusIP();
    Serial.println(startLogo);
    while (!displayScrollText(startLogo));
    //delay(1000);
}

void loop() {
    static uint32_t tCountYT1 = 0, tCountYT2 = 0, tCountNews = 0, tCountYTube = 0;
    static uint8_t oldModeDisplay = 255;
    static bool oldisBigClock = true, showYTube = false;
    if (WiFi.status() != WL_CONNECTED) {
      dnsServer.processNextRequest(); // Captive_Portal
    }
#if USE_ASINCWS != true
    HTTP.handleClient();
#endif
    myWIFI.returnIPtoUDP(); //listenUDP();
    //#ifdef TEST_YOUTUBE
    if (testYOUTUBE) {
      if (!channelOn && !videoOn) modeDisplay = 2;
      if ((millis() - tCountYTube > testper) && (channelOn || videoOn)) {
        static uint8_t stepS = 1;
        //if (modeDisplay == 3) modeDisplay = 31; else modeDisplay = 3;
        if (stepS == 4) stepS = 1;
        if (stepS == 1) curTypeDisp = CHANNEL_SUBCR;
        if (stepS == 2) curTypeDisp = VIDEO_COMMENT;
        if (stepS == 3) curTypeDisp = VIDEO_LIKE;
        stepS++;
        modeDisplay = 31;
        tCountYTube = millis();
      }
    }
    //#endif
    struct tm tnow = myESPTime.getTimeStruct();
    if (modeDisplay != oldModeDisplay || isBigClock != oldisBigClock) {
      clearAllLayer();
      oldModeDisplay = modeDisplay;
      oldisBigClock = isBigClock;
    }
    if (myESPTime.compTimeInt(global_start, global_stop, &tnow)) {
      if (myESPTime.compTimeInt(dmodefrom, dmodeto, &tnow)) matrix.setBrightness(brightd); else matrix.setBrightness(brightn);
      if (isBigClock && (myESPTime.compTimeInt(clockBigFrom, clockBigTo, &tnow))) {
        displayBigTime(showSec);
      }
      else {
        if (modeDisplay == 2) { //INFO MODE
          displayZ1(tnow);
          displayZ2(tnow);
          if (!showYTube) {if (displayZ3(tnow)) showYTube = true;}
          if (showYTube) {if (diplayYouTStatZ3()) showYTube = false;
        }}
        /*else if (modeDisplay == 3) { //YOUTUBE mode
          displayZ1(tnow);
          displayZ2(tnow);
          if (diplayYouTStatZ3()) modeDisplay++;
        }   */
        else if (modeDisplay == 31) { //YOUTUBE ICON mode   
          if(displayYTAlarm(curTypeDisp)) modeDisplay = 2;
        }                           
        else {
          //modeDisplay = 2;
        }
      }
    }
      
    //*********GET DATA
    if (((isLedWeatherZ3 & dispStatZ3) && myESPTime.compTimeInt(weathFromZ3, weathToZ3, &tnow)) || (dispStatZ2 & isTempZ2) || (dispStatZ2 & isWindZ2)) {
        getWeather(60000, PERIOD_WEATHER_SEND);
    }
    if ((isLedForecastZ3 & dispStatZ3) && myESPTime.compTimeInt(fcastFromZ3, fcastToZ3, &tnow)) {
        getWeatherForecast(90000, PERIOD_FORECAST_SEND);
    }
    if ((isNews & dispStatZ3) && myESPTime.compTimeInt(newsFromZ3, newsToZ3, &tnow)) {
      if (millis() - tCountNews > PERIOD_NEWS_SEND) {
        strNews = myNews.getNewsWiFiClient(&lang);
        //strNews = myNews.getNewsHTTPClient(&lang);
        Serial.print("strNews: "); Serial.println(strNews);
        tCountNews = millis();
      } 
    } 
    if (channelOn) {
      if (millis() - tCountYT1 > PERIOD_YT_SEND*4) {
        if (obtain_channel_stats()) handleChannel();
        tCountYT1 = millis();
      } 
    } 
    if (videoOn) {
      if (millis() - tCountYT2 > PERIOD_YT_SEND*3) {
        if (obtain_video_stats()) handleVideo();
        tCountYT2 = millis();
      } 
    } 
    yield();
  //delay(2);//allow the cpu to switch to other tasks
}