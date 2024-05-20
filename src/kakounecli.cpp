#include "kakounecli.hpp"
#include <qprocess.h>

KakouneCli::KakouneCli()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString session_id = env.value("KAKQT_SESSION_ID");

    m_socket = new QLocalSocket();
    m_socket->connectToServer("KakouneQt." + session_id);
    if (!m_socket->waitForConnected())
    {
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
    qDebug() << command_name;
    if (command_name == "new-client")
    {
        if (command.size() == 1)
        {
            m_socket->write("{\"method\":\"newClient\"}");
            m_socket->flush();
        }
        else
        {
            QString request = QString("{\"method\":\"newClient\",\"args\":\"%1\"}").arg(command.mid(1).join(" "));
            m_socket->write(request.toLocal8Bit());
            m_socket->flush();
        }
    }
    else if (command_name == "focus")
    {
        if (command.size() != 2)
        {
            return 1;
        }
        QString request = QString("{\"method\":\"focusWindow\",\"client_name\":\"%1\"}").arg(command[1]);
        m_socket->write(request.toLocal8Bit());
        m_socket->flush();
    }
    else if (command_name == "rename-session")
    {
        if (command.size() != 2)
        {
            return 1;
        }
        QString request = QString("{\"method\":\"renameSession\",\"session_name\":\"%1\"}").arg(command[1]);
        m_socket->write(request.toLocal8Bit());
        m_socket->flush();
    }
    else
    {
        return 1;
    }

    return 0;
}
