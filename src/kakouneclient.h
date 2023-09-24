#ifndef KAKOUNECLIENT_H
#define KAKOUNECLIENT_H

#include <QtWidgets>

class KakouneClient : public QObject {
    Q_OBJECT
public:
    KakouneClient();
    ~KakouneClient();

    void sendKeys(const QString& key);
    void resize(int rows, int columns);
signals:
    void handleRequest(QJsonObject request);
private:
    QProcess m_process;
};

#endif
