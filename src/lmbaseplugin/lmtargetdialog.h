/*
 * Copyright 2016 Canonical Ltd.
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
 * Author: Benjamin Zeller <benjamin.zeller@canonical.com>
 */


#include <lmbaseplugin/processoutputdialog.h>
#include <lmbaseplugin/lmtargettool.h>

namespace LmBase {
namespace Internal {

class LinkMotionTargetDialog : public ProcessOutputDialog
{

public:
    static bool createTargetModal (bool redetectKits = true, QWidget *parent = 0);
    static bool createTargetModal (bool redetectKits = true , const QString &arch = QString(), QWidget *parent = 0);
    static int maintainTargetModal (const LinkMotionTargetTool::Target &target, const LinkMotionTargetTool::MaintainMode &mode);
    static int maintainTargetModal (const QList<LinkMotionTargetTool::Target> &targetList, const LinkMotionTargetTool::MaintainMode &mode);

protected:
    static bool doCreateTarget(bool redetectKits, const LinkMotionTargetTool::Target &t, QWidget *parent);
};

}}
