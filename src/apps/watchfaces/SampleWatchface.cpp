#include "./apps/watchfaces/SampleWatchface.h"
#include "./apps/watchfaces/OswAppWatchface.h"

#include <config.h>
#include <gfx_util.h>
#include <osw_app.h>
#include <osw_config_keys.h>
#include <osw_hal.h>
#include <time.h>

void SampleWatchface::setup()
{
}

void SampleWatchface::loop()
{
    OswHal *hal = OswHal::getInstance();
    if (hal->btnHasGoneDown(BUTTON_3))
    {
        hal->increaseBrightness(25);
    }
    if (hal->btnHasGoneDown(BUTTON_2))
    {
        hal->decreaseBrightness(25);
    }

    hal->gfx()->fill(ui->getBackgroundColor());

    hal->gfx()->setTextCenterAligned();
    hal->gfx()->setTextSize(3);
    hal->gfx()->setTextCursor(120, 100);
    hal->gfx()->print("Sample");
    hal->gfx()->setTextCursor(120, 120);
    hal->gfx()->print("Watchface");

    hal->requestFlush();
}

void SampleWatchface::stop()
{
    // OswHal::getInstance()->disableDisplayBuffer();
}