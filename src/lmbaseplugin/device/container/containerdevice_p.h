/*
 * Copyright 2016 Canonical Ltd.
 * Copyright 2017 Link Motion Oy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.1.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#ifndef CONTAINERDEVICE_P_H
#define CONTAINERDEVICE_P_H

#include <QObject>
#include <QProcess>

namespace LmBase {
namespace Internal {

class ContainerDevice;
class ContainerDevicePrivate : public QObject
{
    Q_OBJECT

public:
    ContainerDevicePrivate (LmBase::Internal::ContainerDevice *q);
    void resetProcess();
    QString userName () const;
    void reset ();

    void showWarningMessage (const QString &msg);

public slots:
    void handleDetectionStepFinished();

public:

    enum State {
        Initial,
        GetStatus,
        DeployKey,
        Finished
    } m_deviceState = Initial;
    QString m_deviceIP;

private:
    void printProcessError ();

private:
    ContainerDevice *q_ptr;
    QProcess *m_detectionProcess;
    Q_DECLARE_PUBLIC(ContainerDevice)
};


}}


#endif // CONTAINERDEVICE_P_H

