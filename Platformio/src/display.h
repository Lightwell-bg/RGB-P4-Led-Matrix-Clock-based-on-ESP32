void drawBitmap(int16_t x, int16_t y, const gimp32x32bitmap* bitmap);

void init_display() {
    matrix.addLayer(&backgroundLayer); //BITMAP
    matrix.addLayer(&scrollingLayerText); 
    //matrix.addLayer(&scrollingLayerTextSmall);
    matrix.addLayer(&indexedLayerZ1); 
    matrix.addLayer(&indexedLayerZ1_1);
    matrix.addLayer(&indexedLayerZ2);
    matrix.addLayer(&indexedLayerZ3);
    matrix.begin();
    matrix.setBrightness(defaultBrightness);
    //scrollingLayer00.setOffsetFromTop(defaultScrollOffset);
    backgroundLayer.enableColorCorrection(true);
    backgroundLayer.fillScreen(BLACK);
    uint8_t x = (kMatrixWidth / 2) - (logo_diy.width/2);
    uint8_t y = (kMatrixHeight / 2) - (logo_diy.height/2);
    // to use drawBitmap, must cast the pointer to pixelmatixlogo as (const gimp32x32bitmap*)
    drawBitmap(x,y,(const gimp32x32bitmap*)&logo_diy);
    backgroundLayer.swapBuffers();
    delay(1100);
    backgroundLayer.fillScreen(BLACK);
    backgroundLayer.swapBuffers();
}

void clearAllLayer() {
  backgroundLayer.fillScreen(BLACK);
  backgroundLayer.swapBuffers();
  scrollingLayerText.fillScreen(BLACK);
  scrollingLayerText.swapBuffers();
  //scrollingLayerTextSmall.fillScreen(BLACK);
  //scrollingLayerTextSmall.swapBuffers();
  indexedLayerZ1.fillScreen(BLACK);
  indexedLayerZ1.swapBuffers();
  indexedLayerZ1_1.fillScreen(BLACK);
  indexedLayerZ1_1.swapBuffers();
  indexedLayerZ2.fillScreen(BLACK);
  indexedLayerZ2.swapBuffers();
  indexedLayerZ3.fillScreen(BLACK);
  indexedLayerZ3.swapBuffers();
}

bool displayBigTime(bool showSec = true) {
  static bool showDOT = true, oldShowDot, canClearSec = true;
  static uint32_t lastDot = 0;
  static uint8_t oldMin = 61, oldSec = 61;
  String nTime, nSec;
  indexedLayerZ1.setFont(&FranklinGothicMediumCond15pt7b);
  indexedLayerZ1.setColor(colorZ1);
  indexedLayerZ1_1.setFont(font6x10);
  //indexedLayerZ1_1.setColor({0, 0xff, 0});
  indexedLayerZ1_1.setColor(colorZ1);
  struct tm tnow = myESPTime.getTimeStruct();
  if (tnow.tm_min != oldMin || showDOT != oldShowDot) {
    indexedLayerZ1.fillScreen(0);
    nTime = (tnow.tm_hour<10 ? "0" + String(tnow.tm_hour) : String(tnow.tm_hour))  + (showDOT ? ":" : " ") + (tnow.tm_min<10 ? "0" + String(tnow.tm_min) : String(tnow.tm_min));
    indexedLayerZ1.drawString(1, matrix.getScreenHeight() / 2 - 24, 1, nTime.c_str());
    indexedLayerZ1.swapBuffers();
    if (!showSec) oldShowDot = showDOT;
    oldMin = tnow.tm_min;
  }
  if (showSec) {
    if (tnow.tm_sec != oldSec || showDOT != oldShowDot) {
      indexedLayerZ1_1.fillScreen(0);
      nSec = (showDOT ? ":" : " ") + (tnow.tm_sec<10 ?  + "0" + String(tnow.tm_sec) : String(tnow.tm_sec));
      //nSec = (tnow.tm_sec<10 ? ":0" + String(tnow.tm_sec) : ":" + String(tnow.tm_sec));
      indexedLayerZ1_1.drawString(46, matrix.getScreenHeight() / 2 + 9, 1, nSec.c_str());
      indexedLayerZ1_1.swapBuffers();
      oldShowDot = showDOT;
      oldSec = tnow.tm_sec;
      canClearSec = true;
    }
  }
  else {
    if (canClearSec) {
      indexedLayerZ1_1.fillScreen(0);
      indexedLayerZ1_1.swapBuffers();
      canClearSec = false;
    }
  }

  if (millis() - lastDot >= 750) {
    /*indexedLayerZ1.fillScreen(0);
    nTime = (tnow.tm_hour<10 ? "0" + String(tnow.tm_hour) : String(tnow.tm_hour))  + (showDOT ? ":" : " ") + (tnow.tm_min<10 ? "0" + String(tnow.tm_min) : String(tnow.tm_min));
    indexedLayerZ1.drawString(1, matrix.getScreenHeight() / 2 - 22, 1, nTime.c_str());
    indexedLayerZ1.swapBuffers();*/
    showDOT = !showDOT;
    lastDot = millis();
  }
  return false;      
}

