/*
 * Copyright 2014 Canonical Ltd.
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
#include "createtargetwizard.h"
#include "ui_createtargetimagepage.h"
#include "ui_createtargetintropage.h"
#include "ui_createtargetnamepage.h"

#include <lmbaseplugin/lmbaseplugin_constants.h>
#include <lmbaseplugin/lmtoolchain.h>

#include <coreplugin/icore.h>

#include <QJsonDocument>
#include <QMessageBox>
#include <QRadioButton>
#include <QFormLayout>
#include <QSpacerItem>

namespace LmBase {

namespace Constants {
    enum {
        INDEX_DATA = 0,
        INDEX_LOADING = 1,
        INDEX_ERROR = 2
    };

    const QString ARCH_FIELD(QStringLiteral("deviceArch"));
    const QString DISTRO_FIELD(QStringLiteral("distroName"));
    const QString VERSION_FIELD(QStringLiteral("distroVersion"));

    const int DistributionRole = Qt::UserRole;
    const int VersionRole = Qt::UserRole+1;
    const int HostArchRole = Qt::UserRole+2;
    const int BuildArchRole = Qt::UserRole+3;
}

namespace Internal {

CreateTargetWizard::CreateTargetWizard(QWidget *parent)
    : Utils::Wizard(parent)
{
    m_introPage = new CreateTargetIntroPage(this);
    addPage(m_introPage);

    m_imageSelectPage = new CreateTargetImagePage(this);
    m_imageSelectPage->setImageType(m_introPage->imageType());
    addPage(m_imageSelectPage);

    m_namePage = new CreateTargetNamePage(this);
    m_namePage->setImageType(m_introPage->imageType());
    addPage(m_namePage);

    connect(m_introPage, &CreateTargetIntroPage::imageTypeChanged,
            m_imageSelectPage, &CreateTargetImagePage::setImageType);

    connect(m_introPage, &CreateTargetIntroPage::imageTypeChanged,
            m_namePage, &CreateTargetNamePage::setImageType);

    setMinimumSize(800,400);
}


CreateTargetWizard::CreateTargetWizard(const QString &arch, const QString &framework, QWidget *parent)
    : Utils::Wizard(parent)
{
    m_introPage = nullptr;
    m_imageSelectPage = new CreateTargetImagePage(this);
    m_imageSelectPage->setFilter(arch, framework);
    addPage(m_imageSelectPage);

    m_namePage = new CreateTargetNamePage(this);
    addPage(m_namePage);

    setMinimumSize(800,400);
}

/**
 * @brief UbuntuCreateNewChrootDialog::getNewChrootParams
 * Opens a dialog that lets the user select a new chroot, returns false
 * if the user pressed cancel
 */
bool CreateTargetWizard::getNewTarget(LinkMotionTargetTool::Target *target, QWidget *parent)
{
    CreateTargetWizard dlg(parent ? parent : Core::ICore::mainWindow());
    return doSelectImage(dlg, target);
}

/**
 * @brief UbuntuCreateNewChrootDialog::getNewChrootParams
 * Opens the CreateTargetWizard but skips the intro page and applies the given filters
 */
bool CreateTargetWizard::getNewTarget(LinkMotionTargetTool::Target *target, const QString &arch, const QString &framework, QWidget *parent)
{

    CreateTargetWizard dlg(arch, framework, parent ? parent : Core::ICore::mainWindow());
    return doSelectImage(dlg, target);
}

bool CreateTargetWizard::doSelectImage (CreateTargetWizard &dlg, LinkMotionTargetTool::Target *target)
{
    if( dlg.exec() == QDialog::Accepted) {
        target->imageArchitecture = dlg.m_imageSelectPage->selectedHostArchitecture();
        target->architecture = dlg.m_imageSelectPage->selectedDeviceArchitecture();
        target->distribution = dlg.m_imageSelectPage->selectedDistribution();
        target->version = dlg.m_imageSelectPage->selectedDistributionVersion();

        if (target->architecture.isEmpty()
                || target->distribution.isEmpty()
                || target->version.isEmpty())
            return false;

        target->containerName = dlg.m_namePage->chosenName();
        return true;
    }
    return false;
}

CreateTargetIntroPage::CreateTargetIntroPage(QWidget *parent)
    : Utils::WizardPage(parent)
    , ui(new Ui::CreateTargetIntroPage)

{
    ui->setupUi(this);

    m_imageTypeGroup = new QButtonGroup(this);
    m_imageTypeGroup->addButton(ui->desktopButton, CreateTargetWizard::DesktopImage);
    m_imageTypeGroup->addButton(ui->deviceButton, CreateTargetWizard::DeviceImage);
    m_imageTypeGroup->addButton(ui->allImagesButton, CreateTargetWizard::AllImages);
    m_imageTypeGroup->button(CreateTargetWizard::DesktopImage)->setChecked(true);

    connect(m_imageTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonSelected(int)));

    setTitle(tr("Target creation"));
    setProperty(Utils::SHORT_TITLE_PROPERTY, tr("Intro"));
}

