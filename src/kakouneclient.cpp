#include "kakouneclient.h"

KakouneClient::KakouneClient() {
    connect(&m_process, &QProcess::readyReadStandardOutput, [=] () {
        QByteArray buffer = m_process.readAllStandardOutput();

        QList<QByteArray> requests = buffer.split('\n');
        for (QByteArray request : requests) {
            if (request == "") continue;
            emit handleRequest(QJsonDocument::fromJson(request).object());
        }
    });

    connect(&m_process, &QProcess::readyReadStandardError, [=] () {
        qFatal() << "KakouneClient: " << m_process.readAllStandardError();
    });

    connect(&m_process, &QProcess::finished, this, &KakouneClient::finished);

    m_process.start("kak", {"-ui", "json"});
}

KakouneClient::~KakouneClient() {
    m_process.close();
}

void KakouneClient::sendKeys(const QString& key) {
    QJsonObject req{
        {{"jsonrpc", "2.0"}, {"method", "keys"}, {"params", QJsonArray{key}}}
    };

    QByteArray rpc = QJsonDocument(req).toJson(QJsonDocument::Compact);
    m_process.write(rpc);
}

void KakouneClient::sendMouseMove(int line, int column) {
    QJsonObject req{
        {{"jsonrpc", "2.0"}, {"method", "mouse_move"}, {"params", QJsonArray{line, column}}}
    };

    QByteArray rpc = QJsonDocument(req).toJson(QJsonDocument::Compact);
    m_process.write(rpc);
}

void KakouneClient::sendMousePress(const QString& button, int line, int column) {
    QJsonObject req{
        {{"jsonrpc", "2.0"}, {"method", "mouse_press"}, {"params", QJsonArray{button, line, column}}}
    };

    QByteArray rpc = QJsonDocument(req).toJson(QJsonDocument::Compact);
    m_process.write(rpc);
}

void KakouneClient::sendMouseRelease(const QString& button, int line, int column) {
    QJsonObject req{
        {{"jsonrpc", "2.0"}, {"method", "mouse_release"}, {"params", QJsonArray{button, line, column}}}
    };

    QByteArray rpc = QJsonDocument(req).toJson(QJsonDocument::Compact);
    m_process.write(rpc);
}

void KakouneClient::resize(int rows, int columns) {
    QJsonObject req{
        {{"jsonrpc", "2.0"}, {"method", "resize"}, {"params", QJsonArray{rows, columns}}}
    };

    QByteArray rpc = QJsonDocument(req).toJson(QJsonDocument::Compact);
    m_process.write(rpc);
}
