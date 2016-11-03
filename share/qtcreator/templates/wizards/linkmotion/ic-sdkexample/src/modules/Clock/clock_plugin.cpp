#include "clock_plugin.h"
#include "clocklabel.h"

#include <qqml.h>

void ClockPlugin::registerTypes(const char *uri)
{
    // @uri Clock
    qmlRegisterType<ClockLabel>(uri, 1, 0, "ClockLabel");
}

