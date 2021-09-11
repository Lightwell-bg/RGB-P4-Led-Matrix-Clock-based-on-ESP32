#ifndef newsAp_h
#define newsAp_h
#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <ArduinoJson.h>        //https://github.com/bblanchon/ArduinoJson.git

#define HTTP_CODE_OK 200
#define USE_SERIAL Serial

class newsAp {
    public:
        newsAp(WiFiClient *client);
        newsAp(HTTPClient *client);
        void begin(const char *api_url, const char *api_key);
        String getNewsHTTPClient(const uint8_t* lang);
        String getNewsWiFiClient(const uint8_t* lang);

    private:
        WiFiClient* _clientWiFi;
        HTTPClient* _clientHTTP;
        const char* _api_url;
        const char* _api_key;

};

#endif