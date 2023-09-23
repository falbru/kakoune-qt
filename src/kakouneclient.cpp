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

    m_process.start("kak", {"-ui", "json"});
}

KakouneClient::~KakouneClient() {
    m_process.close();
}
