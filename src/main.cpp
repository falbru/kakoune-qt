#include "ipc.hpp"
#include "kakounesession.hpp"
#include "mainwindow.hpp"
#include "remotekakounesession.hpp"
#include <QApplication>
#include <qcommandlineoption.h>
#include <qcommandlineparser.h>
#include <qcoreapplication.h>
#include <qdebug.h>
#include <qhashfunctions.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qnamespace.h>
#include <qprocess.h>

int main(int argc, char *argv[])
{
    const QCommandLineOption cliOption("cli", "Use the CLI interface");
    bool is_cli_mode = (argc > 1 && QString(argv[1]) == "--cli");

    if (is_cli_mode)
    {
        QCoreApplication cli_app(argc, argv);

        QCommandLineParser parser;
        const QCommandLineOption helpOption = parser.addHelpOption();
        parser.addOption(cliOption);

        parser.addPositionalArgument("subcommand",
                                     "split-horizontal <args>: Create a new horizontal split <args>\n"
                                     "split-vertical <args>: Create a new vertical split <args>\n"
                                     "focus <name>: Focus the Kakoune client with <name>\n"
                                     "hide <name>: Hide the Kakoune client with <name>\n"
                                     "show <name>: Show the Kakoune client with <name>\n"
                                     "get-visible <name>: Returns true if the Kakoune client with <name> is visible\n"
                                     "rename-client <name>: Rename the Kakoune client to <name>\n"
                                     "rename-session <id>: Rename the Kakoune session to <id>\n");
        parser.process(cli_app);

        const QStringList positional_arguments = parser.positionalArguments();

        if (positional_arguments.size() == 0 || parser.isSet(helpOption))
        {
            parser.showHelp();
            return 0;
        }

        KakouneIPC::IPCClient ipc;

        const QString subcommand = positional_arguments[0];

        if (subcommand == "split-horizontal" || subcommand == "split-vertical")
        {
            auto orientation = subcommand == "split-vertical" ? "vertical" : "horizontal";
            ipc.send("newSplit", {{"args", positional_arguments.mid(1).join(" ")},
                                  {"client_name", QProcessEnvironment::systemEnvironment().value("KAKQT_CLIENT_ID")},
                                  {"orientation", orientation}});
        }
        else if (subcommand == "show" || subcommand == "hide")
        {
            if (positional_arguments.size() < 2)
                return 1;

            bool visible = subcommand == "show";
            ipc.send("setWindowVisible", {{"client_name", positional_arguments[1]}, {"visible", visible}});
        }
        else if (subcommand == "set-tabs")
        {
            if (positional_arguments.size() < 2)
                return 1;

            ipc.send("setTabs", {{"tabs", QJsonArray::fromStringList(positional_arguments.sliced(1))}});
        }
        else if (subcommand == "set-selected-tab")
        {
            if (positional_arguments.size() < 2)
                return 1;

            ipc.send("setSelectedTab", {{"bufname", positional_arguments[1]}});
        }
        else if (subcommand == "get-visible")
        {
            if (positional_arguments.size() < 2)
                return 1;
            ipc.send("getWindowVisible", {{"client_name", positional_arguments[1]}});

            QTextStream(stdout) << ipc.readResponse() << Qt::endl;
        }
        else if (subcommand == "focus")
        {
            if (positional_arguments.size() < 2)
                return 1;
            ipc.send("focusWindow", {{"client_name", positional_arguments[1]}});
        }
        else if (subcommand == "rename-client")
        {
            if (positional_arguments.size() < 2)
                return 1;
            ipc.send("renameClient",
                     {{"client_name", QProcessEnvironment::systemEnvironment().value("KAKQT_CLIENT_ID")},
                      {"new_client_name", positional_arguments[1]}});
        }
        else if (subcommand == "rename-session")
        {
            if (positional_arguments.size() < 2)
                return 1;
            ipc.send("renameSession", {{"session_name", positional_arguments[1]}});
        }
        else
        {
            return 1;
        }

        return 0;
    }
    else
    {
        QApplication app(argc, argv);

        QCommandLineParser parser;
        parser.setApplicationDescription("Kakoune Qt - A Qt-based frontend for Kakoune.");
        parser.addPositionalArgument("files", "open files in session");

        const QCommandLineOption setSessionIdOption("s", "Set the kakoune session id", "session_id");
        const QCommandLineOption connectSessionIdOption("c", "Connect to the given kakoune session", "session_id");
        const QCommandLineOption executeCommandClientOption(
            "e", "Execute command after the client initialization phase", "command");
        parser.addOption(cliOption);
        parser.addOption(executeCommandClientOption);
        parser.addOption(setSessionIdOption);
        parser.addOption(connectSessionIdOption);

        const QCommandLineOption helpOption = parser.addHelpOption();
        const QCommandLineOption versionOption = parser.addVersionOption();

        parser.process(app);

        if (parser.isSet(helpOption))
        {
            parser.showHelp();
            return 0;
        }
        if (parser.isSet(versionOption))
        {
            parser.showVersion();
            return 0;
        }

        KakouneSession *session;
        QStringList session_arguments = parser.positionalArguments();
        QString client_arguments =
            parser.isSet(executeCommandClientOption) ? parser.value(executeCommandClientOption) : "";

        if (parser.isSet(setSessionIdOption) && parser.isSet(connectSessionIdOption))
        {
            QTextStream(stdout) << "-s is incompatible with -c" << Qt::endl;
            return 1;
        }
        else if (parser.isSet(connectSessionIdOption))
        {
            session = new RemoteKakouneSession(parser.value(connectSessionIdOption));
        }
        else if (parser.isSet(setSessionIdOption))
        {
            session = new KakouneSession(parser.value(setSessionIdOption), session_arguments);
        }
        else
        {
            session = new KakouneSession(KakouneSession::generateRandomSessionId(), session_arguments);
        }

        MainWindow main_window(session, client_arguments);
        KakouneIPC::IPCServer server(main_window.getID().toString());
        main_window.bind(&server);

        main_window.show();

        return app.exec();
    }
}
