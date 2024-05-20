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

void createFifo(const QString &path)
{
    if (mkfifo(path.toStdString().c_str(), 0666) == -1)
    {
        qCritical("Failed to create fifo");
    }
}

void waitForFifo(const QString &path)
{
    int fd = open(path.toStdString().c_str(), O_RDONLY);
    if (fd == -1)
    {
        qCritical("Failed to read from fifo");
    }

    char buffer[16];
    while (read(fd, buffer, sizeof(buffer)) > 0)
    {
    }

    close(fd);
}

void deleteFifo(const QString &path)
{
    if (unlink(path.toStdString().c_str()) == -1)
    {
        qCritical("Failed to remove fifo");
    }
}

KakouneSession::KakouneSession(QString session_id)
{
    m_session_id = session_id;

    connect(&m_process, &QProcess::readyReadStandardError,
            [=]() { qCritical() << "KakouneSession: " << m_process.readAllStandardError(); });

    QString session_ready_path = "/tmp/" + session_id;

    createFifo(session_ready_path);

    m_process.start("kak", {"-s", session_id, "-d", "-E", QString("\"nop %sh{ echo > %1 }\"").arg(session_ready_path)});

    waitForFifo(session_ready_path);
    deleteFifo(session_ready_path);
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

void KakouneSession::setSessionId(const QString &session_id)
{
    m_session_id = session_id;
}
