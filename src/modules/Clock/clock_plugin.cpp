#include "clock_plugin.h"
#include "clocklabel.h"

#include <qqml.h>

void ClockPlugin::registerTypes(const char *uri)
{
    // @uri com.link-motion.toolkit
    qmlRegisterType<ClockLabel>(uri, 1, 0, "ClockLabel");
}

