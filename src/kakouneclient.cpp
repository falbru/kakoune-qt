#include "kakouneclient.hpp"

#include "rpc/rpc.hpp"

void KakouneClient::handleRequest(QJsonObject request)
{
    const QString method = request["method"].toString();
    QJsonArray request_params = request["params"].toArray();

    if (method == "draw")
    {
        RPC::DrawRequest request = RPC::deserializeDrawRequest(request_params);
        m_lines = request.lines;
        m_default_face = request.default_face;
        m_padding_face = request.padding_face;
    }
    else if (method == "draw_status")
    {
        RPC::DrawStatusRequest request = RPC::deserializeDrawStatusRequest(request_params);
        m_status_line = request.status_line;
        m_mode_line = request.mode_line;
        m_status_default_face = request.default_face;
    }
    else if (method == "refresh")
    {
        RPC::RefreshRequest request = RPC::deserializeRefreshRequest(request_params);
        emit refresh();
    }
    else
    {
        qDebug() << "Unkown method: " << method;
    }
}

KakouneClient::KakouneClient(const QString& session_id)
{
    connect(&m_process, &QProcess::readyReadStandardOutput, [=]() {
        QByteArray buffer = m_process.readAllStandardOutput();

        QList<QByteArray> requests = buffer.split('\n');
        for (QByteArray request : requests)
        {
            if (request == "")
                continue;
            handleRequest(QJsonDocument::fromJson(request).object());
        }
    });

    connect(&m_process, &QProcess::readyReadStandardError,
            [=]() { qFatal() << "KakouneClient: " << m_process.readAllStandardError(); });

    connect(&m_process, &QProcess::finished, this, &KakouneClient::finished);

    m_process.start("kak", {"-ui", "json", "-c", session_id});
}

KakouneClient::~KakouneClient()
{
    m_process.close();
}

void KakouneClient::sendRequest(const QString &method_name, QJsonArray params)
{
    m_process.write(RPC::serializeRequest(method_name, params));
}

void KakouneClient::sendKeys(const QString &key)
{
    sendRequest("keys", QJsonArray{key});
}

void KakouneClient::sendMouseMove(int line, int column)
{
    sendRequest("mouse_move", QJsonArray{line, column});
}

void KakouneClient::sendMousePress(const QString &button, int line, int column)
{
    sendRequest("mouse_press", QJsonArray{button, line, column});
}

void KakouneClient::sendMouseRelease(const QString &button, int line, int column)
{
    sendRequest("mouse_release", QJsonArray{button, line, column});
}

void KakouneClient::resize(int rows, int columns)
{
    sendRequest("resize", QJsonArray{rows, columns});
}

QList<RPC::Line> KakouneClient::getLines()
{
    return m_lines;
}

RPC::Face KakouneClient::getDefaultFace()
{
    return m_default_face;
}

RPC::Face KakouneClient::getPaddingFace()
{
    return m_padding_face;
}

RPC::Line KakouneClient::getStatusLine()
{
    return m_status_line;
}

RPC::Line KakouneClient::getModeLine()
{
    return m_mode_line;
}

RPC::Face KakouneClient::getStatusDefaultFace()
{
    return m_status_default_face;
}
