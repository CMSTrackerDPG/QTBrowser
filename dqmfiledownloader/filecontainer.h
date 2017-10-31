#ifndef FILECONTAINER_H
#define FILECONTAINER_H

#include <QString>

class FileContainer {
public:
    FileContainer(QString name, QString path) {
        display_name = name;
        file_path = path;
    }

    QString getName() const { return display_name; }
    QString getPath() const { return file_path;    }

private:
    QString display_name;
    QString file_path;
};

#endif // FILECONTAINER_H