CreateTargetIntroPage::~CreateTargetIntroPage()
{
    delete ui;
}

CreateTargetWizard::ImageType CreateTargetIntroPage::imageType() const
{
    return static_cast<CreateTargetWizard::ImageType>(m_imageTypeGroup->checkedId());
}

void CreateTargetIntroPage::buttonSelected(const int id)
{
    emit imageTypeChanged(static_cast<CreateTargetWizard::ImageType>(id));
}

CreateTargetImagePage::CreateTargetImagePage(QWidget *parent) :
    Utils::WizardPage(parent),
    m_loader(nullptr),
    ui(new Ui::CreateTargetImagePage)
{
    ui->setupUi(this);
    ui->progressBar->setRange(0, 0);
    ui->stackedWidget->setCurrentIndex(Constants::INDEX_DATA);
    ui->treeWidgetImages->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    setTitle(tr("Please select the image:"));
    setProperty(Utils::SHORT_TITLE_PROPERTY, tr("Image"));

    registerField(Constants::DISTRO_FIELD, this, "selectedDistribution");
    registerField(Constants::VERSION_FIELD, this, "selectedDistributionVersion");
    registerField(Constants::ARCH_FIELD, this, "selectedDeviceArchitecture");
}

CreateTargetImagePage::~CreateTargetImagePage()
{
    delete ui;
}

void CreateTargetImagePage::setImageType(CreateTargetWizard::ImageType imageType)
{
    if (imageType == CreateTargetWizard::DesktopImage) {
        setFilter(LinkMotionTargetTool::hostArchitecture(), QString());
    } else if (imageType == CreateTargetWizard::DeviceImage){
        setFilter(QStringLiteral("(i386|aarch64|armhf)"), QString());
    } else {
        setFilter(QString(), QString());
    }
}

void CreateTargetImagePage::setFilter(const QString &arch, const QString &framework)
{
    QString fwFilter, archFilter = fwFilter = QStringLiteral("(.*)");
    if(!framework.isEmpty())
        fwFilter = framework;
    if(!arch.isEmpty())
        archFilter = arch;

    m_filter = QString::fromLatin1("^%1-(.*)-%2-(dev|ota[0-9]+)$").arg(fwFilter).arg(archFilter);
    load();
}

QString CreateTargetImagePage::selectedDeviceArchitecture() const
{
    QTreeWidgetItem * item = ui->treeWidgetImages->currentItem();
    if (!item)
        return QString();
    return item->data(0, Constants::BuildArchRole).toString();
}

QString CreateTargetImagePage::selectedHostArchitecture() const
{
    QTreeWidgetItem * item = ui->treeWidgetImages->currentItem();
    if (!item)
        return QString();
    return item->data(0, Constants::HostArchRole).toString();
}

QString CreateTargetImagePage::selectedDistribution() const
{
    QTreeWidgetItem * item = ui->treeWidgetImages->currentItem();
    if (!item)
        return QString();
    return item->data(0, Constants::DistributionRole).toString();
}

QString CreateTargetImagePage::selectedDistributionVersion() const
{
    QTreeWidgetItem * item = ui->treeWidgetImages->currentItem();
    if (!item)
        return QString();
    return item->data(0, Constants::VersionRole).toString();
}

void CreateTargetImagePage::initializePage()
{
    load();
}

bool CreateTargetImagePage::validatePage()
{
    if (!ui->treeWidgetImages->currentItem()) {
        QMessageBox::warning(this, tr("No image selected"), tr("Please select a Image to continue."));
        return false;
    }
    return true;
}

void CreateTargetImagePage::load()
{
    if (m_loader) {
        m_loader->disconnect(this);
        if (m_loader->state() != QProcess::NotRunning) {
            m_loader->kill();
            m_loader->waitForFinished(1000);
        }
        m_loader->deleteLater();
        m_loader = nullptr;
    }

    ui->treeWidgetImages->clear();
    ui->stackedWidget->setCurrentIndex(Constants::INDEX_LOADING);

    m_loader = new QProcess(this);
    connect(m_loader, &QProcess::errorOccurred, this, &CreateTargetImagePage::loaderErrorOccurred);
    connect(m_loader, SIGNAL(finished(int)), this, SLOT(loaderFinished()));
    m_loader->setProgram(Constants::LM_TARGET_TOOL);
    m_loader->setArguments(QStringList{QStringLiteral("images")});

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("LC_ALL"), QStringLiteral("C"));
    m_loader->setProcessEnvironment(env);

    m_loader->start();
}

void CreateTargetImagePage::loaderErrorOccurred(QProcess::ProcessError)
{
    ui->stackedWidget->setCurrentIndex(Constants::INDEX_ERROR);
    ui->errorLabel->setText(QStringLiteral("Error loading the image index from the server"));
}

