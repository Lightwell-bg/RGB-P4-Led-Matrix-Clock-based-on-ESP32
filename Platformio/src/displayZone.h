void displayZ1(struct tm &tnow) {
    displayTimeSmall();
}

void displayZ2(struct tm &tnow) {
    static uint8_t nStep = 1, oldnStep = 0;
    indexedLayerZ2.setColor(colorZ2);
    //scrollingLayerTextSmall.setColor(colorZ2);
    //indexedLayerZ2.setIndexedColor(1, {colorZ2});
    if (nStep != oldnStep) {/*Serial.print("nStepZ2: "); Serial.println(nStep);*/ oldnStep = nStep;}
    if (nStep == 1) {
        if (dispStatZ2 & isSecZ2) {
            if (displaySecondsZ2(4000)) {nStep++;}
        }
        else nStep++;
    }
    else if (nStep == 2) {
        if (dispStatZ2 & isTempZ2) {
            if (weatherTemp.length() > 0) {if (displayWeatherIcon(weatherIcon, 3000)) {nStep++;}}
            else {if (displayWeatherIcon(255, 3000)) {nStep++;}}
        }
        else nStep++;
    }
    else if (nStep == 3) {
        if ((dispStatZ2 & isTempZ2) && weatherTemp.length() > 0) {
            if (displayTemp(3000)) {nStep++;}
        }
        else nStep++;
    }
    else if (nStep == 4) {
        if (dispStatZ2 & isWindZ2 && weatherSpeed.length() > 0) {
            if (displayWind(3000)) {nStep++;}
        }
        else nStep++;
    }
    else {
        nStep = 1;
    }
    //displayBitmap(2500);
}

void displayZ3(struct tm &tnow) {
    static uint8_t nStep = 1, oldnStep = 0;
    scrollingLayerText.setColor(colorZ3);
    indexedLayerZ3.setColor(colorZ3);
    //if (nStep != oldnStep) {Serial.print("nStepZ3: "); Serial.println(nStep); oldnStep = nStep;}
    if (nStep == 1) {
        if ((dispStatZ3 & isLedDateZ3) && myESPTime.compTimeInt(dateFromZ3, dateToZ3, &tnow)) {
            if (isCrLineDateZ3) {
                String NowDate = String(tnow.tm_mday)+" "+month_table[lang][tnow.tm_mon] +" "+String(tnow.tm_year + 1900) + ", " + day_table[lang][tnow.tm_wday];
                NowDate.toLowerCase();
                if (displayScrollText(NowDate)) {nStep++;}
            }
            else {
                String NowDate = String(tnow.tm_mday) + " "+ monthS_table[lang][tnow.tm_mon] + "," + dayS_table[lang][tnow.tm_wday];
                if (displayText(NowDate, 2000, FreeSansBold7pt8b)) {nStep++;}
            }
        }
        else nStep++;
    }
    if (nStep == 2) {
        if ((dispStatZ3 & isTxtOn0) && myESPTime.compTimeInt(txtFrom[0], txtTo[0], &tnow)) {
            if (isCrLine[0]) {
                if (displayScrollText(strText[0])) {nStep++;}
            }
            else {
                if (displayText(strText[0], 2000)) {nStep++;}
            }            
        }
        else nStep++;
    }
    else if (nStep == 3) {
        if ((dispStatZ3 & isTxtOn1) && myESPTime.compTimeInt(txtFrom[1], txtTo[1], &tnow)) {
            if (isCrLine[1]) {
                if (displayScrollText(strText[1])) nStep++;
            }
            else {
                if (displayText(strText[1], 2000)) nStep++;
            }
        }
        else nStep++;
    }
    else if (nStep == 4) {
        if ((dispStatZ3 & isTxtOn2) && myESPTime.compTimeInt(txtFrom[2], txtTo[2], &tnow)) {
            if (isCrLine[2]) {
                if (displayScrollText(strText[2])) nStep++;
            }
            else {
                if (displayText(strText[2], 2000)) nStep++;
            }
        }
        else nStep++;
    }
    else if (nStep == 5) {
        if ((dispStatZ3 & isTxtOn3) && myESPTime.compTimeInt(txtFrom[3], txtTo[3], &tnow)) {
            if (isCrLine[3]) {
                if (displayScrollText(strText[3])) nStep++;
            }
            else {
                if (displayText(strText[3], 2000)) nStep++;
            }
        }
        else nStep++;
    }
    else if (nStep == 6) {
        if ((isLedWeatherZ3 & dispStatZ3) && myESPTime.compTimeInt(weathFromZ3, weathToZ3, &tnow)) {
            if (displayScrollText(strWeather)) nStep++;
        }
        else nStep++;
    }
    else if (nStep == 7) {
        if ((isLedForecastZ3 & dispStatZ3) && myESPTime.compTimeInt(fcastFromZ3, fcastToZ3, &tnow)) {
            if (displayScrollText(strWeatherFcast)) nStep++;
        }
        else nStep++;
    }
    else if (nStep == 8) {
        if ((isNews & dispStatZ3) && myESPTime.compTimeInt(newsFromZ3, newsToZ3, &tnow)) {
            if (displayScrollText(strNews)) nStep++;
        }
        else nStep++;
    }
    else {
        nStep = 1;
    }
}