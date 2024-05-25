#ifndef KAKOUNEIPC_HPP_INCLUDED
#define KAKOUNEIPC_HPP_INCLUDED

#include "mainwindow.hpp"
#include <qlocalserver.h>
#include <qlocalsocket.h>

namespace KakouneIPC
{
    const QString getSocketName(const QString& session_id);

    class IPCClient
    {
      public:
        IPCClient();
        ~IPCClient();

        void send(QString method, QJsonObject parameters);

      private:
        QLocalSocket *m_socket;

    };

    class IPCServer : public QObject
    {
        Q_OBJECT
      public:
        IPCServer(const QString &session_id);
        ~IPCServer();

        void bind(MainWindow *main_window);
      signals:
        void newClient(const QString &arguments);
        void focusWindow(const QString &client_name);
        void renameSession(const QString &session_name);

      protected:
        void handleConnection();
        void handleCommand(QJsonObject request);

      private:
        QLocalServer *m_server;
    };
}

#endif
