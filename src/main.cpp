#include "ipc.hpp"
#include "kakounesession.hpp"
#include "mainwindow.hpp"
#include <QApplication>
#include <qcommandlineoption.h>
#include <qcommandlineparser.h>
#include <qcoreapplication.h>
#include <qjsonobject.h>

int main(int argc, char *argv[])
{
    bool is_cli_mode = (argc > 1 && QString(argv[1]) == "cli");


    if (is_cli_mode) {
        QCoreApplication cli_app(argc, argv);

        QCommandLineParser parser;
        const QCommandLineOption helpOption = parser.addHelpOption();

        parser.addPositionalArgument("subcommand",
            "new-client: Create a new Kakoune client with <args>\n"
            "focus: Focus the Kakoune client with <name>\n"
            "rename-session: Rename the Kakoune session to <id>\n"
            );

        parser.process(cli_app);

        const QStringList positional_arguments = parser.positionalArguments().mid(1);

        if (positional_arguments.size() == 0 || parser.isSet(helpOption)) {
            parser.showHelp();
            return 0;
        }

        KakouneIPC::IPCClient ipc;

        const QString subcommand = positional_arguments[0];

        if (subcommand == "new-client")
        {
            ipc.send("newClient", {{"args", positional_arguments.mid(1).join(" ")}});
        }else if (subcommand == "focus")
        {
            if (positional_arguments.size() < 2) return 1;
            ipc.send("focusWindow", {{"client_name", positional_arguments[1]}});
        }else if (subcommand == "rename-session")
        {
            if (positional_arguments.size() < 2) return 1;
            ipc.send("renameSession", {{"session_name", positional_arguments[1]}});
        }else
        {
            return 1;
        }

        return 0;
    }else {
        QApplication app(argc, argv);

        QCommandLineParser parser;
        parser.addPositionalArgument("mode", "cli: Use the CLI interface");
        parser.setApplicationDescription("Kakoune Qt - A Qt-based frontend for Kakoune.");
        const QCommandLineOption helpOption = parser.addHelpOption();
        const QCommandLineOption versionOption = parser.addVersionOption();

        parser.process(app);

        if (parser.isSet(helpOption)) {
            parser.showHelp();
            return 0;
        }
        if (parser.isSet(versionOption)) {
            parser.showVersion();
            return 0;
        }

        MainWindow main_window(KakouneSession::generateRandomSessionId());
        KakouneIPC::IPCServer server(main_window.getID().toString());
        server.bind(&main_window);

        main_window.show();

        return app.exec();
    }
}
