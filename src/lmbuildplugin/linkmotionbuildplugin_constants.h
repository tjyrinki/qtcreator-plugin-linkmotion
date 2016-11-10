/*####################################################################
#
# This file is part of the LinkMotion Build plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/

#ifndef LINKMOTION_BUILD_CONSTANTS_H
#define LINKMOTION_BUILD_CONSTANTS_H

#include <QString>

namespace LinkMotion {
namespace Internal {
namespace Constants {

const char LINKMOTION_BC_ID[]      = "LinkMotion.BuildConfiguration";
const char LINKMOTION_BUILD_STEP_ID[]      = "LinkMotion.BuildStep";
const char LINKMOTION_BUILD_STEP_DISPLAY_NAME[] = "LinkMotion Build Step";
const char LINKMOTION_SIMULATOR_TYPE[] = "LinkMotion.Simulator";
const char LINKMOTION_DEVICE_TYPE[] = "LinkMotion.Device";

// this value comes from debugger / debuggerrunconfigurationaspect.cpp file
const char PROPERTY_USEQMLDEBUG[] = "linkQmlDebuggingLibrary";


const char BUILD_USE_DEFAULT_ARGS_KEY[] = "LinkMotion.LinkMotionBuildStep.ArgumentsUseDefaults";
const char BUILD_ARGUMENTS_KEY[] = "LinkMotion.LinkMotionBuildStep.Arguments";
const char CLEAN_KEY[] = "LinkMotion.LinkMotionBuildStep.Clean";

}
}
}
#endif // LINKMOTION_BUILD_CONSTANTS_H
