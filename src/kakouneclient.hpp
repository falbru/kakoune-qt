#ifndef KAKOUNECLIENT_HPP
#define KAKOUNECLIENT_HPP

#include "rpc/face.hpp"
#include "rpc/line.hpp"
#include "rpc/rpc.hpp"
#include <QtWidgets>

class KakouneClient : public QObject
{
    Q_OBJECT
  public:
    KakouneClient(const QString &session_id);
    ~KakouneClient();

    void sendKeys(const QString &key);
    void sendMouseMove(int line, int column);
    void sendMousePress(const QString &button, int line, int column);
    void sendMouseRelease(const QString &button, int line, int column);
    void resize(int rows, int columns);

    QList<RPC::Line> getLines();
    RPC::Face getDefaultFace();
    RPC::Face getPaddingFace();
    RPC::Line getStatusLine();
    RPC::Line getModeLine();
    RPC::Face getStatusDefaultFace();
    QList<RPC::Line> getMenuItems();
    RPC::Coord getMenuAnchor();
    RPC::Face getSelectedItemFace();
    RPC::Face getMenuFace();
  signals:
    void refresh();
    void finished();

    void showMenu();
    void hideMenu();
    void selectMenuItem(int selected);

  private:
    void sendRequest(const QString &method_name, QJsonArray params);
    void handleRequest(QJsonObject request);

    QProcess m_process;

    QList<RPC::Line> m_lines;
    RPC::Face m_default_face;
    RPC::Face m_padding_face;

    RPC::Line m_status_line;
    RPC::Line m_mode_line;
    RPC::Face m_status_default_face;

    QList<RPC::Line> m_menu_items;
    RPC::Coord m_menu_anchor;
    RPC::Face m_menu_selected_items_face;
    RPC::Face m_menu_face;
    RPC::MenuStyle m_menu_style;
};

#endif
