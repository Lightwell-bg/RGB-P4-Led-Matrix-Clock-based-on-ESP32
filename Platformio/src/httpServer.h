//void init_SSDP(void);
void handleNotFound();
String getContentType(String filename);
bool handleFileRead(String path);
void handle_ConfigJSON();
void handle_Language();
void handle_Set_Ssid();
void handle_time_zone();
void handle_ntp_server();
void handle_Time();
void handle_TimeNew();
void handle_Restart();
void handle_resetConfig();
void handle_set_z3();
void handle_LedSetup();
void handle_Weather();
void handle_weather_update();
void handle_Setnews();
void handle_newsUpdate();
void handle_set_z2();
void handle_setcolor_z3();
void handle_setcolor_z2();
void handle_setcolor_z1();
void handle_screenoption();

void init_HTTPServer(void) {
    HTTP.serveStatic("/", SPIFFS, "/index.html");
    HTTP.on("/configs.json", handle_ConfigJSON); // create configs.json for sending to WEB interface
    HTTP.on("/lang", handle_Language);
    HTTP.on("/ssid", handle_Set_Ssid);     // Set WiFi config and SSDP
    HTTP.on("/Time", handle_Time);     // Sync Time
    HTTP.on("/TimeZone", handle_time_zone);    // Set time zone
    HTTP.on("/setntpserver", handle_ntp_server); // Set sync NTP
    HTTP.on("/TimeNew", handle_TimeNew);     // Set new time 
    HTTP.on("/restart", handle_Restart);   // reset ESP
    HTTP.on("/resetConfig", handle_resetConfig); 
    HTTP.on("/texts", handle_set_z3);     // Set stext
    HTTP.on("/ledsetup", handle_LedSetup); //speed, brightness,
    HTTP.on("/weather", handle_Weather);
    HTTP.on("/weatherUpdate", handle_weather_update);
    HTTP.on("/setnews", handle_Setnews);
    HTTP.on("/newsUpdate", handle_newsUpdate);
    HTTP.on("/set_z2", handle_set_z2);
    HTTP.on("/setcolor_z3", handle_setcolor_z3);
    HTTP.on("/setcolor_z2", handle_setcolor_z2);
    HTTP.on("/setcolor_z1", handle_setcolor_z1);
    HTTP.on("/screenoption", handle_screenoption);

    //HTTP.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
    //HTTP.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
    HTTP.onNotFound([]() {handleNotFound();});  // Captive_Portal ********************************       
    HTTP.begin();  
}

// Captive_Portal ********************************
bool captivePortal() {
  if (! HTTP.hostHeader().equals(WiFi.softAPIP().toString())) {
    HTTP.sendHeader(F("Location"), String(F("http://")) + WiFi.softAPIP().toString() + String(F("/wifi.html")), true);
    HTTP.send_P(302, "text/plain", NULL, 0);
    return true;
  }
  return false;
}

void handleNotFound() {
    if (WiFi.status() != WL_CONNECTED) {
        if (captivePortal()) return;
    }
    if (! handleFileRead(HTTP.uri()))
        HTTP.send_P(404, "text/plain", "FileNotFound");
}

void handle_ConfigJSON() { 
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
    json["brauto"] = brauto;
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
    root = "";
    serializeJson(json, root);
    HTTP.send(200, "text/json", root);
}

void handle_Language() {               
    if(HTTP.arg("lang") == "RU" && lang !=0) {
        lang = 0;
    }
    else if(HTTP.arg("lang") == "BG" && lang !=1) {
        lang = 1;
    }
    else if(HTTP.arg("lang") == "EN" && lang !=2) {
        lang = 2;
    }
    else {
        lang = 2;
    }
    Serial.print("Set lang: ");    Serial.println(HTTP.arg("lang").c_str());
    saveConfig();
    HTTP.send(200, "text/plain", "OK");
}

