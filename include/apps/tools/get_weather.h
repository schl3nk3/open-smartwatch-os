#ifndef OSW_APP_GET_WHEATHER_H
#define OSW_APP_GET_WHEATHER_H

#include <osw_hal.h>
#include <osw_ui.h>
#include "apps/main/switcher.h"

#include "osw_app.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

extern OswAppSwitcher *settingsAppSwitcher;




class OswAppGetWeather : public OswApp {
 public:
  OswAppGetWeather(void){
    ui = OswUI::getInstance();
    getApiUrl();
  };
  virtual void setup() override;
  virtual void loop() override;
  virtual void stop() override;
  ~OswAppGetWeather(){};

 private:
  OswUI* ui;

  char const *weather_URL = "http://api.openweathermap.org/data/2.5/weather";
  char const *weather_units = "metric";
  char const *weather_appId = "?APIKEY?"; // <- insert API key
  char const *weather_city = "Berlin"; // <- insert city
  char const *weather_country_code = "DE";
  
  std::string weather_api_url;
  const void getApiUrl()
  {
    weather_api_url = OswAppGetWeather::weather_URL;
    weather_api_url.append("?q=");
    weather_api_url.append(OswAppGetWeather::weather_city);
    weather_api_url.append(",");
    weather_api_url.append(OswAppGetWeather::weather_country_code);
    weather_api_url.append("&units=");
    weather_api_url.append(OswAppGetWeather::weather_units);
    weather_api_url.append("&APPID=");
    weather_api_url.append(OswAppGetWeather::weather_appId);
  }

  void drawCityInfo();
  void drawTemperature();
  void drawHttpError();
  bool connected();
  void updateData();

  double temp = -1;
  int httpCode = -1;
  long lastSync = 0;
  long lastConnect = 0;
  HTTPClient http;
};

#endif