void displayTimeSmall(bool showSec = true) {
  static bool showDOT = true;
  static uint32_t lastDot = 0;
  static uint8_t oldMin = 61, oldSec = 61;
  String nTime, nSec;
  indexedLayerZ1.setFont(&FranklinGothicMediumCond9pt7b);
  indexedLayerZ1.setColor(colorZ1);
  struct tm tnow = myESPTime.getTimeStruct();
  if (tnow.tm_min != oldMin) {
    indexedLayerZ1.fillScreen(0);
    nTime = (tnow.tm_hour<10 ? "0" + String(tnow.tm_hour) : String(tnow.tm_hour))  + (showDOT ? ":" : " ") + (tnow.tm_min<10 ? "0" + String(tnow.tm_min) : String(tnow.tm_min));
    indexedLayerZ1.drawString(0, matrix.getScreenHeight() / 2 - 21, 1, nTime.c_str());
    indexedLayerZ1.swapBuffers();
    oldMin = tnow.tm_min;
  }
  if (millis() - lastDot >= 750) {
    indexedLayerZ1.fillScreen(0);
    nTime = (tnow.tm_hour<10 ? "0" + String(tnow.tm_hour) : String(tnow.tm_hour))  + (showDOT ? ":" : " ") + (tnow.tm_min<10 ? "0" + String(tnow.tm_min) : String(tnow.tm_min));
    indexedLayerZ1.drawString(0, matrix.getScreenHeight() / 2 - 21, 1, nTime.c_str());
    indexedLayerZ1.swapBuffers();
    showDOT = !showDOT;
    lastDot = millis();
  }
}

void displayDate(uint16_t durStep = 2000) {
  static bool firstStep = true, secondStep = false;  
  static uint32_t curDur;
  String nowDay, nowMounth, nowWeekDay;
  indexedLayerZ2.setFont(&FranklinGothicMediumCond7pt7b);
  indexedLayerZ2.setColor({0, 0xff, 0});
  struct tm tnow = myESPTime.getTimeStruct();
  if (firstStep) {
    if (millis() - curDur > durStep) {
      indexedLayerZ2.fillScreen(0);
      nowDay = String(tnow.tm_mday);
      indexedLayerZ2.drawString(43, matrix.getScreenHeight() / 2 - 15, 1, nowDay.c_str());
      indexedLayerZ2.swapBuffers();
      firstStep = false; secondStep = true; 
      curDur = millis();
    }
  }
  else if (secondStep) {
    if (millis() - curDur > durStep) {
      indexedLayerZ2.fillScreen(0);
      nowMounth = monthS_table[lang][tnow.tm_mon];
      indexedLayerZ2.drawString(43, matrix.getScreenHeight() / 2 - 15, 1, nowMounth.c_str());
      indexedLayerZ2.swapBuffers();
      secondStep = false;
      curDur = millis();
    }
  }
  else {
    if (millis() - curDur > durStep) {
      indexedLayerZ2.fillScreen(0);
      nowWeekDay = dayS_table[lang][tnow.tm_wday];
      indexedLayerZ2.drawString(43, matrix.getScreenHeight() / 2 - 15, 1, nowWeekDay.c_str());
      indexedLayerZ2.swapBuffers();
      firstStep = true;
      curDur = millis();
    }
  }
}

