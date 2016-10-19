#ifndef LINKMOTIONDEBUGRUNCONTROLFACTORY_H
#define LINKMOTIONDEBUGRUNCONTROLFACTORY_H

#include <QObject>
#include <QDebug>
#include <projectexplorer/runconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionDebugRunControlFactory : public ProjectExplorer::IRunControlFactory
{
    Q_OBJECT

public:
    LinkMotionDebugRunControlFactory(QObject *parent = 0);

    bool canRun(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode) const;
    ProjectExplorer::RunControl *create(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode, QString *errorMessage);
};

}
}

#endif // LINKMOTIONDEBUGRUNCONTROLFACTORY_H
