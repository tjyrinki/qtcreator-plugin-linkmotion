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

    void slotStart_Finished(int, QProcess::ExitStatus);
    void slotStart_StateChanged(QProcess::ProcessState);
    void slotStart_ReadyRead();
    void slotStart_ReadyReadStandardError();
    void slotStart_ReadyReadStandardOutput();

    void slotStop_Finished(int, QProcess::ExitStatus);
    void slotStop_StateChanged(QProcess::ProcessState);
    void slotStop_ReadyRead();
    void slotStop_ReadyReadStandardError();
    void slotStop_ReadyReadStandardOutput();

protected:
    ProjectExplorer::RunConfiguration* m_runConfig;
    Debugger::AnalyzerRunControl* m_runControl;
    Utils::QtcProcess m_processStart;
    Utils::QtcProcess m_processStop;
};

}
}

#endif // LINKMOTIONANALYZERUNRUNNER_H
