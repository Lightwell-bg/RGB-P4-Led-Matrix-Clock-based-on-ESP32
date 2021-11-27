static const char successResponse[] PROGMEM = 
  "<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...";

String updateError = "";
String getUpdaterError();

void init_firmWareUpdate(void) {
    HTTP.on("/update", HTTP_POST, []() {
      if (Update.hasError()) {
        HTTP.send(200, F("text/html"), String(F("Update error: ")) + updateError);
      } else {
        HTTP.client().setNoDelay(true);
        HTTP.send_P(200, PSTR("text/html"), successResponse);
        delay(100);
        HTTP.client().stop();
        ESP.restart();
      }
    }, []() {
      HTTPUpload& upload = HTTP.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        //size_t content_len = upload.totalSize;
        //size_t content_len = upload.currentSize;
        int cmd = (upload.filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
        Serial.printf("Update: %s\n", upload.filename.c_str());
        //if (!Update.begin()) { //start with max available size
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
          //Update.printError(Serial);
          updateError = getUpdaterError();
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          //Update.printError(Serial);
          updateError = getUpdaterError();
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          //Update.printError(Serial);
          updateError = getUpdaterError();
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
    });
    yield();
}

String getUpdaterError() {
  Update.printError(Serial);
  StreamString str;
  Update.printError(str);
  return String(str.c_str());
}

/*
void init_firmWareUpdate(void) { //WORK****************************************
    HTTP.on("/update", HTTP_POST, []() {
      if (Update.hasError()) {
        HTTP.send(200, F("text/html"), String(F("Update error: ")) + updateError);
      } else {
        HTTP.client().setNoDelay(true);
        HTTP.send_P(200, PSTR("text/html"), successResponse);
        delay(100);
        HTTP.client().stop();
        ESP.restart();
      }
    }, []() {
      HTTPUpload& upload = HTTP.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) { //start with max available size
          //Update.printError(Serial);
          updateError = getUpdaterError();
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          //Update.printError(Serial);
          updateError = getUpdaterError();
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          //Update.printError(Serial);
          updateError = getUpdaterError();
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
    });
    yield();
}




void init_firmWareUpdate(void) {
    HTTP.on("/update", HTTP_POST, []() {
      HTTP.sendHeader("Connection", "close");
      HTTP.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = HTTP.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        size_t content_len = 0; //upload.totalSize;
        Serial.printf("Update: %s size %d\n", upload.filename.c_str(), content_len);
        int command;
        //size_t maxSketchSpace = ((size_t) &_FS_end - (size_t) &_FS_start);
        if (upload.filename.indexOf("littlefs") > -1 || upload.filename.indexOf("spiffs") > -1) {
          size_t spiffsSize = ((size_t) SPIFFS.totalBytes() - (size_t) SPIFFS.usedBytes());
          if(content_len > (int) spiffsSize) {
              Serial.printf("[httpUpdate] spiffsSize to low (%d) needed: %d\n", spiffsSize, content_len);
              //startUpdate = false;
          }
          command = U_SPIFFS;
          Serial.printf("[httpUpdate] runUpdate spiffs...\n");
        }
        else {
          command = U_FLASH;
          Serial.printf("[httpUpdate] runUpdate flash...\n");
        }
        if (!Update.begin(content_len, command)) { //start with max available size
          Serial.print("Update.begin "); Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Serial.print("UPLOAD_FILE_WRITE "); Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Serial.print("UPLOAD_FILE_END "); Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
    });
    yield();
}*/