void handle_Set_Ssid() {
    ssid = HTTP.arg("ssid").c_str();            
    password = HTTP.arg("password").c_str();   
    ssidAP = HTTP.arg("ssidAP").c_str();        
    passwordAP = HTTP.arg("passwordAP").c_str(); 
    SSDP_Name = HTTP.arg("ssdp").c_str();
    if (passwordAP.length() != 0) {
        if (passwordAP.length() < 8 || passwordAP.length() > 63)  passwordAP = "";  
    }
    //if (strlen(_apPassword) < 8 || strlen(_apPassword) > 63) {
    myWIFI.setConfigWIFI(ssid.c_str(), password.c_str(), SSDP_Name.c_str(), ssidAP.c_str(), passwordAP.c_str()); 
    Serial.println("Set SSID: " + ssid + ", Set password: " + password + ", Set SSID AP: " + ssidAP + ", Set AP password: " + passwordAP + ", SSDP: " + SSDP_Name);
    HTTP.send(200, "text/plain", "OK");   
}

void handle_time_zone() {     
    timezone = HTTP.arg("timezone").toInt(); 
    isDayLightSaving = HTTP.arg("isDayLightSaving").toInt();
    saveConfig();
    if (typeSync == 1) { //NTP
        myESPTime.setTimeParam(false, timezone, isDayLightSaving, sNtpServerName);
        myESPTime.timeSynch();
    }
#if USE_GPS == true
    if (typeSync == 2) {//GPS
        timeSynchGPS(0, 0);
    }
#endif
    Serial.println("NTP Time Zone: " + String(timezone) + ",  isDayLightSaving: " + String(isDayLightSaving));
    HTTP.send(200, "text/plain", "OK");
}

void handle_ntp_server() {   
    uint8_t oldtypeSync = typeSync;           
    sNtpServerName = HTTP.arg("ntpserver").c_str(); 
    typeSync = HTTP.arg("use_sync").toInt();
#if USE_GPS != true
    if (typeSync == 2) {//GPS but no
        typeSync = oldtypeSync;
    }
#endif  
    saveConfig();
    if (typeSync == 1) { //NTP
        //myESPTime.setTimeParam(useRTC, timezone, isDayLightSaving, sNtpServerName);
        //myESPTime.timeSynch();
        myESPTime.begin(timezone, isDayLightSaving, sNtpServerName, sNtpServerName2, sNtpServerName3, false, false);
    }
#if USE_GPS == true
    if (typeSync == 2) {//GPS
        timeSynchGPS(0, 0);
    }
#endif
    Serial.println("sNtpServerName: " + sNtpServerName + ", typeSync: " + typeSync);
    HTTP.send(200, "text/plain", "OK");
}

void handle_Time() {
    if (typeSync == 1) { //NTP
        myESPTime.timeSynch();
    }
#if USE_GPS == true
    if (typeSync == 2) {//GPS
        timeSynchGPS(0, 0);
    }
#endif
    HTTP.send(200, "text/plain", "OK"); 
}

void handle_TimeNew() {
    struct tm timeman;
    time_t epoch_time;
    uint8_t new_h = HTTP.arg("time_h").toInt(); 
    uint8_t new_m = HTTP.arg("time_m").toInt();
    uint8_t new_s = HTTP.arg("time_s").toInt();
    uint8_t new_day = HTTP.arg("date_day").toInt(); 
    uint8_t new_month = HTTP.arg("date_month").toInt();
    uint16_t new_year = HTTP.arg("date_year").toInt();    
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
    //myESPTime.setTimeParam(useRTC, timezone, isDayLightSaving, sNtpServerName);
    //myESPTime.setTimeRTC(epoch_time);
    saveConfig();
    //myESPTime.timeSynch();
    //sMode = 9;
    HTTP.send(200, "text/plain", "OK"); 
}

void handle_LedSetup() {               
    speedTicker = HTTP.arg("speedTicker").toInt(); 
    brightd = HTTP.arg("brightd").toInt();
    brightn = HTTP.arg("brightn").toInt();
    HTTP.arg("brauto").toInt()==1?brauto=true:brauto=false;
    dmodefrom = HTTP.arg("dmodefrom").toInt();
    dmodeto = HTTP.arg("dmodeto").toInt();
    fontUsed = HTTP.arg("fontUsed").toInt();
    saveConfig();
    Serial.print("speedTicker ");Serial.println(speedTicker); Serial.print("brightd ");Serial.println(brightd);Serial.print("brightn ");Serial.println(brightn);
    Serial.print("dmodefrom ");Serial.println(dmodefrom);Serial.print("dmodeto ");Serial.println(dmodeto);
    HTTP.send(200, "text/plain", "OK");
}

