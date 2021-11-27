uint8_t portHTTP   = 80;

String GetRequestFromYoutube(String &request);

bool obtain_channel_stats() {
    bool changeStat = false;
    String command = "/youtube/?type=channels&part=statistics&key=" + googleAPI + "&id=" + ytChannelID;
    //Serial.print("command "); Serial.println(command); 
    String response = GetRequestFromYoutube(command);   
    //Serial.print("goodAnswerNews "); Serial.println(response);    
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 1000;
    DynamicJsonDocument root(capacity);
    DeserializationError error = deserializeJson(root, response);
    if (error) {
        Serial.print(F("Json parsing failed! ")); Serial.print(F("deserializeJson() failed with code ")); Serial.println(error.c_str());    
        //return; 
    }
    else  {
        JsonObject current                = root["items"][0]["statistics"];  
        channelStats.subscriberCount               = current["subscriberCount"];
        channelStats.viewCount                     = current["viewCount"];
        channelStats.commentCount                  = current["commentCount"];
        channelStats.hiddenSubscriberCount         = current["hiddenSubscriberCount"];
        channelStats.videoCount                    = current["videoCount"];
        if (oldchannelStats.subscriberCount != channelStats.subscriberCount || oldchannelStats.viewCount != channelStats.viewCount \
        || oldchannelStats.commentCount != channelStats.commentCount) {
          changeStat = true;
          //oldchannelStats = channelStats;
        }
        Serial.println("View Count =       " + String(channelStats.viewCount));
        Serial.println("Subscriber Count = " + String(channelStats.subscriberCount));
        Serial.println("commentCount =     " + String(channelStats.commentCount));
        Serial.println("hiddenSubscriberCount =       " + String(channelStats.hiddenSubscriberCount));
        Serial.println("videoCount =       " + String(channelStats.videoCount));
    }  
    return changeStat;
}

bool obtain_video_stats() {
    bool changeStat = false;
    String command = "/youtube/?type=videos&part=statistics&key=" + googleAPI + "&id=" + ytVideoID;
    //Serial.print("command "); Serial.println(command); 
    String response = GetRequestFromYoutube(command);   
    //Serial.print("goodAnswerNews "); Serial.println(response);    
    const size_t capacity = JSON_ARRAY_SIZE(5) + 1000;
    DynamicJsonDocument root(capacity);
    DeserializationError error = deserializeJson(root, response);
    if (error) {
        Serial.print(F("Json parsing failed! ")); Serial.print(F("deserializeJson() failed with code ")); Serial.println(error.c_str());    
        //return; 
    }
    else  {
        JsonObject current                = root["items"][0]["statistics"];  
        videoStats.viewCount                  = current["viewCount"];
        videoStats.likeCount                  = current["likeCount"];
        videoStats.dislikeCount               = current["dislikeCount"];
        videoStats.favoriteCount              = current["favoriteCount"];
        videoStats.commentCount               = current["commentCount"];
        if (oldvideoStats.viewCount != videoStats.viewCount || oldvideoStats.likeCount != videoStats.likeCount \
        || oldvideoStats.dislikeCount != videoStats.dislikeCount || oldvideoStats.commentCount != videoStats.commentCount) {
          changeStat = true;
          //oldvideoStats = videoStats;
        }
        Serial.println("View Count =       " + String(videoStats.viewCount));
        Serial.println("videoStats.likeCount = " + String(videoStats.likeCount));
        Serial.println("dislikeCount =     " + String(videoStats.dislikeCount));
        Serial.println("videoStats.favoriteCount =       " + String(videoStats.favoriteCount));
        Serial.println("videoStats.commentCount =       " + String(videoStats.commentCount));
    } 
    return changeStat; 
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
  if (ESPclient.connect(host.c_str(), portHTTP)) {
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