#include "linkmotiondebugplugin.h"
#include <QDebug>
#include "linkmotiondebugruncontrolfactory.h"

using namespace LinkMotion;
using namespace LinkMotion::Internal;

LinkMotionDebugPlugin::LinkMotionDebugPlugin() {
    qDebug() << Q_FUNC_INFO;
}

LinkMotionDebugPlugin::~LinkMotionDebugPlugin() {
    qDebug() << Q_FUNC_INFO;

}

bool LinkMotionDebugPlugin::initialize(const QStringList &arguments, QString *errorString) {
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)
    addAutoReleasedObject(new LinkMotionDebugRunControlFactory);

    return true;
}

void LinkMotionDebugPlugin::extensionsInitialized() {
    qDebug() << Q_FUNC_INFO;

}
