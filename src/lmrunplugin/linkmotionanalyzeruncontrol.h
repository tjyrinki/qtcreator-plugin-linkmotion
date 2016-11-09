#ifndef LINKMOTIONANALYZERUNCONTROL_H
#define LINKMOTIONANALYZERUNCONTROL_H

#include <QObject>
#include "linkmotionruncontrol.h"
#include <utils/qtcprocess.h>

namespace Debugger { class AnalyzerRunControl; }

namespace LinkMotion {
namespace Internal {

class LinkMotionAnalyzeRunControl : public QObject
{
    Q_OBJECT

public:
    static ProjectExplorer::RunControl *create(LinkMotionRunConfiguration *runConfig, Core::Id runMode);

private:

    Debugger::AnalyzerRunControl *m_runControl;
    LinkMotionRunConfiguration *m_runConfig;
    Utils::QtcProcess m_process;
};

}
}


#endif // LINKMOTIONANALYZERUNCONTROL_H
