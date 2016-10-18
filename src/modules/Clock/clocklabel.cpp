#include "clocklabel.h"

ClockLabel::ClockLabel(QQuickItem *parent):
    QQuickLabel(parent)
{
    m_timeSync.setSingleShot(false);
    m_timeSync.setInterval(1000);
    m_bBlinkEnabled = false;
    m_bBlinkState = true;
    connect(&m_timeSync,SIGNAL(timeout()),this,SLOT(onTimeSync()));
    onTimeSync();
    m_timeSync.start();
}

ClockLabel::~ClockLabel() {
    disconnect(&m_timeSync);
    m_timeSync.stop();
}

void ClockLabel::onTimeSync() {
    QString timeFormat = "hh:mm:ss";
    if (m_bBlinkEnabled) {
        if (m_bBlinkState) {
            timeFormat = "hh mm ss";
        }
        m_bBlinkState = !m_bBlinkState;
    }
    setText(QTime::currentTime().toString(timeFormat));
}
