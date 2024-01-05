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
    else if (method == "menu_show")
    {
        RPC::MenuShowRequest request = RPC::deserializeMenuShowRequest(request_params);
        m_menu_items = request.items;
        m_menu_anchor = request.anchor;
        m_menu_selected_item_face = request.selected_item_face;
        m_menu_face = request.menu_face;
        m_menu_style = request.style;
        emit showMenu();
    }
    else if (method == "menu_hide")
    {
        emit hideMenu();
    }
    else if (method == "menu_select")
    {
        RPC::MenuSelectRequest request = RPC::deserializeMenuSelectRequest(request_params);
        emit selectMenuItem(request.selected);
    }
    else if (method == "info_show")
    {
        RPC::InfoShowRequest request = RPC::deserializeInfoShowRequest(request_params);
        m_info_title = request.title;
        m_info_content = request.content;
        m_info_anchor = request.anchor;
        m_info_face = request.face;
        m_info_style = request.style;
        emit showInfoBox();
    }
    else if (method == "info_hide")
    {
      emit hideInfoBox();
    }
    else
    {
        qDebug() << "Unkown method: " << method;
    }
}

KakouneClient::KakouneClient(const QString &session_id)
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

QList<RPC::Line> KakouneClient::getMenuItems()
{
    return m_menu_items;
}

RPC::Coord KakouneClient::getMenuAnchor()
{
    return m_menu_anchor;
}

RPC::Face KakouneClient::getSelectedMenuItemFace()
{
    return m_menu_selected_item_face;
}

RPC::Face KakouneClient::getMenuFace()
{
    return m_menu_face;
}

RPC::MenuStyle KakouneClient::getMenuStyle()
{
    return m_menu_style;
}

RPC::Line KakouneClient::getInfoTitle() {
  return m_info_title;
}

QList<RPC::Line> KakouneClient::getInfoContent() {
  return m_info_content;
}

RPC::Coord KakouneClient::getInfoAnchor() {
  return m_info_anchor;
}

RPC::Face KakouneClient::getInfoFace() {
  return m_info_face;
}

RPC::InfoStyle KakouneClient::getInfoStyle()
{
    return m_info_style;
}
