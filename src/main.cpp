#include "kakounecli.hpp"
#include "kakouneserver.hpp"
#include "mainwindow.hpp"
#include <QApplication>

#define DBUS_SERVICE_NAME "com.github.falbru.KakouneQt"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc > 1 && strcmp(argv[1], "cli") == 0)
    {
        KakouneCli cli;

        QStringList command;
        for (int i = 2; i < argc; i++)
        {
            command.append(argv[i]);
        }

        return cli.run(command);
    }

    MainWindow w;

    KakouneServer server("KakouneQt.123");
    server.bind(&w);

    w.show();
    return app.exec();
}
