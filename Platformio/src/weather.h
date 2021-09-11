void getWeather(uint32_t firstPer, uint32_t nextPer) {
  static uint32_t ms = 0;
  static bool firstStep = true;
  if ((millis() - ms < firstPer) && firstStep) return;
  if ((millis() - ms < nextPer) && !firstStep) return;
  if (myWeather.getWeather(&lang)) {
    firstStep = false;
    strWeather = overboard[lang];
    strWeather += myWeather.wDescription;
    strWeather += temper[lang];
    strWeather += String(myWeather.temp, 1);
    strWeather += "/" + String(myWeather.feels_like,1);
    strWeather += hum[lang];  strWeather += String(myWeather.humidity);
    strWeather += pres[lang]; strWeather += String(myWeather.pressure_mm,0);
    strWeather += wind[lang];
    strWeather += String(myWeather.wSpeed, 0);
    strWeather += windsp[lang];
    if (myWeather.wDeg >22.5 && myWeather.wDeg <=67.5) {strWeather += windir_table[lang][0]; weatherDirection = windir[0];}
    else if (myWeather.wDeg >67.5 && myWeather.wDeg <=112.5) {strWeather += windir_table[lang][1];weatherDirection = windir[1];}
    else if (myWeather.wDeg >112.5 && myWeather.wDeg <=157.5) {strWeather += windir_table[lang][2];weatherDirection = windir[2];}
    else if (myWeather.wDeg >157.5 && myWeather.wDeg <=202.5) {strWeather += windir_table[lang][3];weatherDirection = windir[3];}
    else if (myWeather.wDeg >202.5 && myWeather.wDeg <=247.5) {strWeather += windir_table[lang][4];weatherDirection = windir[4];}
    else if (myWeather.wDeg >247.5 && myWeather.wDeg <=292.5) {strWeather += windir_table[lang][5];weatherDirection = windir[5];}
    else if (myWeather.wDeg >292.5 && myWeather.wDeg <=337.5) {strWeather += windir_table[lang][6];weatherDirection = windir[6];}
    else strWeather += windir_table[lang][7];
    strWeather += cloudstxt[lang] + String(myWeather.clouds) + "% ";
    Serial.print(F("Get weather: ")); Serial.println(strWeather); 
    //airTemp = (myWeather.temp >= 0? "+" + String(myWeather.temp, 1) : String(myWeather.temp, 1));
    //timeSendAir = myESPTime.getTimeStr(true);
    weatherIcon = myWeather.wIcon.substring(0, 2).toInt();
    //Serial.print("weatherIcon: "); Serial.println(weatherIcon);
    weatherTemp = (myWeather.temp < 0 ? String(myWeather.temp, 0) + "~C" : " " + String(myWeather.temp, 0) + "~C");
    weatherSpeed = String(myWeather.wSpeed, 0);// + "m/c";
  }
  //sendToLED(airTemp, '2'); 
  ms = millis();
}

void getWeatherForecast(uint32_t firstPer, uint32_t nextPer) {
  static uint32_t ms = 0;
  static bool firstStep = true;
  if ((millis() - ms < firstPer) && firstStep) return;
  if ((millis() - ms < nextPer) && !firstStep) return;
  if (myWeather.getWForecast(&lang)) {
    firstStep = false;
    strWeatherFcast = forecast[lang];
    strWeatherFcast += myWeather.wfDescription;
    //strWeatherFcast += temperforecast[lang]; strWeatherFcast += String(myWeather.fTemp,0);
    strWeatherFcast += tempermin[lang]; strWeatherFcast += String(myWeather.tempfMin,1);
    strWeatherFcast += tempermax[lang]; strWeatherFcast += String(myWeather.tempfMax,1);
    strWeatherFcast += hum[lang]; strWeatherFcast += String(myWeather.fHumidity);
    strWeatherFcast += pres[lang]; strWeatherFcast += String(myWeather.fPressure_mm,0);
    strWeatherFcast += wind[lang]; strWeatherFcast += String(myWeather.fwSpeed, 0);
    strWeatherFcast += windsp[lang];
    if (myWeather.fwDeg >22.5 && myWeather.fwDeg <=67.5) strWeatherFcast += windir_table[lang][0];
    else if (myWeather.fwDeg >67.5 && myWeather.fwDeg <=112.5) strWeatherFcast += windir_table[lang][1];
    else if (myWeather.fwDeg >112.5 && myWeather.fwDeg <=157.5) strWeatherFcast += windir_table[lang][2];
    else if (myWeather.fwDeg >157.5 && myWeather.fwDeg <=202.5) strWeatherFcast += windir_table[lang][3];
    else if (myWeather.fwDeg >202.5 && myWeather.fwDeg <=247.5) strWeatherFcast += windir_table[lang][4];
    else if (myWeather.fwDeg >247.5 && myWeather.fwDeg <=292.5) strWeatherFcast += windir_table[lang][5];
    else if (myWeather.fwDeg >292.5 && myWeather.fwDeg <=337.5) strWeatherFcast += windir_table[lang][6];
    else strWeatherFcast += windir_table[lang][7];
    Serial.print(F("Get forecast: ")); Serial.println(strWeatherFcast);
  }
  ms = millis();
}

void getSeaTemp(uint32_t firstPer, uint32_t nextPer) {
  static uint32_t ms = 0;
  static bool firstStep = true;
  if ((millis() - ms < firstPer) && firstStep) return;
  if ((millis() - ms < nextPer) && !firstStep) return;
  if (myWeather.getSeaTemp(seaID.c_str())) {
      firstStep = false;
      strSea = tempersea[lang] + myWeather.seaTemp + "~С.";
      //timeSendSea = myESPTime.getTimeStr(true);
      Serial.print("strSea: "); Serial.println(strSea);
  }  
  ms = millis();
}