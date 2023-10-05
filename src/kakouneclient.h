#ifndef KAKOUNECLIENT_H
#define KAKOUNECLIENT_H

#include <QtWidgets>

class KakouneClient : public QObject {
    Q_OBJECT
public:
    KakouneClient();
    ~KakouneClient();

    void sendKeys(const QString& key);
    void sendMouseMove(int line, int column);
    void sendMousePress(const QString& button, int line, int column);
    void sendMouseRelease(const QString& button, int line, int column);
    void resize(int rows, int columns);
signals:
    void handleRequest(QJsonObject request);
    void finished();
private:
    QProcess m_process;
};

#endif
