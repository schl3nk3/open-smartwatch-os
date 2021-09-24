#include "./apps/watchfaces/watchface_swabian.h"

#include <config.h>
#include <gfx_util.h>
#include <osw_app.h>
#include <osw_config_keys.h>
#include <osw_hal.h>
#include <time.h>

// Schwäbische Texte
char* voll = "voll";
char* viertel = "viertl";
char* halb = "halber";
char* dreiviertel = "dreiviertl";
string zahlen[12] = {"ZWOELFE", "OIS",   "ZWOI", "DREI", "VIER", "FEMF",
                     "SEGGS",   "SIEBA", "ACHD", "NEIN", "ZEAH", "AELF"};

void OswAppWatchfaceSwabian::swabianTime() {
  OswHal* hal = OswHal::getInstance();

  // get current time
  uint32_t second = 0;
  uint32_t minute = 0;
  uint32_t hour = 0;
  hal->getLocalTime(&hour, &minute, &second);

  // default text appearance
  hal->gfx()->setTextLeftAligned();
  hal->gfx()->setTextColor(rgb565(255, 255, 255), rgb565(255, 255, 255));

  // animated background circles
  hal->gfx()->fillCircle(80 - second, 80 + 2.5 * second, 60, rgb565(60, 0, 0), CIRC_OPT::DRAW_ALL);
  hal->gfx()->fillCircle(140 - 1.5 * second, 120 + second, 50, rgb565(0, 60, 0), CIRC_OPT::DRAW_ALL);

  // convert time to SCHWÄBISCH
  string text_row1 = "";
  string text_row2 = "";
  string text_row3 = "";
  timeToSwabian(hour, minute, second, &text_row1, &text_row2, &text_row3);

  // time in Swabian over 3 rows
  hal->gfx()->setTextSize(3);
  drawTextRow(text_row1, 80);
  hal->gfx()->setTextSize(2);
  drawTextRow(text_row2, 120);
  hal->gfx()->setTextSize(3);
  drawTextRow(text_row3, 160);

  // digital time for dummies
  hal->gfx()->setTextSize(1);
  hal->gfx()->setTextCursor(120 - hal->gfx()->getTextOfsetColumns(4), 210);
  hal->gfx()->printDecimal(hour, 2);
  hal->gfx()->print(":");
  hal->gfx()->printDecimal(minute, 2);
  hal->gfx()->print(":");
  hal->gfx()->printDecimal(second, 2);
}

void OswAppWatchfaceSwabian::drawTextRow(string text_row, int16_t y) {
  OswHal* hal = OswHal::getInstance();
  float text_length = text_row.length();
  hal->gfx()->setTextCursor(120 - hal->gfx()->getTextOfsetColumns(text_length / 2.0), y);
  hal->gfx()->print(text_row.c_str());
}

void OswAppWatchfaceSwabian::timeToSwabian(uint32_t hour, uint32_t minute, uint32_t second, string* text_row1,
                                           string* text_row2, string* text_row3) {
  /*
  
    Swabian time has special senteces regarding to the minute of the current hour
    Basically the quater minutes 0, 15, 30, 45 have their own sentences
    The minutes between always tell the minutes to the  next quater (saying: before or after)

  */
  if (minute == 0) {
    if (hour == 0) {
      *text_row2 = "Mitternacht";
    } else {
      *text_row2 = zahlen[hour % 12];
    }
  }
  if (minute <= 9) {
    *text_row1 = zahlen[minute % 12];
    *text_row2 = " noch ";
    *text_row3 = zahlen[hour % 12];
  }
  if (minute >= 10 && minute < 15) {
    uint32_t min_bis_viertel = 15 - minute;
    *text_row1 = zahlen[min_bis_viertel % 12];
    *text_row2 = "vor Viertel";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute == 15) {
    *text_row1 = viertel;
    *text_row2 = " ";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute >= 16 && minute <= 19) {
    uint32_t min_nach_viertel = minute - 15;
    *text_row1 = zahlen[min_nach_viertel % 12];
    *text_row2 = "noch Viertel";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute >= 20 && minute <= 29) {
    uint32_t min_bis_halb = 30 - minute;
    *text_row1 = zahlen[min_bis_halb % 12];
    *text_row2 = "vor Halber";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute == 30) {
    *text_row1 = halb;
    *text_row2 = " ";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute >= 31 && minute <= 39) {
    uint32_t min_nach_halb = minute - 30;
    *text_row1 = zahlen[min_nach_halb % 12];
    *text_row2 = "nach Halber";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute >= 40 && minute <= 44) {
    uint32_t min_vor_dreiviertel = 45 - minute;
    *text_row1 = zahlen[min_vor_dreiviertel % 12];
    *text_row2 = "vor Dreiviertel";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute == 45) {
    *text_row1 = dreiviertel;
    *text_row2 = " ";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute >= 46 && minute <= 49) {
    uint32_t min_nach_dreiviertel = minute - 45;
    *text_row1 = zahlen[min_nach_dreiviertel % 12];
    *text_row2 = "nach Dreiviertel";
    *text_row3 = zahlen[hour % 12 + 1];
  }
  if (minute >= 50 && minute <= 59) {
    uint32_t min_vor_voll = 60 - minute;
    *text_row1 = zahlen[min_vor_voll % 12];
    *text_row2 = "vor";
    *text_row3 = zahlen[hour % 12 + 1];
  }
}

void OswAppWatchfaceSwabian::setup() {}

void OswAppWatchfaceSwabian::loop() {
  OswHal* hal = OswHal::getInstance();

  // change brightness of watch
  if (hal->btnHasGoneDown(BUTTON_3)) {
    hal->increaseBrightness(25);
  }
  if (hal->btnHasGoneDown(BUTTON_2)) {
    hal->decreaseBrightness(25);
  }

  // fill background
  hal->gfx()->fill(ui->getBackgroundColor());

  swabianTime();

  hal->requestFlush();
}

void OswAppWatchfaceSwabian::stop() {
  // hal->disableDisplayBuffer();
}
