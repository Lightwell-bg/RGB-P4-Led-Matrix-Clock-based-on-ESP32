void displayYoutubeIcon(uint8_t codeIcon);

void handleChannel() {
    //Serial.println("handleChannel");
    if (oldchannelStats.subscriberCount != channelStats.subscriberCount) {
        Serial.println("!!!subscriberCount dif =       " + String(channelStats.subscriberCount - oldchannelStats.subscriberCount));
        curTypeDisp = CHANNEL_SUBCR;
        modeDisplay = 31;
    }
    if (oldchannelStats.viewCount != channelStats.viewCount) {
        Serial.println("!!!channelStats.viewCount dif =       " + String(channelStats.viewCount - oldchannelStats.viewCount));
    }
    if (oldchannelStats.commentCount != channelStats.commentCount) {
        Serial.println("!!!channelStats.commentCount dif =       " + String(channelStats.commentCount - oldchannelStats.commentCount));
    }
    oldchannelStats = channelStats;
}

void handleVideo() {
    if (oldvideoStats.viewCount != videoStats.viewCount) {
        Serial.println("!!!videoStats.viewCount dif =       " + String(videoStats.viewCount - oldvideoStats.viewCount));
    }
    if (oldvideoStats.likeCount != videoStats.likeCount) {
        Serial.println("!!!videoStats.likeCount dif =       " + String(videoStats.likeCount - oldvideoStats.likeCount));
        curTypeDisp = VIDEO_LIKE;
        modeDisplay = 31;
    }
    if (oldvideoStats.dislikeCount != videoStats.dislikeCount) {
        Serial.println("!!!videoStats.dislikeCount dif =       " + String(videoStats.dislikeCount - oldvideoStats.dislikeCount));
    } 
    if (oldvideoStats.commentCount != videoStats.commentCount) {
        Serial.println("!!!videoStats.commentCount dif =       " + String(videoStats.commentCount - oldvideoStats.commentCount));
        curTypeDisp = VIDEO_COMMENT;
        modeDisplay = 31;
    }    
    oldvideoStats = videoStats;
}

bool diplayYouTStatZ3() {
    static uint8_t nStep = 1, oldnStep = 0;
    scrollingLayerText.setColor(colorZ3);
    if (nStep == 1) { //Channel
        if (channelOn) {
            String statChannel = youtube_table[lang][0] + String(channelStats.subscriberCount) + \
            youtube_table[lang][1] + String(channelStats.viewCount); // +  \
            //youtube_table[lang][2] + String(channelStats.commentCount);
            if (displayScrollText(statChannel)) {nStep++;};
            return false;
        }
        else nStep++;
        return false;
    }
    else if (nStep == 2) {//Video
        if (videoOn) {
            String statVideo = youtube_table[lang][3] + String(videoStats.likeCount) + \
            youtube_table[lang][1] + String(videoStats.viewCount) + \
            youtube_table[lang][2] + String(videoStats.commentCount);
            if (displayScrollText(statVideo)) {nStep++;};
            return false;
        }
        else nStep++;
        return false;
    }
    else {
        nStep = 1;
        return true;
    }
}

bool displayYTAlarm(statTypeDisp typeDisp, uint32_t Per = 5000) {
    static bool firstStep = true;
    static uint32_t curDur, chIcon;
    static uint8_t nIcon;
    String valDiff, valTotal;
    if (firstStep) {
        backgroundLayer.fillScreen(BLACK);
        backgroundLayer.swapBuffers();
        indexedLayerZ2.fillScreen(BLACK);
        if (typeDisp == VIDEO_LIKE) {
            nIcon = 1;
            valDiff = "+2";
            valTotal = String(videoStats.likeCount);
        }
        else if (typeDisp == VIDEO_COMMENT) {
            nIcon = 3;
            valDiff = "+5";
            valTotal = String(videoStats.commentCount);
        }
        else if (typeDisp == CHANNEL_SUBCR) {
            nIcon = 5;
            valDiff = "+1";
            valTotal = String(channelStats.subscriberCount);
        }
        else {;}
        uint16_t w, h;
        indexedLayerZ2.setFont(&FranklinGothicMediumCond9pt7b);
        indexedLayerZ2.getDimensionsOfPrintedString(valDiff.c_str(), &w, &h);
        indexedLayerZ2.drawString(matrix.getScreenWidth()-w-2, 0, 1, valDiff.c_str());
        indexedLayerZ2.setFont(&FranklinGothicMediumCond7pt7b);
        indexedLayerZ2.getDimensionsOfPrintedString(valTotal.c_str(), &w, &h);
        indexedLayerZ2.drawString((matrix.getScreenWidth()-w)/*/2*/, 16, 1, valTotal.c_str());
        indexedLayerZ2.swapBuffers();
        displayYoutubeIcon(nIcon);
        firstStep = false; 
        curDur = millis();
        return false;
    }
    else {
        if (millis() - curDur > Per) {
            indexedLayerZ2.fillScreen(BLACK);
            indexedLayerZ2.swapBuffers();
            backgroundLayer.fillScreen(BLACK);
            backgroundLayer.swapBuffers();
            firstStep = true;
            return true;
        }
        else {
            if (millis() - chIcon > 500) {
                backgroundLayer.fillScreen(BLACK);
                backgroundLayer.swapBuffers();
                displayYoutubeIcon(nIcon);
                if (typeDisp == VIDEO_LIKE) {if (nIcon == 1) nIcon = 2; else nIcon = 1;}
                else if (typeDisp == VIDEO_COMMENT) {if (nIcon == 3) nIcon = 4; else nIcon = 3;}
                else if (typeDisp == CHANNEL_SUBCR) {if (nIcon == 5) nIcon = 6; else nIcon = 5;}
                else {;}
                chIcon = millis();
            }
            return false;
        }
  }
}

void displayYoutubeIcon(uint8_t codeIcon) {
    uint8_t x, y;
    if (codeIcon % 2 == 0) {
        x = 9; y = 9;
    }
    else {
        x = 1; y = 0;
    }
    //backgroundLayer.fillScreen(BLACK);
    //backgroundLayer.swapBuffers();
    switch (codeIcon)  {
    case 1:
        drawBitmap(x, y, (const gimp32x32bitmap*)&heart32);
        break;
    case 2:
        drawBitmap(x, y, (const gimp32x32bitmap*)&heart16);
        break;
    case 3:
        drawBitmap(x, y, (const gimp32x32bitmap*)&bell32);
        break;
    case 4:
        drawBitmap(x, y, (const gimp32x32bitmap*)&bell16);
        break;
    case 5:
        drawBitmap(x, y, (const gimp32x32bitmap*)&youtube32);
        break;
    case 6:
        drawBitmap(x, y, (const gimp32x32bitmap*)&youtube16);
        break;
    default:
        drawBitmap(x, y, (const gimp32x32bitmap*)&bell32);
        break;
    }
    backgroundLayer.swapBuffers();
}