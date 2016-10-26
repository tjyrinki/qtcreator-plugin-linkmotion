#ifndef LINKMOTIONDEBUGRUNCONTROL_H
#define LINKMOTIONDEBUGRUNCONTROL_H

#include <QObject>
#include <QDebug>
#include "../lmrunplugin/linkmotionruncontrol.h"

#include <debugger/debuggerruncontrol.h>
#include <utils/qtcprocess.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDebugRunControl : public QObject
{
    Q_OBJECT

public:
    static ProjectExplorer::RunControl *create(ProjectExplorer::RunConfiguration *runConfig, QString *errorMessage);

private:
    Debugger::DebuggerRunControl *m_runControl;
};

}
}

#endif // LINKMOTIONDEBUGRUNCONTROL_H
