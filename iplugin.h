#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QWidget>
#include <QDebug>

#include <TH1.h>

namespace Ui {
class IPlugin;
}

class IPlugin
{
public:
    virtual ~IPlugin() {}

public slots:
    virtual void receiveTObj(TObject* obj) = 0;
};

#endif // IPLUGIN_H
