void notFound(AsyncWebServerRequest *request);
String processor(const String& var);

void init_HTTPServer(void) {
    HTTP.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
           request->send(SPIFFS, "/index.html", "text/html");
    });
    HTTP.on("/index.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    HTTP.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });  
    HTTP.on("/img/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/img/favicon.png", "image/x-icon");
    });      
    HTTP.on("/function.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/function.js", "application/javascript");
    }); 
    HTTP.on("/lang_EN.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/lang_EN.js", "application/javascript");
    });  
    HTTP.on("/lang_RU.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/lang_RU.js", "application/javascript");
    });    
    HTTP.on("/lang_BG.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/lang_BG.js", "application/javascript");
    });  
    HTTP.on("/wifi.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        //request->send(SPIFFS, "/wifi.html", "text/html");
        request->send(SPIFFS, "/wifi.html", String(), false, processor);
    });
    HTTP.on("/time.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/time.html", "text/html");
    });
    HTTP.on("/setup.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/setup.html", "text/html");
    });
    HTTP.on("/shop.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/shop.html", "text/html");
    });
    HTTP.on("/rdata.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/rdata.html", "text/html");
    });
    HTTP.on("/help.html", HTTP_ANY, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/help.html", "text/html");
    }); 
    HTTP.on(filePath.c_str(), HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, filePath, "application/json");
    });     
    HTTP.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {   
        String restart = request->getParam("device")->value(); 
        if (restart == "ok") {                         
            request->send(200, "text/html", "Restart OK"); 
            ESP.restart();                                
        }
        else {                                        
            request->send(200, "text/html", "Restart NO");  
        }
    }); 
    HTTP.on("/resetConfig", HTTP_GET, [](AsyncWebServerRequest *request) {   //Reset og configuration file     
        String reset = request->getParam("device")->value(); 
        if (reset == "ok") {  
            //SPIFFS.format();
            SPIFFS.remove(filePath);
            Serial.println("ESP erase Config file");
            delay(3000);                                   
            request->send(200, "text/html", "Reset OK"); 
            delay(3000);
            ESP.restart();                                
        }
        else {                                        
            request->send(200, "text/html", "Reset NO");  
        }
    });

    HTTP.on("/configs.json", HTTP_GET, [](AsyncWebServerRequest *request) {
        struct tm tnow = myESPTime.getTimeStruct();
        String root = "{}";  
        DynamicJsonDocument jsonDoc(5096);//4096
        DeserializationError error =  deserializeJson(jsonDoc, root);
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        }     
        JsonObject json = jsonDoc.as<JsonObject>(); 
        json["cVersion"] = cVersion;   
        json["SSDP"] = myWIFI.getNameSSDP();
        json["ssidAP"] = myWIFI.getNameAPSSID();
        json["passwordAP"] = myWIFI.getPassAPSSID();
        json["ssid"] = myWIFI.getNameSSID();
        json["password"] = myWIFI.getPassSSID();
        json["ip"] = myWIFI.getDevStatusIP();
        json["time"] = myESPTime.getTimeStr(true);
        json["date"] = myESPTime.getDateStr();
        json["time_h"] = String(tnow.tm_hour);
        json["time_m"] = String(tnow.tm_min);
        json["time_s"] = String(tnow.tm_sec); 
        json["date_day"] = tnow.tm_mday;
        json["date_month"] = tnow.tm_mon + 1;
        json["date_year"] = tnow.tm_year + 1900;
        json["timezone"] = timezone;
        json["isDayLightSaving"] = (isDayLightSaving?"checked":"");
        json["ntpserver"] = sNtpServerName; 
        json["use_sync"] = typeSync;
        json["lang"] = (lang==0?"RU":lang==1?"BG":lang==2?"EN":"EN");
        json["global_start"] = global_start; json["global_stop"] = global_stop;
        json["fontUsed"] = fontUsed; 
        json["brightd"] = brightd; json["brightn"] = brightn;
        json["brauto"] = (brauto?"checked":"");
        json["dmodefrom"] = dmodefrom; json["dmodeto"] = dmodeto;
        json["speedTicker"] = speedTicker;
        json["modeDisplay"] = modeDisplay;
        json["showSec"]=(showSec?"checked":""); //ZONE BIG
        json["isBigClock"] = (isBigClock?"checked":"");
        json["clockBigFrom"] = clockBigFrom; json["clockBigTo"] = clockBigTo;
        //ZONE 3
        json["isLedDateZ3"]=(dispStatZ3 & isLedDateZ3?"checked":"");
        json["dateFromZ3"] = dateFromZ3; json["dateToZ3"] = dateToZ3;
        json["isCrLineDateZ3"]=(isCrLineDateZ3?"checked":"");
        json["ledText0"] = strText[0];
        json["ledText1"] = strText[1];
        json["ledText2"] = strText[2];
        json["ledText3"] = strText[3];
        json["isTxtOn0"]=(dispStatZ3 & isTxtOn0?"checked":""); json["isTxtOn1"]=(dispStatZ3 & isTxtOn1?"checked":"");
        json["isTxtOn2"]=(dispStatZ3 & isTxtOn2?"checked":""); json["isTxtOn3"]=(dispStatZ3 & isTxtOn3?"checked":"");
        json["isCrLine0"]=(isCrLine[0]?"checked":""); json["isCrLine1"]=(isCrLine[1]?"checked":""); json["isCrLine2"]=(isCrLine[2]?"checked":"");
        json["isCrLine3"]=(isCrLine[3]?"checked":"");
        json["txtFrom0"] = txtFrom[0]; json["txtFrom1"] = txtFrom[1]; json["txtFrom2"] = txtFrom[2]; json["txtFrom3"] = txtFrom[3];
        json["txtTo0"] = txtTo[0]; json["txtTo1"] = txtTo[1]; json["txtTo2"] = txtTo[2]; json["txtTo3"] = txtTo[3];
        json["isLedWeatherZ3"]=(dispStatZ3 & isLedWeatherZ3?"checked":"");json["isLedForecastZ3"]=(dispStatZ3 & isLedForecastZ3?"checked":"");
        json["weathFromZ3"] = weathFromZ3; json["weathToZ3"] = weathToZ3;
        json["fcastFromZ3"] = fcastFromZ3; json["fcastToZ3"] = fcastToZ3;
        json["isNews"]=(dispStatZ3 & isNews?"checked":"");
        json["newsFromZ3"] = newsFromZ3; json["newsToZ3"] = newsToZ3;
        json["colorZ3R"] = colorZ3R; json["colorZ3G"] = colorZ3G;json["colorZ3B"] = colorZ3B;

        json["clockFromZ1"] = clockFromZ1; json["clockToZ1"] = clockToZ1;//ZONE 1
        json["colorZ1R"] = colorZ1R; json["colorZ1G"] = colorZ1G;json["colorZ1B"] = colorZ1B;
        //ZONE 2
        json["isSecZ2"]=(dispStatZ2 & isSecZ2?"checked":"");
        json["isTempZ2"]=(dispStatZ2 & isTempZ2?"checked":"");
        json["isWindZ2"]=(dispStatZ2 & isWindZ2?"checked":"");
        json["colorZ2R"] = colorZ2R; json["colorZ2G"] = colorZ2G;json["colorZ2B"] = colorZ2B;
        
        json["weatherHost"] = weatherHost;
        json["w_api"] = weatherKey;
        json["city_code"] = cityID;
        json["newsAPI"] = newsAPI;
        json["lastupw"] = (millis() - myWeather.lastUpdate)/60000;
        json["lastupwf"] = (millis() - myWeather.fLastUpdate)/60000;
        json["googleAPI"] = googleAPI;
        json["ytChannelID"] = ytChannelID;
        json["channelOn"]=(channelOn?"checked":"");
        json["ytVideoID"] = ytVideoID;
        json["videoOn"] = (videoOn?"checked":"");
    #ifdef USE_OC_MODE
        json["openFrom"] = openFrom;
        json["openTo"] = openTo;
        json["statOC"] = (statOC?"checked":"");
    #endif
        json["testYOUTUBE"] = (testYOUTUBE?"TEST MODE":"");
        root = "";
        serializeJson(json, root);
        request->send(200, "text/json", root);
    });
    HTTP.on("/lang", HTTP_GET, [](AsyncWebServerRequest *request){    
        if(request->getParam("lang")->value() == "RU" && lang !=0) lang = 0; 
        else if(request->getParam("lang")->value() == "BG" && lang !=1) lang = 1; 
        else if(request->getParam("lang")->value() == "EN" && lang !=2) lang = 2; 
        else {request->send(200, "text/html", "OK"); return;} 
        Serial.print("Set lang: ");    Serial.println(request->getParam("lang")->value());
        saveConfig();
        request->send(200, "text/html", "OK");
    });  
    HTTP.on("/ssid", HTTP_GET, [](AsyncWebServerRequest *request) {
        ssid = request->getParam("ssid")->value();   
        password = request->getParam("password")->value();  
        ssidAP = request->getParam("ssidAP")->value();
        passwordAP = request->getParam("passwordAP")->value();
        SSDP_Name = request->getParam("ssdp")->value();
        if (passwordAP.length() != 0) {
            if (passwordAP.length() < 8 || passwordAP.length() > 63)  passwordAP = "";  
        }
        //if (strlen(_apPassword) < 8 || strlen(_apPassword) > 63) {
        myWIFI.setConfigWIFI(ssid.c_str(), password.c_str(), SSDP_Name.c_str(), ssidAP.c_str(), passwordAP.c_str()); 
        Serial.println("Set SSID: " + ssid + ", Set password: " + password + ", Set SSID AP: " + ssidAP + ", Set AP password: " + passwordAP + ", SSDP: " + SSDP_Name);
        request->send(200, "text/html", "OK");
    });
    HTTP.on("/TimeZone", HTTP_GET, [](AsyncWebServerRequest *request){    
        timezone = request->getParam("timezone")->value().toInt(); 
        isDayLightSaving = request->getParam("isDayLightSaving")->value().toInt(); 
        saveConfig();
        if (typeSync == 1) { //NTP
            myESPTime.setTimeParam(false, timezone, isDayLightSaving, sNtpServerName);
            myESPTime.timeSynch();
        }
        Serial.println("NTP Time Zone: " + String(timezone) + ",  isDayLightSaving: " + String(isDayLightSaving));
        request->send(200, "text/html", "OK");        
    }); 
    HTTP.on("/setntpserver", HTTP_GET, [](AsyncWebServerRequest *request){
        uint8_t oldtypeSync = typeSync;
        sNtpServerName = request->getParam("ntpserver")->value().c_str(); 
        typeSync = request->getParam("use_sync")->value().toInt();
        if (typeSync == 2) {//GPS but no
            typeSync = oldtypeSync;
        }
        saveConfig();
        if (typeSync == 1) { //NTP
            //myESPTime.setTimeParam(useRTC, timezone, isDayLightSaving, sNtpServerName);
            //myESPTime.timeSynch();
            myESPTime.begin(timezone, isDayLightSaving, sNtpServerName, sNtpServerName2, sNtpServerName3, false, false);
        }
        Serial.println("sNtpServerName: " + sNtpServerName + ", typeSync: " + typeSync);
        request->send(200, "text/html", "OK"); 
    });
    HTTP.on("/Time", HTTP_GET, [](AsyncWebServerRequest *request){    
        if (typeSync == 1) { //NTP
            myESPTime.timeSynch();
        }
        Serial.println(F("timeSynch()")); 
        request->send(200, "text/html", "OK");        
    });
    HTTP.on("/TimeNew", HTTP_GET, [](AsyncWebServerRequest *request) {
        struct tm timeman;
        time_t epoch_time;
        uint8_t new_h = request->getParam("time_h")->value().toInt();
        uint8_t new_m = request->getParam("time_m")->value().toInt();
        uint8_t new_s = request->getParam("time_s")->value().toInt();
        uint8_t new_day = request->getParam("date_day")->value().toInt(); 
        uint8_t new_month = request->getParam("date_month")->value().toInt();
        uint16_t new_year = request->getParam("date_year")->value().toInt();     
        timeman.tm_hour = new_h; 
        timeman.tm_min = new_m;
        timeman.tm_sec = new_s;
        timeman.tm_mday = new_day;
        timeman.tm_mon = new_month - 1;
        timeman.tm_year = new_year - 1900; 
        epoch_time = mktime(&timeman);// - timezone * 3600;
        struct timeval epoch;
        epoch = {epoch_time, 0};
        settimeofday(&epoch, nullptr);
        printf("Setting time: %s", asctime(&timeman)); 
        Serial.print(F("Setting time UNIX: ")); Serial.println(epoch_time);   
        typeSync = 0;
        saveConfig();
        request->send(200, "text/html", "OK");
    });
    HTTP.on("/ledsetup", HTTP_GET, [](AsyncWebServerRequest *request){    
        speedTicker = request->getParam("speedTicker")->value().toInt();
        brightd = request->getParam("brightd")->value().toInt();
        brightn = request->getParam("brightn")->value().toInt();
        request->getParam("brauto")->value().toInt()==1?brauto=true:brauto=false;
        dmodefrom = request->getParam("dmodefrom")->value().toInt();
        dmodeto = request->getParam("dmodeto")->value().toInt();
        //fontUsed = request->getParam("fontUsed")->value().toInt();
        saveConfig();
        Serial.print("speedTicker ");Serial.println(speedTicker); Serial.print("brightd ");Serial.println(brightd);Serial.print("brightn ");Serial.println(brightn);
        Serial.print("dmodefrom ");Serial.println(dmodefrom);Serial.print("dmodeto ");Serial.println(dmodeto);
        request->send(200, "text/html", "OK");        
    });
    HTTP.on("/weather", HTTP_GET, [](AsyncWebServerRequest *request){    
        weatherHost = request->getParam("weatherHost")->value().c_str();
        cityID = request->getParam("city_code")->value().c_str(); 
        weatherKey = request->getParam("w_api")->value().c_str();
        saveConfig();                
        //getWeather(1000);
        Serial.println("weatherHost: " + weatherHost + ", cityID: " + cityID + ", weatherKey: " + weatherKey);
        request->send(200, "text/html", "OK");        
    });
    HTTP.on("/weatherUpdate", HTTP_GET, [](AsyncWebServerRequest *request){   
        String upd = request->getParam("update")->value(); 
        if(upd == "ok") {
            if ((isLedWeatherZ3 & dispStatZ3) || (dispStatZ2 & isTempZ2)) getWeather(0, 250);
             request->send(200, "text/html", "OK");
        }   
    });
    HTTP.on("/setnews", HTTP_GET, [](AsyncWebServerRequest *request){    
        newsAPI = request->getParam("newsAPI")->value().c_str();
        saveConfig();                
        Serial.println("newsAPI: " + newsAPI);
        request->send(200, "text/html", "OK");        
    });
    HTTP.on("/newsUpdate", HTTP_GET, [](AsyncWebServerRequest *request){   
        String upd = request->getParam("update")->value(); 
        if(upd == "ok") {
            if (isNews & dispStatZ3) strNews = myNews.getNewsWiFiClient(&lang);
            Serial.print("strNews: "); Serial.println(strNews);
             request->send(200, "text/html", "OK");
        }   
    });
    HTTP.on("/setyoutube", HTTP_GET, [](AsyncWebServerRequest *request){    
        googleAPI = request->getParam("googleAPI")->value().c_str();
        ytChannelID = request->getParam("ytChannelID")->value().c_str(); 
        ytVideoID = request->getParam("ytVideoID")->value().c_str();
        request->getParam("channelOn")->value().toInt()==1?channelOn=true:channelOn=false;
        request->getParam("videoOn")->value().toInt()==1?videoOn=true:videoOn=false;
        saveConfig();                
        Serial.println("googleAPI: " + googleAPI + ", ytChannelID: " + ytChannelID + ", ytVideoID: " + ytVideoID);
        request->send(200, "text/html", "OK");        
    });
    HTTP.on("/testyoutube", HTTP_GET, [](AsyncWebServerRequest *request){    
        request->getParam("test")->value().toInt()==1?testYOUTUBE=true:testYOUTUBE=false;
        testper =  request->getParam("testper")->value().toInt();
        saveConfig();
        Serial.println(testYOUTUBE ? "testYOUTUBE: ON" : "testYOUTUBE: OFF");
        request->send(200, "text/html", "OK");        
    });
