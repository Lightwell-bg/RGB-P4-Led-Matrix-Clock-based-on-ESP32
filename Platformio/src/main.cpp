//Multifunctional clock based on ESP8266 и MAX79xxx for Home Automation (IoT)
#include <Arduino.h>
#include "config.h"
#include "configMP.h"
#include "configFile.h"
#include "fwareupd.h"
#include "display.h"
#include "httpServer.h"
#include "displayZone.h"
#include "weather.h"


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
        init_SSDP(); Serial.println(F("Start SSDP"));   //Run SSDP
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

    //delay(1000);
}

void loop() {
    static uint32_t tCount = 0, tCountNews = 0;
    static uint8_t oldModeDisplay = 255;
    static bool oldisBigClock = true;
    if (WiFi.status() != WL_CONNECTED) {
      dnsServer.processNextRequest(); // Captive_Portal
    }
    HTTP.handleClient();
    myWIFI.returnIPtoUDP(); //listenUDP();
    modeDisplay = 2;
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
        if (modeDisplay == 2) {
          displayZ1(tnow);
          displayZ2(tnow);
          displayZ3(tnow);
        }
        //else if () {}                                 
        else {}
      }
    }
      
    //*********GET DATA
    if (((isLedWeatherZ3 & dispStatZ3) && myESPTime.compTimeInt(weathFromZ3, weathToZ3, &tnow)) || (dispStatZ2 & isTempZ2) || (dispStatZ2 & isWindZ2)) {
        getWeather(PERIOD_WEATHER_SEND, PERIOD_WEATHER_SEND);
    }
    if ((isLedForecastZ3 & dispStatZ3) && myESPTime.compTimeInt(fcastFromZ3, fcastToZ3, &tnow)) {
        getWeatherForecast(PERIOD_FORECAST_SEND, PERIOD_FORECAST_SEND);
    }
    if (millis() - tCount > 70000*3) {
      obtain_subscriber_stats();
      tCount = millis();
    }
    if ((isNews & dispStatZ3) && myESPTime.compTimeInt(newsFromZ3, newsToZ3, &tnow)) {
      if (millis() - tCountNews > PERIOD_NEWS_SEND) {
        strNews = myNews.getNewsWiFiClient(&lang);
        //strNews = myNews.getNewsHTTPClient(&lang);
        Serial.print("strNews: "); Serial.println(strNews);
        tCountNews = millis();
      } 
    }   
    yield();
  //delay(2);//allow the cpu to switch to other tasks
}