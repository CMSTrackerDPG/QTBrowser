#ifndef PREVIEWPLUGIN_H
#define PREVIEWPLUGIN_H

#include <QWidget>
#include <TApplication.h>
#include <QListWidgetItem>

#include "iplugin.h"

namespace Ui {
class PreviewPlugin;
}

class PreviewPlugin : public QWidget , public IPlugin
{
    Q_OBJECT

public:
    PreviewPlugin(QWidget *parent = 0);
    ~PreviewPlugin();

    // IPlugin interface
public slots:
    void receiveTObjectContainer(TObjectContainer& container) override;

private:
    TH1* current_obj {nullptr};
    Ui::PreviewPlugin *ui;
};

#endif // PREVIEWPLUGIN_H
