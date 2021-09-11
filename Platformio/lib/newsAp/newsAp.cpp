#include <Arduino.h>
#include <newsAp.h>

newsAp::newsAp(WiFiClient* client) {
    _clientWiFi = client;
}

newsAp::newsAp(HTTPClient* client) {
    _clientHTTP = client;
}

void newsAp::begin(const char *api_url, const char *api_key) {
    _api_url = api_url;
    _api_key = api_key;
}

String newsAp::getNewsHTTPClient(const uint8_t* lang) {
    String strURL = String("/v2/top-headlines?country="); 
    switch (*lang) {
        case 0: 
        strURL += "ru";
        break;
        case 1:
        strURL += "bg";
        break;
        case 2:
        strURL += "gb";
        break;     
        default:
        strURL += "gb";
        break;
    }  
    strURL += "&pageSize=1&apiKey=" + String(_api_key);
    Serial.println(strURL);
    uint8_t buff[128] = { 0 };
    _clientHTTP->begin(_api_url, 80, strURL);
    int httpCode = _clientHTTP->GET();
    if (httpCode > 0) {
         USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
         if(httpCode == HTTP_CODE_OK) {
             int len = _clientHTTP->getSize(); // get length of document (is -1 when Server sends no Content-Length header)
             //uint8_t buff[128] = { 0 };
             WiFiClient * stream = _clientHTTP->getStreamPtr();
             while(_clientHTTP->connected() && (len > 0 || len == -1)) {
                 size_t size = stream->available();
                 if (size) {
                     // read up to 128 byte
                    int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                    USE_SERIAL.write(buff, c);
                    if (len > 0) {
                            len -= c;
                    }
                 }
                 delay(1);
             }
         }
    }
    else {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", _clientHTTP->errorToString(httpCode).c_str());;
    }
    _clientHTTP->end();

    String answerNews = String((char *) buff);
    Serial.print("\n answerNews "); Serial.println(answerNews);
 
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 2000;
    DynamicJsonDocument root(capacity);
    uint16_t fJson = answerNews.indexOf("status");
    uint16_t lJson = answerNews.lastIndexOf("}");
    String goodAnswerNews = answerNews.substring(fJson - 2, lJson + 1);
    goodAnswerNews.replace("\\\"", "");
    Serial.print("goodAnswerNews "); Serial.println(goodAnswerNews);
    DeserializationError error = deserializeJson(root, goodAnswerNews);
    if (error) {
        Serial.print(F("Json parsing failed! ")); Serial.print(F("deserializeJson() failed with code ")); Serial.println(error.c_str());    
        return ""; 
    }
    JsonObject articles_0 = root["articles"][0];
    String strNews = articles_0["title"].as<String>();
    //Serial.print("strNews: "); Serial.println(strNews);
    yield();
    return strNews;
}

String newsAp::getNewsWiFiClient(const uint8_t* lang) {
    String strURL = String("GET /v2/top-headlines?country="); 
    switch (*lang) {
        case 0: 
        strURL += "ru";
        break;
        case 1:
        strURL += "bg";
        break;
        case 2:
        strURL += "gb";
        break;     
        default:
        strURL += "gb";
        break;
    }  
    strURL += "&pageSize=1&apiKey=" + String(_api_key);
    strURL += " HTTP/1.1";
    strURL += "\r\nHost: " + String(_api_url) + "\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:36.0) Gecko/20100101 Firefox/36.0\r\nConnection: close\r\n\r\n";
    Serial.println(strURL);
    if (_clientWiFi->connect(_api_url, 80)) {
        _clientWiFi->println(strURL);
    }
    else {
        Serial.print(F("Connection weather server failed")); Serial.println(_api_url);
        return ""; 
    }
    //String answerNews = _clientWiFi->readString();
    //Serial.print("answerNews "); Serial.println(answerNews);
    String header, answerNews = "";
    bool Headers    = false;
    bool currentLineIsBlank = true;
    uint16_t MessageLength = 5000;
    char c;
    uint16_t ch_count=0;
    uint32_t now   = millis();
    while (millis() - now < 3000) {
        while (_clientWiFi->available()) {
            char c = _clientWiFi->read();
            if(!Headers) {
                if (currentLineIsBlank && c == '\n') {
                    Headers = true;
                }
                else {
                    header += c;
                }
            }
            else {
                if (ch_count < MessageLength)  {
                    answerNews += c;
                    ch_count++;
                }
            }
            if (c == '\n') {
                currentLineIsBlank = true;
            } 
            else if (c != '\r') {
                currentLineIsBlank = false;
            }
        }
    }
    Serial.println("header---*---- "); Serial.println(header);
    Serial.println("answerNews---*--- "); Serial.println(answerNews);
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 2000;
    DynamicJsonDocument root(capacity);
    uint16_t fJson = answerNews.indexOf("status");
    uint16_t lJson = answerNews.lastIndexOf("}");
    String goodAnswerNews = answerNews.substring(fJson - 2, lJson + 1);
    goodAnswerNews.replace("\\\"", "");
    //Serial.print("goodAnswerNews "); Serial.println(goodAnswerNews);
    DeserializationError error = deserializeJson(root, goodAnswerNews);
    if (error) {
        Serial.print(F("Json parsing failed! ")); Serial.print(F("deserializeJson() failed with code ")); Serial.println(error.c_str());    
        _clientWiFi->stop();
        return ""; 
    }
    _clientWiFi->stop();
    JsonObject articles_0 = root["articles"][0];
    String strNews = articles_0["title"].as<String>();
    //Serial.print("strNews: "); Serial.println(strNews);
    yield();
    return strNews;
}