#ifdef USE_OC_MODE
    HTTP.on("/shopopenclose", HTTP_GET, [](AsyncWebServerRequest *request) {
        openFrom = request->getParam("openFrom")->value().toFloat(); openTo = request->getParam("openTo")->value().toFloat();
        (request->getParam("statOC")->value().toInt()==1 ?  statOC = true : statOC = false);
        saveConfig();                 
        Serial.print("statOC ");Serial.println(statOC);
        request->send(200, "text/html", "OK");
    });
#endif
    HTTP.on("/screenoption", HTTP_GET, [](AsyncWebServerRequest *request) {
        global_start = request->getParam("global_start")->value().toFloat(); global_stop = request->getParam("global_stop")->value().toFloat();
        (request->getParam("isBigClock")->value().toInt()==1 ?  isBigClock = true : isBigClock = false);
        clockBigFrom = request->getParam("clockBigFrom")->value().toFloat(); clockBigTo = request->getParam("clockBigTo")->value().toFloat();
        (request->getParam("showSec")->value().toInt()==1 ?  showSec = true : showSec = false);
        backgroundLayer.fillScreen(BLACK); //Clear Sec
        backgroundLayer.swapBuffers();
        saveConfig();
        request->send(200, "text/html", "OK");
    });
    HTTP.on("/setcolor_z1", HTTP_GET, [](AsyncWebServerRequest *request) {
        colorZ1R = request->getParam("colorZ1R")->value().toInt(); colorZ1G = request->getParam("colorZ1G")->value().toInt(); colorZ1B = request->getParam("colorZ1B")->value().toInt();
        colorZ1.red = colorZ1R; colorZ1.green = colorZ1G; colorZ1.blue = colorZ1B;
        saveConfig();
        request->send(200, "text/html", "OK");
    });
    HTTP.on("/set_z2", HTTP_GET, [](AsyncWebServerRequest *request) {
        global_start = request->getParam("global_start")->value().toFloat(); global_stop = request->getParam("global_stop")->value().toFloat();
        if (request->getParam("isSecZ2")->value().toInt()==1) dispStatZ2 |= isSecZ2; else dispStatZ2 &= ~isSecZ2;
        if (request->getParam("isTempZ2")->value().toInt()==1) dispStatZ2 |= isTempZ2; else dispStatZ2 &= ~isTempZ2;
        if (request->getParam("isWindZ2")->value().toInt()==1) dispStatZ2 |= isWindZ2; else dispStatZ2 &= ~isWindZ2;
        saveConfig();                 
        Serial.print("dispStatZ2 ");Serial.println(dispStatZ2, BIN);
        request->send(200, "text/html", "OK");
    });
    HTTP.on("/setcolor_z2", HTTP_GET, [](AsyncWebServerRequest *request) {
        colorZ2R = request->getParam("colorZ2R")->value().toInt(); colorZ2G = request->getParam("colorZ2G")->value().toInt(); 
        colorZ2B = request->getParam("colorZ2B")->value().toInt();
        colorZ2.red = colorZ2R; colorZ2.green = colorZ2G; colorZ2.blue = colorZ2B;
        saveConfig();
        request->send(200, "text/html", "OK");
    });
    HTTP.on("/texts", HTTP_GET, [](AsyncWebServerRequest *request) {
        global_start = request->getParam("global_start")->value().toFloat(); global_stop = request->getParam("global_stop")->value().toFloat();
        if (request->getParam("isLedDateZ3")->value().toInt()==1) dispStatZ3 |= isLedDateZ3; else dispStatZ3 = dispStatZ3 & ~isLedDateZ3;
        dateFromZ3 = request->getParam("dateFromZ3")->value().toFloat(); dateToZ3 = request->getParam("dateToZ3")->value().toFloat();
        request->getParam("isCrLineDateZ3")->value().toInt()==1?isCrLineDateZ3=true:isCrLineDateZ3=false;
        if (request->getParam("isLedWeatherZ3")->value().toInt()==1) dispStatZ3 |= isLedWeatherZ3; else dispStatZ3 &= ~isLedWeatherZ3;
        if (request->getParam("isLedForecastZ3")->value().toInt()==1) dispStatZ3 |= isLedForecastZ3; else dispStatZ3 &= ~isLedForecastZ3;
        if (request->getParam("isNews")->value().toInt()==1) dispStatZ3 |= isNews; else dispStatZ3 &= ~isNews;
        weathFromZ3 = request->getParam("weathFromZ3")->value().toFloat(); weathToZ3 = request->getParam("weathToZ3")->value().toFloat();
        fcastFromZ3 = request->getParam("fcastFromZ3")->value().toFloat(); fcastToZ3 = request->getParam("fcastToZ3")->value().toFloat();
        newsFromZ3 = request->getParam("newsFromZ3")->value().toFloat(); newsToZ3 = request->getParam("newsToZ3")->value().toFloat();
        strText[0] = request->getParam("ledText0")->value().c_str(); // 
        strText[1] = request->getParam("ledText1")->value().c_str(); // 
        strText[2] = request->getParam("ledText2")->value().c_str(); // 
        strText[3] = request->getParam("ledText3")->value().c_str(); //
        if (request->getParam("isTxtOn0")->value().toInt()==1) dispStatZ3 |= isTxtOn0; else dispStatZ3 = dispStatZ3 & ~isTxtOn0;
        if (request->getParam("isTxtOn1")->value().toInt()==1) dispStatZ3 |= isTxtOn1; else dispStatZ3 = dispStatZ3 & ~isTxtOn1;
        if (request->getParam("isTxtOn2")->value().toInt()==1) dispStatZ3 |= isTxtOn2; else dispStatZ3 = dispStatZ3 & ~isTxtOn2;
        if (request->getParam("isTxtOn3")->value().toInt()==1) dispStatZ3 |= isTxtOn3; else dispStatZ3 = dispStatZ3 & ~isTxtOn3;
        txtFrom[0] = request->getParam("txtFrom0")->value().toFloat();
        txtFrom[1] = request->getParam("txtFrom1")->value().toFloat();
        txtFrom[2] = request->getParam("txtFrom2")->value().toFloat();
        txtFrom[3] = request->getParam("txtFrom3")->value().toFloat();
        txtTo[0] = request->getParam("txtTo0")->value().toFloat();
        txtTo[1] = request->getParam("txtTo1")->value().toFloat();
        txtTo[2] = request->getParam("txtTo2")->value().toFloat();
        txtTo[3] = request->getParam("txtTo3")->value().toFloat();
        request->getParam("isCrLine0")->value().toInt()==1?isCrLine[0]=true:isCrLine[0]=false;
        request->getParam("isCrLine1")->value().toInt()==1?isCrLine[1]=true:isCrLine[1]=false;
        request->getParam("isCrLine2")->value().toInt()==1?isCrLine[2]=true:isCrLine[2]=false;
        request->getParam("isCrLine3")->value().toInt()==1?isCrLine[3]=true:isCrLine[3]=false;
        saveConfig();                 
        Serial.print("strText0 ");Serial.println(strText[0]);
        Serial.print("strText1 ");Serial.println(strText[1]);
        Serial.print("strText2 ");Serial.println(strText[2]);
        Serial.print("strText3 ");Serial.println(strText[3]);
        Serial.print("dispStatZ3 ");Serial.println(dispStatZ3, BIN);
        indexedLayerZ3.fillScreen(BLACK);
        indexedLayerZ3.swapBuffers();
        request->send(200, "text/html", "OK");
    });
    HTTP.on("/setcolor_z3", HTTP_GET, [](AsyncWebServerRequest *request) {
        colorZ3R = request->getParam("colorZ3R")->value().toInt(); colorZ3G = request->getParam("colorZ3G")->value().toInt();
        colorZ3B = request->getParam("colorZ3B")->value().toInt();
        colorZ3.red = colorZ3R; colorZ3.green = colorZ3G; colorZ3.blue = colorZ3B;
        saveConfig();
        request->send(200, "text/html", "OK");
    });
    

    HTTP.onNotFound(notFound);
    HTTP.begin();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

