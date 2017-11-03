#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QWidget>
#include <QDebug>

#include "tobjectcontainer.h"

#include <TH1.h>

namespace Ui {
class IPlugin;
}

class IPlugin
{
public:
    virtual ~IPlugin() {}

public slots:
    virtual void receiveTObjectContainer(TObjectContainer& container) = 0;
};

#endif // IPLUGIN_H
