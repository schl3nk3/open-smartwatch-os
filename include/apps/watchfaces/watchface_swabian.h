#ifndef OSW_APP_WATCHFACE_SWABIAN_H
#define OSW_APP_WATCHFACE_SWABIAN_H

#include <osw_hal.h>
#include <osw_ui.h>

#include <string>

#include "osw_app.h"
using std::string;

class OswAppWatchfaceSwabian : public OswApp {
 public:
  OswAppWatchfaceSwabian(void) { ui = OswUI::getInstance(); };
  virtual void setup() override;
  virtual void loop() override;
  virtual void stop() override;
  ~OswAppWatchfaceSwabian(){};

 private:
  OswUI* ui;
  void swabianTime();
  void drawTextRow(string text_row, int16_t y, bool lineBelow, bool lineAbove);
  void timeToSwabian(uint32_t hour, uint32_t minute, uint32_t second, string* text_row1, string* text_row2,
                     string* text_row3);
  string* getHourByIndex(int hour);
};

#endif
