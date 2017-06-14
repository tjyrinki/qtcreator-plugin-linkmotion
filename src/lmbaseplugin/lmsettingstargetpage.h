/*
 * Copyright 2013 - 2017 Canonical Ltd.
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
 * Author: Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
 * Author: Benjamin Zeller <benjamin.zeller@link-motion.com>
 */


#ifndef LINKMOTION_SETTINGS_TARGET_PAGE_H_INCLUDED
#define LINKMOTION_SETTINGS_TARGET_PAGE_H_INCLUDED

#include <coreplugin/dialogs/ioptionspage.h>
#include "lmsettingstargetwidget.h"
#include <QPointer>

namespace LmBase {
    namespace Internal {
        class LinkMotionSettingsTargetPage : public Core::IOptionsPage
        {
            Q_OBJECT

        public:
            explicit LinkMotionSettingsTargetPage();
            ~LinkMotionSettingsTargetPage();

            QWidget *widget( ) override;
            void apply() override;
            void finish() override;

        protected:
            QPointer<LinkMotionSettingsTargetWidget> m_widget;
        };
    }
}


#endif // LINKMOTION_SETTINGS_TARGET_PAGE_H_INCLUDED
