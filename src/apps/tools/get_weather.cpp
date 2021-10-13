#include "./apps/tools/get_weather.h"

#include <config.h>
#include <gfx_util.h>
#include <osw_app.h>
#include <osw_config_keys.h>
#include <osw_hal.h>
#include <osw_ui.h>
#include <services/OswServiceTasks.h>
#include <time.h>
#ifdef OSW_FEATURE_WIFI
#include <services/OswServiceTaskWiFi.h>
#endif

void OswAppGetWeather::setup() {}

void OswAppGetWeather::drawCityInfo() {
  OswHal* hal = OswHal::getInstance();

  // print city name
  hal->gfx()->setTextSize(2);
  int16_t w = hal->gfx()->getTextOfsetColumns(strlen(OswAppGetWeather::weather_city));
  hal->gfx()->setTextCursor(120.0 - w / 2.0, 200);
  hal->gfx()->print(OswAppGetWeather::weather_city);

  // print country code
  hal->gfx()->setTextSize(1);
  int16_t w2 = hal->gfx()->getTextOfsetColumns(strlen(OswAppGetWeather::weather_country_code));
  hal->gfx()->setTextCursor(120.0 - w2 / 2.0, 220);
  hal->gfx()->print(OswAppGetWeather::weather_country_code);
}

void OswAppGetWeather::drawTemperature() {
  // TODO: Center temperatue
  // TODO: How to negative numbers perform?
  if (temp == -1 || httpCode != 200) {
    return;
  }

  OswHal* hal = OswHal::getInstance();
  hal->gfx()->setTextSize(3);
  hal->gfx()->setTextCursor(60, 120);
  hal->gfx()->setTextLeftAligned();
  hal->gfx()->setTextColor(this->ui->getForegroundColor(), this->ui->getForegroundColor());
  if (this->temp == -1) {
    hal->gfx()->print("???");
  } else {
    hal->gfx()->print(this->temp, 2);
    float arc_start = 160;
    float arc_width = 40;
    hal->gfx()->drawArc(120, 120, arc_start, arc_start + arc_width, 5, 110, 5, rgb565(220, 220, 220), false);
    hal->gfx()->drawArc(120, 120, arc_start, arc_start + this->temp, 4, 110, 4, rgb565(255, 80, 80), false);
  }
  hal->gfx()->setTextCursor(hal->gfx()->getTextCursorX() + 10, 120);
  hal->gfx()->print("*C");
}

void OswAppGetWeather::drawHttpError() {
  if (this->httpCode == 200) return;

  OswHal* hal = OswHal::getInstance();
  hal->gfx()->setTextSize(3);
  hal->gfx()->setTextCursor(120, 120);
  hal->gfx()->setTextRightAligned();
  hal->gfx()->setTextColor(this->ui->getDangerColor(), this->ui->getDangerColor());
  if (this->httpCode == -1) {
    hal->gfx()->print("???");
  } else {
    hal->gfx()->print(this->httpCode);
  }
}

bool OswAppGetWeather::connected() {
  OswHal* hal = OswHal::getInstance();
  this->ui->setTextCursor(BUTTON_3);
#ifdef OSW_FEATURE_WIFI
  if (OswServiceAllTasks::wifi.isConnected()) {
    hal->gfx()->print(LANG_DISCONNECT);
  } else {
    hal->gfx()->print(LANG_CONNECT);
  }

  if (hal->btnHasGoneDown(BUTTON_3)) {
    if (OswServiceAllTasks::wifi.isConnected()) {
      OswServiceAllTasks::wifi.disconnectWiFi();
      OswServiceAllTasks::wifi.disableWiFi();
    } else {
      OswServiceAllTasks::wifi.enableWiFi();
      OswServiceAllTasks::wifi.connectWiFi();
      this->lastConnect = millis();
    }
  }
  return OswServiceAllTasks::wifi.isConnected();
#endif
  return false;
}

void OswAppGetWeather::updateData() {
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  // TODO: MAKE URL DYNAMIC AGAIN
  // http.begin(this->getApiUrl());
  http.begin(this->weather_api_url.c_str());
  Serial.println(this->weather_api_url.c_str());

  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  this->httpCode = http.GET();

  // httpCode will be negative on error
  if (this->httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      StaticJsonDocument<1024> payload;
      deserializeJson(payload, http.getString().c_str());
      JsonObject object = payload.as<JsonObject>();
      this->temp = object["main"]["temp"].as<double>();
      Serial.printf("Got temperatur %f\n", this->temp);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(this->httpCode).c_str());
  }

  http.end();
}

void OswAppGetWeather::loop() {
  OswHal* hal = OswHal::getInstance();
  hal->gfx()->fill(ui->getBackgroundColor());
  hal->gfx()->setTextLeftAligned();
  hal->gfx()->setTextColor(this->ui->getForegroundColor(), this->ui->getForegroundColor());

  // TODO: Add weather icon
  // TODO: Add huminity
  // TODO: Add timestamp of last sync

  this->drawCityInfo();

  if (this->connected()) {
    // wait 2 seconds to connect successfully, refresh every 10 seconds
    if (millis() - lastConnect > 2000 && millis() - lastSync > 10000) {
      this->updateData();
      this->lastSync = millis();
    }
  }

  this->drawHttpError();
  this->drawTemperature();

  hal->requestFlush();
}
void OswAppGetWeather::stop() {
  // disconnect wifi on exit
  if (OswServiceAllTasks::wifi.isConnected()) {
    OswServiceAllTasks::wifi.disconnectWiFi();
    OswServiceAllTasks::wifi.disableWiFi();
  }
}