String translateEncryptionType(uint8_t encryptionType) {
   switch (encryptionType) {
    case (7):
      return "Open";
    case (5):
      return "WEP";
    case (2):
      return "WPA_PSK";
    case (4):
      return "WPA2_PSK";
    case (8):
      return "WPA_WPA2_PSK";
    default:
      return "UNKNOWN"; 
  }
}

String processor(const String& var) { //For only LIST of Wi-Fi
    String listWIFI="";
    //Serial.println(var);
    if(var == "LISTWIFI") {
        if (WiFi.status() != WL_CONNECTED) {
            listWIFI="<strong>LIST of Wi-Fi networks:</strong></br>";
            int n = WiFi.scanComplete();
            if(n == -2) {
                WiFi.scanNetworks(true);
            }
            else
                if(n) {
                    for (int i = 0; i < n; ++i) {
                        listWIFI += "ssid: <strong>"+WiFi.SSID(i)+"</strong> ";
                        listWIFI += "rssi: <strong>"+String(WiFi.RSSI(i))+"</strong> ";
                        //listWIFI += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
                        //listWIFI += ",\"channel\":"+String(WiFi.channel(i));
                        listWIFI += "secure: "+String(translateEncryptionType(WiFi.encryptionType(i)));
                        listWIFI += "</br>";
                    }
                WiFi.scanDelete();
                if(WiFi.scanComplete() == -2) {
                    WiFi.scanNetworks(true);
                }
            }    
    }
    return listWIFI;
  }
  return String();
}