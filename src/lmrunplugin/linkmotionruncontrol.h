/*####################################################################
#
# This file is part of the LinkMotion Run plugin.
#
# License: Proprietary
# Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
#
# All rights reserved.
# (C) 2016 Link Motion Oy
####################################################################*/
#ifndef LINKMOTIONRUNCONTROL_H
#define LINKMOTIONRUNCONTROL_H

#include <QObject>
#include <utils/qtcprocess.h>
#include "linkmotionrunconfiguration.h"
#include <projectexplorer/runconfiguration.h>

namespace LinkMotion {
namespace Internal {

class LinkMotionRunControl : public ProjectExplorer::RunControl
{
    Q_OBJECT

public:
    explicit LinkMotionRunControl(LinkMotionRunConfiguration *runConfig);
    ~LinkMotionRunControl();

    void start();
    StopResult stop();
    bool isRunning() const;
    QString displayName() const;

protected slots:
    void emulateStop() { stop(); }

    void slotStart_StdOut();
    void slotStart_StdErr();
    void slotStart_Finished(int, QProcess::ExitStatus);
    void slotStart_Error(QProcess::ProcessError err);

    void slotStop_StdOut();
    void slotStop_StdErr();
    void slotStop_Finished(int, QProcess::ExitStatus);
    void slotStop_Error(QProcess::ProcessError err);

private:
    Utils::QtcProcess m_processStart;
    Utils::QtcProcess m_processStop;
    bool m_running;
};

}
}

#endif // LINKMOTIONRUNCONTROL_H
