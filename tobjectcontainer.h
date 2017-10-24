#ifndef TOBJECTCONTAINER_H
#define TOBJECTCONTAINER_H

#include <QStandardItem>
#include <TObject.h>

class TObjectContainer
{
public:
    TObjectContainer(TObject* o) : obj(o){}
    TObject* getObject() { return obj; }
    QString getName() { return QString(obj->GetTitle()); }
private:
    TObject* obj{nullptr};
};
Q_DECLARE_METATYPE(TObjectContainer*)

#endif // TOBJECTCONTAINER_H
