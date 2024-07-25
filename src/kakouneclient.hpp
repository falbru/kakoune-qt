#ifndef KAKOUNECLIENT_HPP
#define KAKOUNECLIENT_HPP

#include "rpc/face.hpp"
#include "rpc/line.hpp"
#include "rpc/rpc.hpp"
#include <QtWidgets>
#include <qglobal.h>

class KakouneClient : public QObject
{
    Q_OBJECT
  public:
    KakouneClient(const QString &session_id);
    KakouneClient(const QString &session_id, QString arguments);
    KakouneClient(const QString &session_id, QString arguments, QList<QPair<QString, QString>> environment_variables);
    ~KakouneClient();

    void sendKeys(const QString &key);
    void sendMouseMove(int line, int column);
    void sendMousePress(const QString &button, int line, int column);
    void sendMouseRelease(const QString &button, int line, int column);
    void resize(int rows, int columns);

    const QString &getClientName() const;
    void setClientName(QString client_name);

    QList<RPC::Line> getLines();
    RPC::Face getDefaultFace();
    RPC::Face getPaddingFace();
    RPC::Line getStatusLine();
    RPC::Line getModeLine();
    RPC::Face getStatusDefaultFace();
    QList<RPC::Line> getMenuItems();
    RPC::Coord getMenuAnchor();
    RPC::Face getSelectedMenuItemFace();
    RPC::Face getMenuFace();
    RPC::MenuStyle getMenuStyle();
    QList<RPC::Line> getInfoContent();
    RPC::Line getInfoTitle();
    RPC::Coord getInfoAnchor();
    RPC::Face getInfoFace();
    RPC::InfoStyle getInfoStyle();
  signals:
    void refresh();
    void finished();

    void showMenu();
    void hideMenu();
    void selectMenuItem(int selected);

    void showInfoBox();
    void hideInfoBox();

    void setUIOptions(QMap<QString, QString> options);

  private:
    void sendRequest(const QString &method_name, QJsonArray params);
    void handleRequest(QJsonObject request);

    QString m_client_name;
    QProcess m_process;

    QByteArray m_request_buffer;

    QList<RPC::Line> m_lines;
    RPC::Face m_default_face;
    RPC::Face m_padding_face;

    RPC::Line m_status_line;
    RPC::Line m_mode_line;
    RPC::Face m_status_default_face;

    QList<RPC::Line> m_menu_items;
    RPC::Coord m_menu_anchor;
    RPC::Face m_menu_selected_item_face;
    RPC::Face m_menu_face;
    RPC::MenuStyle m_menu_style;

    RPC::Line m_info_title;
    QList<RPC::Line> m_info_content;
    RPC::Coord m_info_anchor;
    RPC::Face m_info_face;
    RPC::InfoStyle m_info_style;
};

#endif