void handle_Weather() {
  weatherHost = HTTP.arg("weatherHost").c_str();
  cityID = HTTP.arg("city_code").c_str(); 
  weatherKey = HTTP.arg("w_api").c_str();
  saveConfig();                
  //getWeather(1000);
  Serial.println("weatherHost: " + weatherHost + ", cityID: " + cityID + ", weatherKey: " + weatherKey);
  HTTP.send(200, "text/plain", "OK"); 
}

void handle_weather_update() {
  if(HTTP.arg("update") == "ok") {
    //if (isLedWeather & dispStat) getWeather(0, 250);
    HTTP.send(200, "text/plain", "OK");
  }
}

void handle_screenoption() {
    global_start = HTTP.arg("global_start").toFloat(); global_stop = HTTP.arg("global_stop").toFloat();
    (HTTP.arg("isBigClock").toInt()==1 ?  isBigClock = true : isBigClock = false);
    clockBigFrom = HTTP.arg("clockBigFrom").toFloat(); clockBigTo = HTTP.arg("clockBigTo").toFloat();
    (HTTP.arg("showSec").toInt()==1 ?  showSec = true : showSec = false);
    backgroundLayer.fillScreen(BLACK); //Clear Sec
    backgroundLayer.swapBuffers();
    saveConfig();
    HTTP.send(200, "text/plain", "OK");
}

void handle_setcolor_z1() {
    colorZ1R = HTTP.arg("colorZ1R").toInt(); colorZ1G = HTTP.arg("colorZ1G").toInt(); colorZ1B = HTTP.arg("colorZ1B").toInt();
    colorZ1.red = colorZ1R; colorZ1.green = colorZ1G; colorZ1.blue = colorZ1B;
    saveConfig();
    HTTP.send(200, "text/plain", "OK");       
}

void handle_set_z2() {//for ZONE 2
    global_start = HTTP.arg("global_start").toFloat(); global_stop = HTTP.arg("global_stop").toFloat();
    if (HTTP.arg("isSecZ2").toInt()==1) dispStatZ2 |= isSecZ2; else dispStatZ2 &= ~isSecZ2;
    if (HTTP.arg("isTempZ2").toInt()==1) dispStatZ2 |= isTempZ2; else dispStatZ2 &= ~isTempZ2;
    if (HTTP.arg("isWindZ2").toInt()==1) dispStatZ2 |= isWindZ2; else dispStatZ2 &= ~isWindZ2;
    saveConfig();                 
    Serial.print("dispStatZ2 ");Serial.println(dispStatZ2, BIN);
    HTTP.send(200, "text/plain", "OK"); 
}

void handle_setcolor_z2() {
    colorZ2R = HTTP.arg("colorZ2R").toInt(); colorZ2G = HTTP.arg("colorZ2G").toInt(); colorZ2B = HTTP.arg("colorZ2B").toInt();
    colorZ2.red = colorZ2R; colorZ2.green = colorZ2G; colorZ2.blue = colorZ2B;
    saveConfig();
    HTTP.send(200, "text/plain", "OK");       
}

