bool saveConfig();

bool loadConfig() {
    if(!SPIFFS.exists(filePath)) {
        Serial.println(F("Failed to open config file"));
        saveConfig();   
        return false;
    }
    File configFile = SPIFFS.open(filePath, "r");   
    size_t size = configFile.size();   
    if (size > 2048) {  
        Serial.print(F("Config file size is too large: ")); Serial.println(size);
        configFile.close();
        return false;
    }
    jsonConfig = configFile.readString(); 
    configFile.close();
    DynamicJsonDocument jsonDoc(5096); //4096
    DeserializationError error = deserializeJson(jsonDoc, jsonConfig);
    if (error) {
      Serial.print(F("loadConfig() deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return false;
    }   
    JsonObject root = jsonDoc.as<JsonObject>();   
    timezone = root["timezone"].as<int>();               
    isDayLightSaving = root["isDayLightSaving"];
    sNtpServerName = root["ntp_server"].as<String>(); 
    typeSync = root["typeSync"];   
    lang = root["lang"].as<int>();
    fontUsed = root["fontUsed"];
    brightd = root["brightd"]; brightn = root["brightn"];
    brauto = root["brauto"];
    dmodefrom = root["dmodefrom"]; dmodeto = root["dmodeto"];             
    speedTicker = root["speedTicker"]; 
    global_start = root["global_start"]; global_stop = root["global_stop"];
    speedTicker = root["speedTicker"];
    modeDisplay = root["modeDisplay"];
    showSec = root["showSec"]; //ZONE BIG
    isBigClock = root["isBigClock"];
    clockBigFrom = root["clockBigFrom"]; clockBigTo = root["clockBigTo"];
    dispStatZ3 = root["dispStatZ3"]; //ZONE 3
    dateFromZ3 = root["dateFromZ3"]; dateToZ3 = root["dateToZ3"];
    isCrLineDateZ3 = root["isCrLineDateZ3"]; 
    weathFromZ3 = root["weathFromZ3"]; weathToZ3 = root["weathToZ3"];
    fcastFromZ3 = root["fcastFromZ3"]; fcastToZ3 = root["fcastToZ3"];
    newsFromZ3 = root["newsFromZ3"]; newsToZ3 = root["newsToZ3"];
    strText[0] = root["ledText0"].as<String>();
    strText[1] = root["ledText1"].as<String>();
    strText[2] = root["ledText2"].as<String>();
    strText[3] = root["ledText3"].as<String>();
    isCrLine[0] = root["isCrLine0"]; isCrLine[1] = root["isCrLine1"]; isCrLine[2] = root["isCrLine2"]; isCrLine[3] = root["isCrLine3"];
    txtFrom[0] = root["txtFrom0"]; txtFrom[1] = root["txtFrom1"]; txtFrom[2] = root["txtFrom2"]; txtFrom[3] = root["txtFrom3"];
    txtTo[0] = root["txtTo0"]; txtTo[1] = root["txtTo1"]; txtTo[2] = root["txtTo2"]; txtTo[3] = root["txtTo3"];
    colorZ3R = root["colorZ3R"]; colorZ3G = root["colorZ3G"]; colorZ3B = root["colorZ3B"];
    colorZ3.red = colorZ3R; colorZ3.green = colorZ3G; colorZ3.blue = colorZ3B;
    //ZONE 1
    clockFromZ1 = root["clockFromZ1"]; clockToZ1 = root["clockToZ1"];
    colorZ1R = root["colorZ1R"]; colorZ1G = root["colorZ1G"]; colorZ1B = root["colorZ1B"];
    colorZ1.red = colorZ1R; colorZ1.green = colorZ1G; colorZ1.blue = colorZ1B;
        //ZONE 2
    dispStatZ2 = root["dispStatZ2"];
    colorZ2R = root["colorZ2R"]; colorZ2G = root["colorZ2G"]; colorZ2B = root["colorZ2B"];
    colorZ2.red = colorZ2R; colorZ2.green = colorZ2G; colorZ2.blue = colorZ2B;

    weatherHost = root["weatherHost"].as<String>();
    weatherKey = root["weatherKey"].as<String>();
    cityID = root["cityID"].as<String>();
    newsAPI = root["newsAPI"].as<String>();
    return true;
}

bool saveConfig() {
    DynamicJsonDocument jsonDoc(5096); //4096
    DeserializationError error = deserializeJson(jsonDoc, jsonConfig);
    if (error) {
        Serial.print(F("saveConfig() deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return false;
    }     
    JsonObject json = jsonDoc.as<JsonObject>();  
    json["timezone"] = timezone;
    json["isDayLightSaving"] = isDayLightSaving;
    json["ntp_server"] = sNtpServerName;
    json["typeSync"] = typeSync;
    json["lang"] = lang;
    json["cVersion"] = cVersion;
    json["fontUsed"] = fontUsed; 
    json["brightd"] = brightd; json["brightn"] = brightn;
    json["brauto"] = brauto;
    json["dmodefrom"] = dmodefrom; json["dmodeto"] = dmodeto;
    json["speedTicker"] = speedTicker;
    json["global_start"] = global_start; json["global_stop"] = global_stop;
    json["speedTicker"] = speedTicker;
    json["modeDisplay"] = modeDisplay;
    json["showSec"] = showSec; //ZONE BIG
    json["isBigClock"] = isBigClock;
    json["clockBigFrom"] = clockBigFrom; json["clockBigTo"] = clockBigTo;
    json["dispStatZ3"] = dispStatZ3; //ZONE 3
    json["dateFromZ3"] = dateFromZ3; json["dateToZ3"] = dateToZ3;
    json["isCrLineDateZ3"] = isCrLineDateZ3;
    json["ledText0"] = strText[0];
    json["ledText1"] = strText[1];
    json["ledText2"] = strText[2];
    json["ledText3"] = strText[3];
    json["isCrLine0"] = isCrLine[0]; json["isCrLine1"] = isCrLine[1]; json["isCrLine2"] = isCrLine[2]; json["isCrLine3"] = isCrLine[3];
    json["txtFrom0"] = txtFrom[0]; json["txtFrom1"] = txtFrom[1]; json["txtFrom2"] = txtFrom[2]; json["txtFrom3"] = txtFrom[3];
    json["txtTo0"] = txtTo[0]; json["txtTo1"] = txtTo[1]; json["txtTo2"] = txtTo[2]; json["txtTo3"] = txtTo[3];
    json["weathFromZ3"] = weathFromZ3; json["weathToZ3"] = weathToZ3;
    json["fcastFromZ3"] = fcastFromZ3; json["fcastToZ3"] = fcastToZ3;
    json["newsFromZ3"] = newsFromZ3; json["newsToZ3"] = newsToZ3;
    json["colorZ3R"] = colorZ3R; json["colorZ3G"] = colorZ3G;json["colorZ3B"] = colorZ3B;
    //ZONE 1
    json["clockFromZ1"] = clockFromZ1; json["clockToZ1"] = clockToZ1;
    json["colorZ1R"] = colorZ1R; json["colorZ1G"] = colorZ1G;json["colorZ1B"] = colorZ1B;
    //ZONE 2
    json["dispStatZ2"] = dispStatZ2;
    json["colorZ2R"] = colorZ2R; json["colorZ2G"] = colorZ2G;json["colorZ2B"] = colorZ2B;

    json["weatherHost"] = weatherHost;
    json["weatherKey"] = weatherKey;
    json["cityID"] = cityID;
    json["newsAPI"] = newsAPI;
    serializeJson(json, jsonConfig);
    File configFile = SPIFFS.open(filePath, "w");
    if (!configFile) {
        Serial.println(F("Failed to open config file for writing"));
        configFile.close();
        return false;
    }
    serializeJson(json, configFile);
    configFile.close();
    return true;
}