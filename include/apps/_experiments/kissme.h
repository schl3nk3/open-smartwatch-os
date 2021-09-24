#ifndef OSW_APP_KISS_ME_H
#define OSW_APP_KISS_ME_H

#include <osw_hal.h>

#include "osw_app.h"

class OswAppKissMe : public OswApp {
 public:
  OswAppKissMe(void){};
  virtual void setup() override;
  virtual void loop() override;
  virtual void stop() override;
  ~OswAppKissMe(){};

 private:
  int intervall = 0;
  int diff = 10;
};
#endif
