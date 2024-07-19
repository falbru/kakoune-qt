#ifndef KAKOUNESESSION_HPP
#define KAKOUNESESSION_HPP

#include <QtWidgets>

class KakouneSession : public QObject
{
    Q_OBJECT
  public:
    KakouneSession(QString session_id);
    KakouneSession(QString session_id, QStringList session_arguments);
    ~KakouneSession();

    QString getSessionId();
    void setSessionId(const QString &session_id);

    static QString generateRandomSessionId();

  protected:
    KakouneSession();
    QProcess* m_process;
    QString m_session_id;
};

#endif
