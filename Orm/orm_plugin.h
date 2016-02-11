#ifndef ORM_PLUGIN_H
#define ORM_PLUGIN_H

#include <QQmlExtensionPlugin>

class OrmPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri);
};

#endif // ORM_PLUGIN_H

