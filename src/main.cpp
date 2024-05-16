#include "kakounecli.hpp"
#include "kakouneserver.hpp"
#include "mainwindow.hpp"
#include <QApplication>

QString generateRandomKakouneSessionId()
{
    return QString::number(QRandomGenerator::global()->bounded(1000, 9999));
}

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

    QString session_id = generateRandomKakouneSessionId();

    MainWindow w(session_id);
    KakouneServer server("KakouneQt." + session_id);
    server.bind(&w);

    w.show();
    return app.exec();
}
