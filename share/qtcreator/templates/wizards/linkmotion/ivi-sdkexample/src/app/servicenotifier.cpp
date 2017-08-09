#include "servicenotifier.h"

void ServiceNotifier::appReady() {
	sd_notify(0, "READY=1");
	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(watchdog()));
	m_timer.start();
}


void ServiceNotifier::watchdog() {
	sd_notify(0, "WATCHDOG=1");
}

