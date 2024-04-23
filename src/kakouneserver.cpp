#include "kakouneserver.hpp"
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>

KakouneServer::KakouneServer(const QString& session_id)
{
    m_server = new QLocalServer(this);

    QString server_name = "KakouneQt.543";
    if (!m_server->listen(server_name))
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
}

void KakouneServer::handleConnection()
{
    QLocalSocket *client_socket = m_server->nextPendingConnection();

    connect(client_socket, &QLocalSocket::disconnected,
            client_socket, &QLocalSocket::deleteLater);

    if (client_socket->waitForReadyRead()) {
        QByteArray request = client_socket->readAll();
        QJsonObject request_json = QJsonDocument::fromJson(request).object();
        qDebug() << request_json;

        handleCommand(request_json);
    }

    client_socket->disconnectFromServer();
}

void KakouneServer::handleCommand(QJsonObject request)
{
    const QString method = request["method"].toString();
    if (method == "newClient") {
      emit newClient("");
    }
}
