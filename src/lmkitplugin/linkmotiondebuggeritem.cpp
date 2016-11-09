/*####################################################################
#
# This file is part of the LinkMotion Kit plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#include "linkmotiondebuggeritem.h"
#include "linkmotionkitplugin_constants.h"

#include <QDebug>

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDebuggerItem::LinkMotionDebuggerItem(ProjectExplorer::Abi abi) : Debugger::DebuggerItem() {
    qDebug() << Q_FUNC_INFO;
    setCommand(Utils::FileName::fromString(LinkMotion::Constants::LINKMOTION_GDB_COMMAND));
    setEngineType(Debugger::GdbEngineType);
    setUnexpandedDisplayName(QStringLiteral("GDB for LinkMotion"));
    setAutoDetected(true);
    setAbi(abi);
    reinitializeFromFile();
}

