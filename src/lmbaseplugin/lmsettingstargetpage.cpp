/*
 * Copyright 2013 Canonical Ltd.
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
 */

#include "lmsettingstargetpage.h"
#include <lmbaseplugin/lmbaseplugin_constants.h>

using namespace LmBase::Internal;

LinkMotionSettingsTargetPage::LinkMotionSettingsTargetPage() :
    m_widget(0)
{
    setId("A.LinkMotion.Target");
    setDisplayName(tr("Link Motion Target"));
    setCategory("Link Motion");
    setDisplayCategory(QStringLiteral("Link Motion"));
    //setCategoryIcon(QLatin1String(Ubuntu::Constants::UBUNTU_SETTINGS_ICON));
}

LinkMotionSettingsTargetPage::~LinkMotionSettingsTargetPage()
{
}

QWidget *LinkMotionSettingsTargetPage::widget( )
{
    if(!m_widget)
        m_widget = new LinkMotionSettingsTargetWidget();
    return m_widget;
}

void LinkMotionSettingsTargetPage::apply()
{
    if (!m_widget) // page was never shown
        return;

    m_widget->apply();
}

void LinkMotionSettingsTargetPage::finish()
{
    if (m_widget)
        delete m_widget;
}
