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

#include <lmbaseplugin/lmsettingstargetwidget.h>
#include "ui_lmsettingstargetwidget.h"

#include <lmbaseplugin/lmtargettool.h>
#include <lmbaseplugin/lmtargetdialog.h>
#include "settings.h"

#include <QFileDialog>
#include <QDir>
#include <QRegExp>
#include <QTreeWidgetItem>
#include <QCheckBox>
#include <QDebug>

enum {
    debug = 0
};

namespace LmBase { namespace Internal {

LinkMotionSettingsTargetWidget::LinkMotionSettingsTargetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkMotionSettingsTargetWidget)
{
    ui->setupUi(this);

    //hide this setting for the moment, as we have no local mirror for lxd images
    ui->checkBoxLocalMirror->setVisible(false);

    Settings::TargetSettings def = Settings::chrootSettings();
    ui->checkBoxLocalMirror->setChecked(def.useLocalMirror);

    Settings::ImageServerCredentials creds = Settings::imageServerCredentials();
    ui->groupBoxAuth->setChecked(creds.useCredentials);
    ui->lineEditUser->setText(creds.user);
    ui->lineEditPass->setText(creds.pass);

    m_deleteMapper = new QSignalMapper(this);
    connect(m_deleteMapper, SIGNAL(mapped(int)),this, SLOT(on_deleteTarget(int)));
    m_maintainMapper = new QSignalMapper(this);
    connect(m_maintainMapper, SIGNAL(mapped(int)),this, SLOT(on_maintainTarget(int)));
    m_updateMapper = new QSignalMapper(this);
    connect(m_updateMapper, SIGNAL(mapped(int)),this, SLOT(on_upgradeTarget(int)));

    QStringList headers;
    headers << tr("Name")<< tr("Distribution")<< tr("Version") << tr("Architecture") << QLatin1String("")<<QLatin1String("")<<QLatin1String("");
    ui->treeWidgetClickTargets->setHeaderLabels(headers);
    ui->treeWidgetClickTargets->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidgetClickTargets->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeWidgetClickTargets->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidgetClickTargets->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeWidgetClickTargets->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->treeWidgetClickTargets->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->treeWidgetClickTargets->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    listExistingClickTargets();
}

void LinkMotionSettingsTargetWidget::apply() {

    Settings::TargetSettings set;
    set.useLocalMirror = ui->checkBoxLocalMirror->checkState() == Qt::Checked;
    Settings::setChrootSettings(set);

    Settings::ImageServerCredentials creds;
    creds.useCredentials = ui->groupBoxAuth->isChecked();
    creds.user = ui->lineEditUser->text();
    creds.pass = ui->lineEditPass->text();
    Settings::setImageServerCredentials(creds);

    Settings::flushSettings();
}

LinkMotionSettingsTargetWidget::~LinkMotionSettingsTargetWidget()
{
    delete ui;
}

void LinkMotionSettingsTargetWidget::on_pushButtonCreateClickTarget_clicked()
{
    //make sure the current settings are stored
    apply();

    Internal::LinkMotionTargetDialog::createTargetModal(true, this);
    listExistingClickTargets();
}

void LinkMotionSettingsTargetWidget::on_deleteTarget(const int index)
{
    if(index < 0 || index > m_availableTargets.size())
        return;

    if(debug) qDebug()<<"Destroying target "<< m_availableTargets.at(index);

    Internal::LinkMotionTargetDialog::maintainTargetModal(m_availableTargets.at(index),LinkMotionTargetTool::Delete);
    listExistingClickTargets();
}

void LinkMotionSettingsTargetWidget::on_maintainTarget(const int index)
{
    if(index < 0 || index > m_availableTargets.size())
        return;
    LinkMotionTargetTool::openTargetTerminal(m_availableTargets.at(index));
}

void LinkMotionSettingsTargetWidget::on_upgradeTarget(const int index)
{
    if(index < 0 || index > m_availableTargets.size())
        return;
    Internal::LinkMotionTargetDialog::maintainTargetModal(m_availableTargets.at(index),LinkMotionTargetTool::Upgrade);
}

/**
 * @brief UbuntuSettingsClickWidget::listExistingClickTargets
 * Lists all existing link motion targets
 */
void LinkMotionSettingsTargetWidget::listExistingClickTargets()
{
    //this should hopefully also delete all mapped pushbuttons
    ui->treeWidgetClickTargets->clear();

    QList<LinkMotionTargetTool::Target> items = LinkMotionTargetTool::listAvailableTargets();
    m_availableTargets = items;

    QAbstractItemModel* model = ui->treeWidgetClickTargets->model();

    //fill the treeview with all existing chroots
    for(int i = 0; i < items.size(); i++) {
        const LinkMotionTargetTool::Target& target = items.at(i);

        QTreeWidgetItem* chrootItem = new QTreeWidgetItem;
        chrootItem->setText(0,target.containerName);
        chrootItem->setText(1,target.distribution);
        chrootItem->setText(2,target.version);
        chrootItem->setText(3,target.architecture);
        ui->treeWidgetClickTargets->addTopLevelItem(chrootItem);

        QPushButton* push = new QPushButton(tr("Update"));
        m_updateMapper->setMapping(push,i);
        connect(push,SIGNAL(clicked()),m_updateMapper,SLOT(map()));
        ui->treeWidgetClickTargets->setIndexWidget(model->index(i,4), push);

        push = new QPushButton(tr("Maintain"));
        m_maintainMapper->setMapping(push,i);
        connect(push,SIGNAL(clicked()),m_maintainMapper,SLOT(map()));
        ui->treeWidgetClickTargets->setIndexWidget(model->index(i,5), push);

        push = new QPushButton(tr("Delete"));
        m_deleteMapper->setMapping(push,i);
        connect(push,SIGNAL(clicked()),m_deleteMapper,SLOT(map()));
        ui->treeWidgetClickTargets->setIndexWidget(model->index(i,6), push);
    }
}

}}
