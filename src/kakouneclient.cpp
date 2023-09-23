#include "kakouneclient.h"

void handleRequest(QByteArray request) {
    QJsonObject requestObject = QJsonDocument::fromJson(request).object();

    QString method = requestObject["method"].toString();

    // TODO
}

KakouneClient::KakouneClient() {
    connect(&m_process, &QProcess::readyReadStandardOutput, [=] () {
        QByteArray buffer = m_process.readAllStandardOutput();

        QList<QByteArray> requests = buffer.split('\n');
        for (QByteArray request : requests) {
            if (request == "") continue;
            handleRequest(request);
        }
    });

    connect(&m_process, &QProcess::readyReadStandardError, [=] () {
        qFatal() << "KakouneClient: " << m_process.readAllStandardError();
    });

    m_process.start("kak", {"-ui", "json"});
}

KakouneClient::~KakouneClient() {
    m_process.close();
}
