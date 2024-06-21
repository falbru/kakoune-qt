#include "ipc.hpp"
#include <qjsondocument.h>
#include <qnamespace.h>

namespace KakouneIPC
{
const QString getSocketName(const QString &session_id)
{
    return "KakouneQt." + session_id;
}

IPCClient::IPCClient()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString session_id = env.value("KAKQT_SESSION_ID");

    m_socket = new QLocalSocket();
    m_socket->connectToServer(getSocketName(session_id));
    if (!m_socket->waitForConnected())
    {
        qDebug() << "Failed to connect to server:" << m_socket->errorString();
        return;
    }
}

IPCClient::~IPCClient()
{
    m_socket->disconnectFromServer();
}

void IPCClient::send(QString method, QJsonObject parameters)
{
    parameters["method"] = method;
    m_socket->write(QJsonDocument(parameters).toJson(QJsonDocument::Compact));
    m_socket->flush();
}

IPCServer::IPCServer(const QString &session_id)
{
    m_server = new QLocalServer(this);

    if (!m_server->listen(getSocketName(session_id)))
    {
        qDebug() << "Unable to start the server: " << m_server->errorString();
        m_server->close();
        return;
    }

    connect(m_server, &QLocalServer::newConnection, this, &IPCServer::handleConnection);
}

IPCServer::~IPCServer()
{
    m_server->close();
    QLocalServer::removeServer(m_server->serverName());
    delete m_server;
}

void IPCServer::bind(MainWindow *main_window)
{
    connect(this, &IPCServer::newSplit, main_window, &MainWindow::newSplit);
    connect(this, &IPCServer::focusWindow, main_window, &MainWindow::focusWindow);
    connect(this, &IPCServer::renameSession, main_window, &MainWindow::renameSession);
}

void IPCServer::handleConnection()
{
    QLocalSocket *client_socket = m_server->nextPendingConnection();

    connect(client_socket, &QLocalSocket::disconnected, client_socket, &QLocalSocket::deleteLater);

    if (client_socket->waitForReadyRead())
    {
        QByteArray request = client_socket->readAll();
        QJsonObject request_json = QJsonDocument::fromJson(request).object();

        handleCommand(request_json);
    }

    client_socket->disconnectFromServer();
}

void IPCServer::handleCommand(QJsonObject request)
{
    const QString method = request["method"].toString();
    if (method == "newSplit")
    {
        emit newSplit(request["client_name"].toString(), request["args"].isString() ? request["args"].toString() : "",
                      request["orientation"].toString() == "vertical" ? Qt::Vertical : Qt::Horizontal);
    }
    else if (method == "focusWindow")
    {
        emit focusWindow(request["client_name"].toString());
    }
    else if (method == "renameSession")
    {
        emit renameSession(request["session_name"].toString());
    }
}

} // namespace KakouneIPC