void CreateTargetImagePage::loaderFinished()
{
    if (m_loader->exitCode() != 0) {
        ui->stackedWidget->setCurrentIndex(Constants::INDEX_ERROR);
        ui->errorLabel->setText(QStringLiteral("Error loading the image index from the server"));
        return;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(m_loader->readAllStandardOutput(), &err);
    if (err.error != QJsonParseError::NoError) {
        ui->stackedWidget->setCurrentIndex(Constants::INDEX_ERROR);
        ui->errorLabel->setText(QString::fromLatin1("Error while parsing the response from the server\n%1")
                                .arg(err.errorString()));
        return;
    }

    qDebug()<<"Filter is "<<m_filter;
    QRegularExpression expr(m_filter);
    QList<QVariant> data = doc.toVariant().toList();
    foreach (const QVariant &entry, data) {

        QVariantMap m = entry.toMap();
        QString distribution = m.value(QStringLiteral("distribution"), QStringLiteral("error")).toString();
        if (distribution == QStringLiteral("error"))
            continue;

        QString version = m.value(QStringLiteral("version"), QStringLiteral("error")).toString();
        if (version == QStringLiteral("error"))
            continue;

        //@TODO enable filtering again
        /*
        if(!expr.match(alias).hasMatch())
            continue;
            */

        //check arch compat
        QString arch = m.value(QStringLiteral("arch"), QStringLiteral("error")).toString();
        if (!LinkMotionToolChain::supportsArchitecture(arch))
            continue;

        QString variant = m.value(QStringLiteral("variant"), QStringLiteral("error")).toString();
        if (!LinkMotionToolChain::supportsArchitecture(arch))
            continue;

        if (!LinkMotionTargetTool::compatibleWithHostArchitecture(arch))
            continue;

#if 0
        LinkMotionTargetTool::Target target;
        QStringList extensions;
        if (!LinkMotionTargetTool::parseContainerName(alias, &target, &extensions))
            continue;
#endif

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, distribution);
        item->setData(0, Constants::DistributionRole, distribution);
        item->setData(0, Constants::VersionRole, version);
        item->setData(0, Constants::HostArchRole, arch);
        item->setData(0, Constants::BuildArchRole, variant);

        item->setText(1, version);
        item->setText(2, variant);
        item->setText(3, m.value(QStringLiteral("uploadDate"), QStringLiteral("error")).toString());
        ui->treeWidgetImages->addTopLevelItem(item);
    }
    ui->stackedWidget->setCurrentIndex(Constants::INDEX_DATA);
    ui->treeWidgetImages->setCurrentItem(ui->treeWidgetImages->topLevelItem(0));
}

CreateTargetNamePage::CreateTargetNamePage(QWidget *parent) : Utils::WizardPage(parent),
    ui(new Ui::CreateTargetNamePage) ,
    m_imageType(-1)
{
    ui->setupUi(this);
    ui->lineEditName->setValidationFunction([](Utils::FancyLineEdit *edit, QString *errorMessage) {
        if (edit->text().isEmpty()) {
            if (errorMessage)
                *errorMessage = tr("Name can not be empty");
            return false;
        }

        QRegularExpression hostnameRegEx(QStringLiteral("^[A-Za-z0-9\\-_]+$"));
        if (!hostnameRegEx.match(edit->text()).hasMatch()){
            if (errorMessage)
                *errorMessage = tr("Name can only contain letters, numbers,dash and underscore");
            return false;
        }

        return true;
    });
    ui->lineEditName->setPlaceholderText(tr("Please select a name"));
    ui->lineEditName->validate();

    setTitle(tr("Please type a name:"));
    setProperty(Utils::SHORT_TITLE_PROPERTY, tr("Name"));
}

CreateTargetNamePage::~CreateTargetNamePage()
{
    delete ui;
}

void CreateTargetNamePage::setImageType(CreateTargetWizard::ImageType imageType)
{
    m_imageType = imageType;
}

QString CreateTargetNamePage::chosenName() const
{
    return ui->lineEditName->text();
}

void CreateTargetNamePage::initializePage()
{
    QString name;
    switch(m_imageType) {
        case CreateTargetWizard::DesktopImage:
            name = QStringLiteral("desktop");
            break;
        case CreateTargetWizard::DeviceImage:
            name = QString::fromLatin1("device-%1").arg(field(Constants::ARCH_FIELD).toString());
            break;
        default:
            name = QString::fromLatin1("builder-%1").arg(field(Constants::ARCH_FIELD).toString());
            break;
    }

    if (!name.isEmpty())
        ui->lineEditName->setText(name);
}

bool CreateTargetNamePage::validatePage()
{
    if (!ui->lineEditName->isValid()) {
        return false;
    }
    return true;
}

} // namespace Internal
} // namespace Ubuntu