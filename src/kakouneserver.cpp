#include "kakouneserver.hpp"
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>

KakouneServer::KakouneServer(const QString& session_id)
{
    m_server = new QLocalServer(this);

    if (!m_server->listen(session_id))
    {
      qDebug() << "Unable to start the server: " << m_server->errorString();
      m_server->close();
      return;
    }

    connect(m_server, &QLocalServer::newConnection, this, &KakouneServer::handleConnection);
}

KakouneServer::~KakouneServer()
{
    m_server->close();
    delete m_server;
}

void KakouneServer::bind(MainWindow* main_window)
{
    connect(this, &KakouneServer::newClient, main_window, &MainWindow::newClient);
    connect(this, &KakouneServer::focusWindow , main_window, &MainWindow::focusWindow);
}

void KakouneServer::handleConnection()
{
    QLocalSocket *client_socket = m_server->nextPendingConnection();

    connect(client_socket, &QLocalSocket::disconnected,
            client_socket, &QLocalSocket::deleteLater);

    if (client_socket->waitForReadyRead()) {
        QByteArray request = client_socket->readAll();
        QJsonObject request_json = QJsonDocument::fromJson(request).object();

        handleCommand(request_json);
    }

    client_socket->disconnectFromServer();
}

void KakouneServer::handleCommand(QJsonObject request)
{
    const QString method = request["method"].toString();
    qDebug() << method;
    if (method == "newClient") {
      emit newClient(request["args"].isString() ? request["args"].toString() : "");
    }else if (method == "focusWindow") {
      qDebug() << request["client_name"].toString();
      emit focusWindow(request["client_name"].toString());
    }
}
