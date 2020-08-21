#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("LiveView-ImageServer");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Image Server");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption portOption(QStringList() << "p" << "port",
        QCoreApplication::translate("main", "Start Image Server on manual port: <custom-port>."),
        QCoreApplication::translate("main", "port"));

    QCommandLineOption dirOption(QStringList() << "d" << "directory",
        QCoreApplication::translate("main", "Start Image Server on custom directory: <custom-directory>."),
        QCoreApplication::translate("main", "directory"));

    QCommandLineOption typeOption(QStringList() << "e" << "envi", QCoreApplication::translate("main", "Start Image Server to read '.envi' files. Default is '.xio'"));

    parser.addOption(portOption);
    parser.addOption(dirOption);
    parser.addOption(typeOption);

    parser.process(a.arguments());

    const QStringList args = parser.positionalArguments();

    int port = parser.value(portOption).toInt();
    QString directory = parser.value(dirOption);
    bool envi = parser.isSet(typeOption);

    // Start the server with the arguments from the command line
    Server s;
    if (!s.StartServer(port, directory, envi)) {
        a.quit();
    }

    return a.exec();
}