bool displayScrollText(const String &sText) {
  static bool firstStep = true;
  if (sText.length() > 0) {
      if (firstStep) {
        //Serial.print("scrollingLayerText: "); Serial.println(sText);
        scrollingLayerText.setOffsetFromTop(17);
        scrollingLayerText.setFont(&FreeSansBold7pt8b);
        //scrollingLayerText.setColor({0, 0, 0xff});
        scrollingLayerText.setMode(wrapForward);
        scrollingLayerText.setSpeed(speedTicker);
        //if(scrollingLayerText.start(sText.c_str(), -1) < 0); //-1 continios forever
        if(scrollingLayerText.start(sText.c_str(), 1) < 0)    Serial.println("scrollingLayerText text doesn't fit");
        firstStep = false;
        return false;
      }
      else {
        int16_t step = scrollingLayerText.getStatus();
        //Serial.print("scrollingLayerText step: "); Serial.println(step);
        if (step > 0) return false;
        else {
          scrollingLayerText.stop();
          firstStep = true;
          return true;
        }
      }
  }
  else return true;
}

bool displayText(const String &sText, uint32_t Per = 3000, const GFXfont usefont = FreeSansBold7pt8b, uint8_t offset = 0) { //offset special for date
  static bool firstStep = true;
  static uint32_t curDur;
  if (firstStep) {
    //Serial.print("indexedLayerZ3Text: "); Serial.println(sText);
    indexedLayerZ3.fillScreen(BLACK);
    //indexedLayerZ3.setFont(&FreeSansBold7pt8b);
    indexedLayerZ3.setFont(&usefont);
    uint16_t w, h;
    indexedLayerZ3.getDimensionsOfPrintedString(sText.c_str(), &w, &h);
    //Serial.print(w); Serial.print("x"); Serial.println(h);
    indexedLayerZ3.drawString((matrix.getScreenWidth()-w)/2, matrix.getScreenHeight() / 2 - 3 + offset, 1, sText.c_str());
    indexedLayerZ3.swapBuffers();
    firstStep = false; 
    curDur = millis();
    //firstStep ? Serial.println("1 firstStep 1 ") : Serial.println("1 firstStep 0 ");
    return false;
  }
  else {
    if (millis() - curDur > Per) {
      indexedLayerZ3.fillScreen(BLACK);
      indexedLayerZ3.swapBuffers();
      firstStep = true;
      return true;
    }
    else {
      return false;
    }
  }
}

bool displaySecondsZ2(uint32_t Per = 3000) {
  static bool firstStep = true, showDOT = true;
  static uint32_t curDur, lastDot;
  struct tm tnow = myESPTime.getTimeStruct();
  String nSec;
  //if (tnow.tm_sec != oldSec) {
  if (millis() - lastDot >= 750) {
      showDOT = !showDOT;
      lastDot = millis();
  }
  nSec = (tnow.tm_sec<10 ? ((showDOT ? " :0" : "  0") + String(tnow.tm_sec)) : ((showDOT ? " :" : "  ") + String(tnow.tm_sec)));
  if (firstStep) {
    backgroundLayer.fillScreen(BLACK);
    backgroundLayer.swapBuffers();
    indexedLayerZ2.fillScreen(BLACK);
    indexedLayerZ2.setFont(&FranklinGothicMediumCond7pt7b);
    //indexedLayerZ2.setColor({0xff, 0, 0xff});
    indexedLayerZ2.drawString(39, matrix.getScreenHeight() / 2 - 17, 1, nSec.c_str());
    indexedLayerZ2.swapBuffers();
    firstStep = false; 
    curDur = millis();
    //firstStep ? Serial.println("1 firstStep 1 ") : Serial.println("1 firstStep 0 ");
    return false;
  }
  else {
    if (millis() - curDur > Per) {
      indexedLayerZ2.fillScreen(BLACK);
      indexedLayerZ2.swapBuffers();
      firstStep = true;
      return true;
    }
    else {
      indexedLayerZ2.fillScreen(BLACK);
      indexedLayerZ2.drawString(39, matrix.getScreenHeight() / 2 - 17, 1, nSec.c_str());
      indexedLayerZ2.swapBuffers();
      return false;
    }
  }
}

