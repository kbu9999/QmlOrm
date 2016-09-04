#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "QSortFilterProxyModel"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qRegisterMetaType<QObjectList>();
    qRegisterMetaType<QHash<int, QByteArray> >();

    engine.addImportPath("../");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

