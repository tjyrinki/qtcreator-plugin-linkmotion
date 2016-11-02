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
#ifndef LINKMOTIONDEBUGGERITEM_H
#define LINKMOTIONDEBUGGERITEM_H

#include <QObject>
#include <debugger/debuggeritem.h>
#include <debugger/debuggeritemmanager.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDebuggerItem : public Debugger::DebuggerItem
{
public:
    LinkMotionDebuggerItem(ProjectExplorer::Abi abi);
};

}
}

#endif // LINKMOTIONDEBUGGERITEM_H
