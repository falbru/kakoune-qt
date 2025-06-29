#ifndef KAKOUNEIPC_HPP_INCLUDED
#define KAKOUNEIPC_HPP_INCLUDED

#include <qjsonobject.h>
#include <qlocalserver.h>
#include <qlocalsocket.h>
#include <qtmetamacros.h>

namespace KakouneIPC
{
const QString getSocketName(const QString &session_id);

class IPCClient : public QObject
{
    Q_OBJECT
  public:
    IPCClient();
    ~IPCClient();

    void send(QString method, QJsonObject parameters);
    QString readResponse();

  private:
    QLocalSocket *m_socket;
};

class IPCServer : public QObject
{
    Q_OBJECT
  public:
    IPCServer(const QString &session_id);
    ~IPCServer();

  signals:
    void newSplit(const QString &client_name, const QString &arguments, const Qt::Orientation &orientation);
    void setWindowVisible(const QString &client_name, bool visible);
    bool getWindowVisible(const QString &client_name);
    void focusWindow(const QString &client_name);
    void renameClient(const QString &client_name, const QString &new_client_name);
    void renameSession(const QString &session_name);
    void setTabs(const QList<QString> &tabs);
    void setSelectedTab(const QString &selected);

  protected:
    void handleConnection();
    QString handleCommand(QJsonObject request);

  private:
    QLocalServer *m_server;
};
} // namespace KakouneIPC

#endif
