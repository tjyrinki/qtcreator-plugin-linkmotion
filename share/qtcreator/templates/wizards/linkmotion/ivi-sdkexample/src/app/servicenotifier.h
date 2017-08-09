#ifndef SERVICENOTIFIER_H
#define SERVICENOTIFIER_H

#include <QObject>
#include <QTimer>
#include <systemd/sd-daemon.h>

class ServiceNotifier : public QObject
{
	Q_OBJECT

public slots:
	void appReady();
	void watchdog();

private:
	QTimer m_timer;
};

#endif // SERVICENOTIFIER_H