bool displayWeatherIcon(uint8_t codeIcon, struct tm &tnow, uint32_t Per = 3000) {
  uint8_t x, y;
  x = 43; y = 0;
  static bool firstStep = true;
  static uint32_t curDur;
  if (firstStep) {
    backgroundLayer.fillScreen(BLACK);
    backgroundLayer.swapBuffers();
    indexedLayerZ2.fillScreen(BLACK);
    indexedLayerZ2.swapBuffers();
    switch (codeIcon)  {
    case 1:
        //x = (kMatrixWidth / 2) - (w01d.width/2 - 29);
        //y = (kMatrixHeight / 3) - (w01d.height/2 + 2);
        if (myESPTime.compTimeInt(dmodefrom, dmodeto, &tnow)) {
          drawBitmap(x, y, (const gimp32x32bitmap*)&w01d);
        }
        else drawBitmap(x, y, (const gimp32x32bitmap*)&w01n);
        break;
      case 2:
        if (myESPTime.compTimeInt(dmodefrom, dmodeto, &tnow)) {
          drawBitmap(x, y, (const gimp32x32bitmap*)&w02d);
        }
        else drawBitmap(x, y, (const gimp32x32bitmap*)&w02n);
        break;
      case 3:
        drawBitmap(x, y, (const gimp32x32bitmap*)&w03d);
        break;
      case 4:
        drawBitmap(x, y, (const gimp32x32bitmap*)&w04d);
        break;
      case 9:
        if (myESPTime.compTimeInt(dmodefrom, dmodeto, &tnow)) {
          drawBitmap(x, y, (const gimp32x32bitmap*)&w09d);
        }
        else drawBitmap(x, y, (const gimp32x32bitmap*)&w09n);
        break;
      case 10:
        if (myESPTime.compTimeInt(dmodefrom, dmodeto, &tnow)) {
          drawBitmap(x, y, (const gimp32x32bitmap*)&w09d);
        }
        else drawBitmap(x, y, (const gimp32x32bitmap*)&w09n);
        break;
      case 11:
        if (myESPTime.compTimeInt(dmodefrom, dmodeto, &tnow)) {
          drawBitmap(x, y, (const gimp32x32bitmap*)&w11d);
        }
        else drawBitmap(x, y, (const gimp32x32bitmap*)&w11n);
        break;
      case 13:
        drawBitmap(x, y, (const gimp32x32bitmap*)&w13d);
        break;
      case 50:
        drawBitmap(x, y, (const gimp32x32bitmap*)&w50d);
        break;
      default:
        drawBitmap(x, y, (const gimp32x32bitmap*)&wbee_icon);
        break;
    }
    backgroundLayer.swapBuffers();
    firstStep = false; 
    curDur = millis();
    return false; 
  }
  else {
    if (millis() - curDur > Per) {
      backgroundLayer.fillScreen(BLACK);
      backgroundLayer.swapBuffers();
      indexedLayerZ2.fillScreen(BLACK);
      indexedLayerZ2.swapBuffers();
      firstStep = true;
      return true;
    }
    else return false;
  }
}

