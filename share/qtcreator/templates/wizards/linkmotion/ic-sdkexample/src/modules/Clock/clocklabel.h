#ifndef CLOCKLABEL_H
#define CLOCKLABEL_H

#include <QQuickItem>
#include <QTime>
#include <QtQuickControls2>
#include <QtQuickTemplates2/QtQuickTemplates2>
#include <QtQuickControls2/private/qtquickcontrols2global_p.h>
#include <QtQuickTemplates2/private/qquicklabel_p.h>

class ClockLabel : public QQuickLabel
{
    Q_OBJECT
    Q_DISABLE_COPY(ClockLabel)
    Q_PROPERTY(bool blinkEnabled MEMBER m_bBlinkEnabled)

public:
    ClockLabel(QQuickItem *parent = 0);
    ~ClockLabel();

protected slots:
    void onTimeSync();

protected:
    QTimer m_timeSync;
    bool m_bBlinkEnabled;
    bool m_bBlinkState;
};

#endif // CLOCKLABEL_H
