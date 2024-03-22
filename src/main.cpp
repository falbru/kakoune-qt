#include "kakounecli.hpp"
#include "mainwindow.hpp"
#include <QApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusInterface>
#include <QDBusReply>

#define DBUS_SERVICE_NAME "com.github.falbru.KakouneQt"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto connection = QDBusConnection::sessionBus();

    if (!connection.isConnected())
    {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "To start it, run:\n"
                 "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    if (argc > 1 && strcmp(argv[1], "cli") == 0)
    {
        KakouneCli cli(DBUS_SERVICE_NAME);

        QList<QString> command;
        for (int i = 2; i < argc; i++)
        {
            command.append(argv[i]);
        }

        return cli.run(command);
    }

    if (!connection.registerService(DBUS_SERVICE_NAME))
    {
        qWarning("%s\n", qPrintable(connection.lastError().message()));
        return 1;
    }

    MainWindow w;
    connection.registerObject("/", &w, QDBusConnection::ExportAllSlots);

    w.show();
    return app.exec();
}
