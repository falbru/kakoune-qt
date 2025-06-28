#include "ipc.hpp"
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qnamespace.h>
#include <qprocess.h>

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

QString IPCClient::readResponse()
{
    if (!m_socket->waitForReadyRead())
        return "";

    QByteArray response_data = m_socket->readAll();
    return QString(response_data);
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

void IPCServer::handleConnection()
{
    QLocalSocket *client_socket = m_server->nextPendingConnection();

    connect(client_socket, &QLocalSocket::disconnected, client_socket, &QLocalSocket::deleteLater);

    if (client_socket->waitForReadyRead())
    {
        QByteArray request = client_socket->readAll();
        QJsonObject request_json = QJsonDocument::fromJson(request).object();

        QString response = handleCommand(request_json);
        if (!response.isEmpty())
        {
            QByteArray response_data = response.toUtf8();
            client_socket->write(response_data);
            client_socket->flush();
        }
    }

    client_socket->disconnectFromServer();
}

QString IPCServer::handleCommand(QJsonObject request)
{
    const QString method = request["method"].toString();
    qDebug() << "IPC:" << method;
    if (method == "newSplit")
    {
        emit newSplit(request["client_name"].toString(), request["args"].isString() ? request["args"].toString() : "",
                      request["orientation"].toString() == "vertical" ? Qt::Vertical : Qt::Horizontal);
    }
    else if (method == "setWindowVisible")
    {
        emit setWindowVisible(request["client_name"].toString(), request["visible"].toBool(true));
    }
    else if (method == "getWindowVisible")
    {
        bool visible = emit getWindowVisible(request["client_name"].toString());
        return visible ? "true" : "false";
    }
    else if (method == "setTabs")
    {
        QJsonArray tabsJsonArray = request["tabs"].toArray();
        QList<QString> tabs;

        for (const QJsonValue& value : tabsJsonArray) {
            tabs.append(value.toString());
        }

        emit setTabs(tabs);
    }
    else if (method == "setSelectedTab")
    {
        emit setSelectedTab(request["bufname"].toString());
    }
    else if (method == "focusWindow")
    {
        emit focusWindow(request["client_name"].toString());
    }
    else if (method == "renameClient")
    {
        emit renameClient(request["client_name"].toString(), request["new_client_name"].toString());
    }
    else if (method == "renameSession")
    {
        emit renameSession(request["session_name"].toString());
    }

    return "";
}

} // namespace KakouneIPC
