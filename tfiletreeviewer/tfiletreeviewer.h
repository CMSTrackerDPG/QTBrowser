#ifndef TFILETREEVIEWER_H
#define TFILETREEVIEWER_H

#include <QWidget>

namespace Ui {
class TFileTreeViewer;
}

class TFileTreeViewer : public QWidget
{
    Q_OBJECT

public:
    explicit TFileTreeViewer(QWidget *parent = 0);
    ~TFileTreeViewer();

private:
    Ui::TFileTreeViewer *ui;
};

#endif // TFILETREEVIEWER_H
