#include "kakounecli.hpp"

KakouneCli::KakouneCli(const QString &service_name) : m_dbusiface(service_name, "/")
{
}

int KakouneCli::run(QStringList command)
{
    QString &command_name = command[0];
    if (command_name == "new-client")
    {
        if (command.size() == 1)
        {
            m_dbusiface.call("newClient");
        }
        else
        {
            m_dbusiface.call("newClient", command.sliced(1).join(" "));
        }
    }
    else if (command_name == "focus")
    {
        if (command.size() != 2)
        {
            return 1;
        }
        m_dbusiface.call("focusWindow", command[1]);
    }
    else
    {
        return 1;
    }

    return 0;
}