void handle_set_z3() { //for ZONE 3
    global_start = HTTP.arg("global_start").toFloat(); global_stop = HTTP.arg("global_stop").toFloat();
    if (HTTP.arg("isLedDateZ3").toInt()==1) dispStatZ3 |= isLedDateZ3; else dispStatZ3 = dispStatZ3 & ~isLedDateZ3;
    dateFromZ3 = HTTP.arg("dateFromZ3").toFloat(); dateToZ3 = HTTP.arg("dateToZ3").toFloat();
    HTTP.arg("isCrLineDateZ3").toInt()==1?isCrLineDateZ3=true:isCrLineDateZ3=false;
    if (HTTP.arg("isLedWeatherZ3").toInt()==1) dispStatZ3 |= isLedWeatherZ3; else dispStatZ3 &= ~isLedWeatherZ3;
    if (HTTP.arg("isLedForecastZ3").toInt()==1) dispStatZ3 |= isLedForecastZ3; else dispStatZ3 &= ~isLedForecastZ3;
    if (HTTP.arg("isNews").toInt()==1) dispStatZ3 |= isNews; else dispStatZ3 &= ~isNews;
    weathFromZ3 = HTTP.arg("weathFromZ3").toFloat(); weathToZ3 = HTTP.arg("weathToZ3").toFloat();
    fcastFromZ3 = HTTP.arg("fcastFromZ3").toFloat(); fcastToZ3 = HTTP.arg("fcastToZ3").toFloat();
    newsFromZ3 = HTTP.arg("newsFromZ3").toFloat(); newsToZ3 = HTTP.arg("newsToZ3").toFloat();
    strText[0] = HTTP.arg("ledText0").c_str(); // 
    strText[1] = HTTP.arg("ledText1").c_str(); // 
    strText[2] = HTTP.arg("ledText2").c_str(); // 
    strText[3] = HTTP.arg("ledText3").c_str(); //
    if (HTTP.arg("isTxtOn0").toInt()==1) dispStatZ3 |= isTxtOn0; else dispStatZ3 = dispStatZ3 & ~isTxtOn0;
    if (HTTP.arg("isTxtOn1").toInt()==1) dispStatZ3 |= isTxtOn1; else dispStatZ3 = dispStatZ3 & ~isTxtOn1;
    if (HTTP.arg("isTxtOn2").toInt()==1) dispStatZ3 |= isTxtOn2; else dispStatZ3 = dispStatZ3 & ~isTxtOn2;
    if (HTTP.arg("isTxtOn3").toInt()==1) dispStatZ3 |= isTxtOn3; else dispStatZ3 = dispStatZ3 & ~isTxtOn3;
    txtFrom[0] = HTTP.arg("txtFrom0").toFloat();
    txtFrom[1] = HTTP.arg("txtFrom1").toFloat();
    txtFrom[2] = HTTP.arg("txtFrom2").toFloat();
    txtFrom[3] = HTTP.arg("txtFrom3").toFloat();
    txtTo[0] = HTTP.arg("txtTo0").toFloat();
    txtTo[1] = HTTP.arg("txtTo1").toFloat();
    txtTo[2] = HTTP.arg("txtTo2").toFloat();
    txtTo[3] = HTTP.arg("txtTo3").toFloat();
    HTTP.arg("isCrLine0").toInt()==1?isCrLine[0]=true:isCrLine[0]=false;
    HTTP.arg("isCrLine1").toInt()==1?isCrLine[1]=true:isCrLine[1]=false;
    HTTP.arg("isCrLine2").toInt()==1?isCrLine[2]=true:isCrLine[2]=false;
    HTTP.arg("isCrLine3").toInt()==1?isCrLine[3]=true:isCrLine[3]=false;
    saveConfig();                 
    Serial.print("strText0 ");Serial.println(strText[0]);
    Serial.print("strText1 ");Serial.println(strText[1]);
    Serial.print("strText2 ");Serial.println(strText[2]);
    Serial.print("strText3 ");Serial.println(strText[3]);
    Serial.print("dispStatZ3 ");Serial.println(dispStatZ3, BIN);
    //Serial.print("isTxtOn0 ");Serial.println(isTxtOn0);Serial.println(txtFrom0);Serial.println(txtTo0);Serial.println(isCrLine0);
    //Serial.print("isTxtOn1 ");Serial.println(isTxtOn1);Serial.println(txtFrom1);Serial.println(txtTo1);Serial.println(isCrLine1);
    //Serial.print("isTxtOn2 ");Serial.println(isTxtOn2);Serial.println(txtFrom2);Serial.println(txtTo2);Serial.println(isCrLine2);
    //Serial.print("isTxtOn3 ");Serial.println(isTxtOn3);Serial.println(txtFrom3);Serial.println(txtTo3);Serial.println(isCrLine3);
    HTTP.send(200, "text/plain", "OK"); 
}

