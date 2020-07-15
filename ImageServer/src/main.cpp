#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("LiveView-ImageServer");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Image Ser");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));

    QCommandLineOption portOption(QStringList() << "p" << "custom-port",
        QCoreApplication::translate("main", "Start Image Server on manual port: <custom-port>."),
        QCoreApplication::translate("main", "custom-port"));
    parser.addOption(portOption);

    parser.process(a);

    const QStringList args = parser.positionalArguments();

    int port = parser.value(portOption).toInt();

    Server s;

    // The following code can be optimized
    if (!s.StartServer(port)) {
        a.quit();
    }

    return a.exec();
}
