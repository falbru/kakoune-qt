#include "kakounecli.hpp"
#include <qprocess.h>

KakouneCli::KakouneCli()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString session_id = env.value("KAKQT_SESSION_ID") ;

    m_socket = new QLocalSocket();
    m_socket->connectToServer("KakouneQt." + session_id);
    if (!m_socket->waitForConnected()) {
        qDebug() << "Failed to connect to server:" << m_socket->errorString();
        return;
    }
}

KakouneCli::~KakouneCli()
{
  m_socket->disconnectFromServer();
}

int KakouneCli::run(QStringList command)
{
    QString &command_name = command[0];
    if (command_name == "new-client")
    {
        if (command.size() == 1)
        {
            m_socket->write("{\"method\":\"newClient\"}");
            m_socket->flush();
        }
        else
        {
            // m_socket->write("newClient", command.sliced(1).join(" "));
            // m_socket->flush();
        }
    }
    else if (command_name == "focus")
    {
        if (command.size() != 2)
        {
            return 1;
        }
        // m_socket->write("focusWindow", command[1]);
        // m_socket->flush();
    }
    else
    {
        return 1;
    }

    return 0;
}
