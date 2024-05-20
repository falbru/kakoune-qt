#ifndef KAKOUNESERVER_HPP
#define KAKOUNESERVER_HPP

#include "mainwindow.hpp"
#include <QLocalServer>
#include <QObject>

class KakouneServer : public QObject
{
    Q_OBJECT
  public:
    KakouneServer(const QString &session_id);
    ~KakouneServer();

    void bind(MainWindow *main_window);
  signals:
    void newClient(const QString &arguments);
    void focusWindow(const QString &client_name);
    void renameSession(const QString &session_name);

  protected:
    void handleConnection();
    void handleCommand(QJsonObject request);

  private:
    QLocalServer *m_server;
};

#endif
