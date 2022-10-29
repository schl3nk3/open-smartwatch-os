#ifndef SAMPLE_WATCHFACE_H
#define SAMPLE_WATCHFACE_H

#include <osw_hal.h>
#include <osw_ui.h>

#include "osw_app.h"

class SampleWatchface : public OswApp
{
public:
    SampleWatchface(void)
    {
        ui = OswUI::getInstance();
    };
    virtual void setup() override;
    virtual void loop() override;
    virtual void stop() override;
    ~SampleWatchface(){};

private:
    OswUI *ui;
};

#endif