/*
 * Copyright 2013 Canonical Ltd.
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
 * Author: Juhapekka Piiroinen <juhapekka.piiroinen@canonical.com>
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */

#ifndef UBUNTUWELCOMEMODE_H
#define UBUNTUWELCOMEMODE_H

#include <coreplugin/iwelcomepage.h>
#include <QQuickView>
#include <QObject>

namespace LmBase {
namespace Internal {

class LinkMotionWelcomePage : public Core::IWelcomePage
{
    Q_OBJECT

public:
    // IWelcomePage interface
    virtual QWidget *createWidget() const override;
    virtual QString title() const override;
    virtual int priority() const override;
    virtual Core::Id id() const override;


    QUrl pageLocation() const;
    void facilitateQml(QQmlEngine *engine) const;

public slots:
    void newProject();
    void openProject();
    void openGallery();
};

} // Internal
} // Ubuntu

#endif // UBUNTUWELCOMEMODE_H
