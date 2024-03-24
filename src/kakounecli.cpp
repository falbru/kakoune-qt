#include "kakounecli.hpp"

KakouneCli::KakouneCli(const QString &service_name) : m_dbusiface(service_name, "/")
{
}

int KakouneCli::run(QList<QString> command)
{
    QString &command_name = command[0];
    if (command_name == "new-client")
    {
        m_dbusiface.call("newClient");
    }
    else
    {
        return 1;
    }

    return 0;
}
