#include "kakounesession.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

QString generateRandomSessionId()
{
    return QString::number(QRandomGenerator::global()->bounded(1000, 9999));
}

KakouneSession::KakouneSession() : KakouneSession(generateRandomSessionId())
{
}

KakouneSession::KakouneSession(QString session_id)
{
    m_session_id = session_id;

    connect(&m_process, &QProcess::readyReadStandardError,
            [=]() { qFatal() << "KakouneSession: " << m_process.readAllStandardError(); });

    std::string session_ready_path = ("/tmp/" + session_id).toStdString();
    const char *session_ready_path_cstr = session_ready_path.c_str();

    if (mkfifo(session_ready_path_cstr, 0666) == -1)
    {
        qFatal("Failed to create fifo");
    }

    m_process.start("kak",
                    {"-s", session_id, "-d", "-E", QString("\"nop %sh{ echo > %1 }\"").arg(session_ready_path_cstr)});

    int fd = open(session_ready_path_cstr, O_RDONLY);
    if (fd == -1)
    {
        qFatal("Failed to read from fifo");
    }

    char buffer[16];
    while (read(fd, buffer, sizeof(buffer)) > 0)
    {
    }

    close(fd);

    if (unlink(session_ready_path_cstr) == -1)
    {
        qFatal("Failed to remove fifo");
    }
}

KakouneSession::~KakouneSession()
{
    QProcess kill_session;
    kill_session.start("kak", {"-p", m_session_id});
    kill_session.waitForStarted();
    kill_session.write("kill");
    kill_session.closeWriteChannel();
    kill_session.waitForFinished();
    kill_session.close();

    m_process.close();
}

QString KakouneSession::getSessionId()
{
    return m_session_id;
}