void handle_setcolor_z3() {
    colorZ3R = HTTP.arg("colorZ3R").toInt(); colorZ3G = HTTP.arg("colorZ3G").toInt(); colorZ3B = HTTP.arg("colorZ3B").toInt();
    colorZ3.red = colorZ3R; colorZ3.green = colorZ3G; colorZ3.blue = colorZ3B;
    saveConfig();
    HTTP.send(200, "text/plain", "OK");       
}

void handle_Setnews() {
  newsAPI = HTTP.arg("newsAPI").c_str();
  saveConfig();                
  Serial.println("newsAPI: " + newsAPI);
  HTTP.send(200, "text/plain", "OK"); 
}

void handle_newsUpdate() {
  if(HTTP.arg("update") == "ok") {
    if (isNews & dispStatZ3) strNews = myNews.getNewsWiFiClient(&lang);
    //strNews = myNews.getNewsHTTPClient(&lang);
    Serial.print("strNews: "); Serial.println(strNews);
    HTTP.send(200, "text/plain", "OK");
  }
}

void handle_Restart() {
  String restart = HTTP.arg("device");       
  if (restart == "ok") {                      
    HTTP.send(200, "text/plain", "Reset OK");
    ESP.restart();                               
  }
  else {                                       
    HTTP.send(200, "text/plain", "No Reset"); 
  }
}

void handle_resetConfig() {
  String restart = HTTP.arg("device");
  if(restart == "ok") {
    //SPIFFS.format();
    SPIFFS.remove("/myconfig.json");
    Serial.println("ESP erase Config file");
    delay(3000);
    HTTP.send(200, "text/plain", "OK");
    delay(3000);
    ESP.restart();
  }
}

bool handleFileRead(String path) {
    if (path.endsWith("/")) path += "index.html";
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
        if (SPIFFS.exists(pathWithGz))
        path += ".gz";
        File file = SPIFFS.open(path, "r");
        size_t sent = HTTP.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

String getContentType(String filename) {
    if (HTTP.hasArg("download")) return "application/octet-stream";
    else if (filename.endsWith(".htm")) return "text/html";
    else if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".json")) return "application/json";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".png")) return "image/png";
    else if (filename.endsWith(".gif")) return "image/gif";
    else if (filename.endsWith(".jpg")) return "image/jpeg";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".xml")) return "text/xml";
    else if (filename.endsWith(".pdf")) return "application/x-pdf";
    else if (filename.endsWith(".zip")) return "application/x-zip";
    else if (filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}

void init_SSDP(void) {
    HTTP.on("/description.xml", HTTP_GET, []() {
        SSDP.schema(HTTP.client());
    });
    SSDP.setSchemaURL("description.xml");
    SSDP.setHTTPPort(80);
    SSDP.setName(SSDP_Name);
    SSDP.setSerialNumber("001788102201");
    SSDP.setURL("index.html");
    SSDP.setModelName(nName);
    SSDP.setModelDescription("This device can be controled by WiFi");
    SSDP.setModelNumber(nVersion);
    SSDP.setModelURL("http://" + (WiFi.status() != WL_CONNECTED ? WiFi.softAPIP().toString() : WiFi.localIP().toString()));
    SSDP.setManufacturer("LIGHTWELL");
    SSDP.setManufacturerURL("https://diy-led.eu");
    SSDP.setDeviceType("upnp:rootdevice");
    SSDP.setServerName("SSDPServer/1.0");
    //set UUID, you can use https://www.uuidgenerator.net/
        //use 38323636-4558-4dda-9188-cda0e6 + 4 last bytes of mac address if not set
        //use SSDP.setUUID("daa26fa3-d2d4-4072-bc7a-a1b88ab4234a", false); for full UUID
    SSDP.setUUID("daa26fa3-d2d4-4072-bc7a");
    //SSDP.setURL("/");
    SSDP.begin();
}