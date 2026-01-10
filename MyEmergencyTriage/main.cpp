#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "TriageBridge.hpp"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Create an instance of TriageBridge and expose it to QML
    TriageBridge triageSystem;
    engine.rootContext()->setContextProperty("TriageSystem", &triageSystem);

    // Load the QML file
    const QUrl url(QStringLiteral("MyEmergencyTriage/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl) {
                             QCoreApplication::exit(-1);
                         }
                     }, Qt::QueuedConnection);

    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
