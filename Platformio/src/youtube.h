//#include <WiFiClientSecure.h>

String API_KEY = "AIzaSyAymdsOcVX7yszuSE6_Ws4fIzszK1j1mWk";  // API KEY выданный Google 
String CHANNEL_ID = "UCb1gig2zaV3LHLeQ2NlCvPg"; // ID YouTube канала
String host = "api.diy-led.eu";

struct channelStatistics{
  long viewCount;
  long commentCount;
  long subscriberCount;
  bool hiddenSubscriberCount;
  long videoCount;
};
int port   = 80;
channelStatistics channelStats;

String GetRequestFromYoutube(String &request);

void obtain_subscriber_stats() {
    String command = "/channels/?part=statistics&key=" + API_KEY + "&id=" + CHANNEL_ID;
    //Serial.print("command "); Serial.println(command); 
    String response = GetRequestFromYoutube(command);   
    //Serial.print("goodAnswerNews "); Serial.println(response);    
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 1000;
    DynamicJsonDocument root(capacity);
    DeserializationError error = deserializeJson(root, response);
    if (error) {
        Serial.print(F("Json parsing failed! ")); Serial.print(F("deserializeJson() failed with code ")); Serial.println(error.c_str());    
        return; 
    }
    else  {
        JsonObject current                = root["items"][0]["statistics"];  
        uint32_t subscriberCount               = current["subscriberCount"];
        uint32_t viewCount                     = current["viewCount"];
        uint32_t commentCount                  = current["commentCount"];
        uint32_t hiddenSubscriberCount         = current["hiddenSubscriberCount"];
        uint32_t videoCount                    = current["videoCount"];
        channelStats.viewCount             = viewCount;
        channelStats.subscriberCount       = subscriberCount;
        channelStats.commentCount          = commentCount;
        channelStats.hiddenSubscriberCount = hiddenSubscriberCount;
        channelStats.videoCount            = videoCount;
    }  
      Serial.println("View Count =       " + String(channelStats.viewCount));
      Serial.println("Subscriber Count = " + String(channelStats.subscriberCount));
      Serial.println("commentCount =     " + String(channelStats.commentCount));
      Serial.println("videoCount =       " + String(channelStats.videoCount));
}

String GetRequestFromYoutube(String &request) {
  String headers, body = "";
  bool Headers    = false;
  bool currentLineIsBlank = true;
  uint16_t  MessageLength = 1000;
  //request = "GET " + request + " HTTP/1.1\r\nHost: " + String(host) + "\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:36.0) Gecko/20100101 Firefox/36.0\r\nConnection: close\r\n\r\n";
  request = "GET " + request + " HTTP/1.1\r\nHost: " + String(host);
  request += "\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.159 Safari/537.36 OPR/78.0.4093.184\r\nConnection: close\r\n\r\n";
  Serial.print("request "); Serial.println(request);
  if (ESPclient.connect(host.c_str(), port)) {
    Serial.println(".... connected to server");
    char c;
    uint16_t ch_count=0;
    ESPclient.println(request);
    uint32_t now   = millis();
    while (millis()-now < 1500) {
      while (ESPclient.available()) {
        char c = ESPclient.read();
        if(!Headers){
          if (currentLineIsBlank && c == '\n') {
            Headers = true;
          }
          else{
            headers = headers + c;
          }
        } else {
          if (ch_count < MessageLength)  {
            body = body+c;
            ch_count++;
          }
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    ESPclient.stop();
  }
  else {ESPclient.stop(); Serial.println(".... CANT connected to server");}
  return body;
}