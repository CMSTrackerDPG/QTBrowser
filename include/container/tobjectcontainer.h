#ifndef TOBJECTCONTAINER_H
#define TOBJECTCONTAINER_H

#include <QStandardItem>
#include <TObject.h>

class TObjectContainer
{
public:    
    TObjectContainer(TObject* o,
                     QString parent_file_name_ = "",
                     QString path_in_parent_file_ = "")
        : obj(o) {
        path_in_parent_file = path_in_parent_file_;
        parent_file_name = parent_file_name_;
    }

    TObject* getObject()            { return obj; }
    QString getTitle()              { return QString(obj->GetTitle()); }
    QString getPathInParentFile()   { return path_in_parent_file; }
    QString getParentFileName()     { return parent_file_name; }
private:
    TObject* obj{nullptr};
    QString  path_in_parent_file;
    QString  parent_file_name;
};
Q_DECLARE_METATYPE(TObjectContainer*)

#endif // TOBJECTCONTAINER_H
