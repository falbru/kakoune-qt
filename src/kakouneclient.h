#ifndef KAKOUNECLIENT_H
#define KAKOUNECLIENT_H

#include <QtWidgets>

class KakouneClient : public QObject {
    Q_OBJECT
public:
    KakouneClient();
    ~KakouneClient();
private:
    QProcess m_process;
};

#endif
