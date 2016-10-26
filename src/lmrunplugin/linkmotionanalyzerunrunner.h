#ifndef LINKMOTIONANALYZERUNRUNNER_H
#define LINKMOTIONANALYZERUNRUNNER_H

#include <QObject>
#include <projectexplorer/runconfiguration.h>
#include <debugger/debuggerruncontrol.h>
#include <utils/qtcprocess.h>
#include <QProcess>

namespace Debugger { class AnalyzerRunControl; }

namespace LinkMotion {
namespace Internal {

class LinkMotionAnalyzeRunRunner : public QObject
{
    Q_OBJECT
public:
    LinkMotionAnalyzeRunRunner(ProjectExplorer::RunConfiguration *runConfig,
                             Debugger::AnalyzerRunControl *runControl);

protected slots:
    void slotRunControl_Finished();
    void slotRunControl_Started();

    void slotProcess_Finished(int, QProcess::ExitStatus);
    void slotProcess_StateChanged(QProcess::ProcessState);
    void slotProcess_ReadyRead();
    void slotProcess_ReadyReadStandardError();
    void slotProcess_ReadyReadStandardOutput();

protected:
    ProjectExplorer::RunConfiguration* m_runConfig;
    Debugger::AnalyzerRunControl* m_runControl;
    Utils::QtcProcess m_process;
};

}
}

#endif // LINKMOTIONANALYZERUNRUNNER_H