bool displayTemp(uint32_t Per = 3000) {
  static bool firstStep = true;
  static uint32_t curDur;
  if (firstStep) {
    //weatherTemp = "-20~C";
    backgroundLayer.fillScreen(BLACK);
    backgroundLayer.swapBuffers();
    indexedLayerZ2.fillScreen(BLACK);
    indexedLayerZ2.swapBuffers();
    indexedLayerZ2.setFont(&FranklinGothicMediumCond7pt7b);
    //indexedLayerZ2.setColor({0xff, 0, 0xff});
    indexedLayerZ2.drawString(38, matrix.getScreenHeight() / 2 - 17, 1, weatherTemp.c_str());
    indexedLayerZ2.swapBuffers();
    //displayWeatherIcon(weatherIcon);
    firstStep = false; 
    curDur = millis();
    return false;    
  }
  else {
    if (millis() - curDur > Per) {
      backgroundLayer.fillScreen(BLACK);
      backgroundLayer.swapBuffers();
      indexedLayerZ2.fillScreen(BLACK);
      indexedLayerZ2.swapBuffers();
      firstStep = true;
      return true;
    }
    else return false;
  }
}

bool displayWind(uint32_t Per = 3000) {
  static bool firstStep = true;
  static uint32_t curDur;
  if (firstStep) {
    //weatherTemp = "-20~C";
    backgroundLayer.fillScreen(BLACK);
    backgroundLayer.swapBuffers();
    indexedLayerZ2.fillScreen(BLACK);
    indexedLayerZ2.setFont(&FranklinGothicMediumCond7pt7b);
    //indexedLayerZ2.setColor({0xff, 0, 0xff});
    indexedLayerZ2.drawString(39, matrix.getScreenHeight() / 2 - 17, 1, weatherSpeed.c_str());
    //indexedLayerZ2.swapBuffers();
    indexedLayerZ2.setFont(font5x7);
    indexedLayerZ2.drawString(55, 10, 1, weatherDirection.c_str());
    indexedLayerZ2.swapBuffers();
    drawBitmap(54, 0, (const gimp32x32bitmap*)&wind10);
    backgroundLayer.swapBuffers();
    /*scrollingLayerTextSmall.setOffsetFromTop(5);
    scrollingLayerTextSmall.setStartOffsetFromLeft(20);
    scrollingLayerTextSmall.setFont(font6x10);
    scrollingLayerTextSmall.setMode(wrapForwardFromLeft);
    scrollingLayerTextSmall.setSpeed(speedTicker);
    if(scrollingLayerTextSmall.start("m/s", 1) < 0)    Serial.println("scrollingLayerText text doesn't fit");*/
    firstStep = false; 
    curDur = millis();
    return false;    
  }
  else {
    if (millis() - curDur > Per) {
      backgroundLayer.fillScreen(BLACK);
      backgroundLayer.swapBuffers();
      indexedLayerZ2.fillScreen(BLACK);
      indexedLayerZ2.swapBuffers();
      firstStep = true;
      return true;
    }
    else return false;
  }
}

bool displayBitmap(uint8_t nIcon, uint32_t Per = 3000) { //https://docs.pixelmatix.com/SmartMatrix/library.html#smartmatrix-library-how-to-use-the-library-drawing-raw-bitmaps
  /*static bool firstStep = true;
  static uint32_t curDur;
  uint8_t x, y;
  if (firstStep) {
    indexedLayerZ2.fillScreen(BLACK);
    indexedLayerZ2.swapBuffers();
    backgroundLayer.fillScreen(BLACK);
    switch (nIcon)
    {
    case 1:
      x = (kMatrixWidth / 2) - (wrainy_icon.width/2 - 24);
      y = (kMatrixHeight / 3) - (wrainy_icon.height/2 + 2);
      drawBitmap(x, y, (const gimp32x32bitmap*)&wrainy_icon);
      break;
    case 2:
      x = (kMatrixWidth / 2) - (whot_sun.width/2 - 24);
      y = (kMatrixHeight / 3) - (whot_sun.height/2 + 2);
      drawBitmap(x, y, (const gimp32x32bitmap*)&whot_sun);
      break;
    default:
      break;
    }
    backgroundLayer.swapBuffers();
    firstStep = false; 
    curDur = millis();
    //firstStep ? Serial.println("1 firstStep 1 ") : Serial.println("1 firstStep 0 ");
    return false;
  }
  else {
    if (millis() - curDur > Per) {
      backgroundLayer.fillScreen(BLACK);
      backgroundLayer.swapBuffers();
      firstStep = true;
      return true;
    }
    else {
      return false;
    }
  }*/
}

/*void displayBitmap1(uint16_t durStep = 2000) {
  static uint32_t curDur;
  static uint8_t nIcon = 1;
  uint8_t x, y;
  if (millis() - curDur > durStep) {
    nIcon++;
    if (nIcon > 4) nIcon = 1;
    curDur = millis();
  }
  backgroundLayer.fillScreen(BLACK);
  indexedLayerZ2.setFont(&FranklinGothicMediumCond7pt7b);
  indexedLayerZ2.setColor({0xff, 0, 0xff});
  String nowDay;
  switch (nIcon)  {
  case 1:
    x = (kMatrixWidth / 2) - (Heart_image.width/2 - 20);
    y = (kMatrixHeight / 3) - (Heart_image.height/2 + 2);
    // to use drawBitmap, must cast the pointer to pixelmatixlogo as (const gimp32x32bitmap*)
    drawBitmap(x,y,(const gimp32x32bitmap*)&Heart_image);
    nowDay = "+25C";
    indexedLayerZ2.fillScreen(0);
    indexedLayerZ2.drawString(39, matrix.getScreenHeight() / 2 - 15, 1, nowDay.c_str());
    indexedLayerZ2.swapBuffers();
    break;
  case 2:
    x = (kMatrixWidth / 2) - (bar_chart_image.width/2 - 20);
    y = (kMatrixHeight / 3) - (bar_chart_image.height/2 + 2);
    // to use drawBitmap, must cast the pointer to pixelmatixlogo as (const gimp32x32bitmap*)
    drawBitmap(x,y,(const gimp32x32bitmap*)&bar_chart_image);
    nowDay = "+80C";
    indexedLayerZ2.fillScreen(0);
    indexedLayerZ2.drawString(39, matrix.getScreenHeight() / 2 - 15, 1, nowDay.c_str());
    indexedLayerZ2.swapBuffers();
    break;
  case 3:
    x = (kMatrixWidth / 2) - (Alarm_clock_image.width/2 - 20);
    y = (kMatrixHeight / 3) - (Alarm_clock_image.height/2 + 2);
    // to use drawBitmap, must cast the pointer to pixelmatixlogo as (const gimp32x32bitmap*)
    drawBitmap(x,y,(const gimp32x32bitmap*)&Alarm_clock_image);
    nowDay = "-25C";
    indexedLayerZ2.fillScreen(0);
    indexedLayerZ2.drawString(39, matrix.getScreenHeight() / 2 - 15, 1, nowDay.c_str());
    indexedLayerZ2.swapBuffers();
    break;
  case 4:
    x = (kMatrixWidth / 2) - (Bee_image.width/2 - 20);
    y = (kMatrixHeight / 3) - (Bee_image.height/2 + 2);
    // to use drawBitmap, must cast the pointer to pixelmatixlogo as (const gimp32x32bitmap*)
    drawBitmap(x,y,(const gimp32x32bitmap*)&Bee_image);
    nowDay = ":00";
    indexedLayerZ2.fillScreen(0);
    indexedLayerZ2.drawString(39, matrix.getScreenHeight() / 2 - 17, 1, nowDay.c_str());
    indexedLayerZ2.swapBuffers();
    break;  
  default:
    break;
  } 
  backgroundLayer.swapBuffers();
}*/

void drawBitmap(int16_t x, int16_t y, const gimp32x32bitmap* bitmap) {
  for(unsigned int i=0; i < bitmap->height; i++) {
    for(unsigned int j=0; j < bitmap->width; j++) {
      SM_RGB pixel = { bitmap->pixel_data[(i*bitmap->width + j)*3 + 0],
                      bitmap->pixel_data[(i*bitmap->width + j)*3 + 1],
                      bitmap->pixel_data[(i*bitmap->width + j)*3 + 2] };
      if(COLOR_DEPTH == 48) {
          pixel.red = pixel.red << 8;
          pixel.green = pixel.green << 8;
          pixel.blue = pixel.blue << 8;
      }

      backgroundLayer.drawPixel(x + j, y + i, pixel);
    }
  }
}
