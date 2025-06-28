#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "drawoptions.hpp"
#include "ipc.hpp"
#include "kakounesession.hpp"
#include "kakounetabbar.hpp"
#include "kakounewidget.hpp"
#include "lastfocusedfilter.hpp"
#include "splitcontainer.hpp"
#include <QMainWindow>
#include <QtWidgets>
#include <qtabbar.h>
#include <quuid.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(KakouneSession *session, QWidget *parent = nullptr);
    MainWindow(KakouneSession *session, QString client_arguments, QWidget *parent = nullptr);
    ~MainWindow();

    void focusLeft();
    void focusRight();
    void focusUp();
    void focusDown();
    void focusInDirection(std::function<bool(const QRect &, const QRect &)> isWithinBounds);

    void bind(KakouneIPC::IPCServer *server);

    QUuid getID();

  public slots:
    void newSplit(const QString &client_name, const QString &arguments, const Qt::Orientation &orientation);
    void setWindowVisible(const QString &client_name, bool visible);
    bool getWindowVisible(const QString &client_name);
    void focusWindow(const QString &client_name);
    void renameSession(const QString &session_name);
    void renameClient(const QString &client_name, const QString &new_client_name);
    void switchBufferInFocusedWidget(const QString& bufname);

  protected:
    void closeEvent(QCloseEvent *ev) override;
    void updateWindowTitle();
    void installLastFocusedFilter(QWidget *widget);

  private:
    QUuid m_id;
    KakouneSession *m_session;

    KakouneWidget *createKakouneWidget(const QString &arguments = "");
    KakouneWidget *findKakouneWidget(const QString &client_name);
    KakouneWidget *findFocusedKakouneWidget();
    void ensureOneVisibleKakouneWidget();
    void focusLastFocusedVisibleKakouneWidget();

    SplitContainer *m_root;
    KakouneTabBar* m_tab_bar;
    QList<KakouneWidget *> m_windows;
    DrawOptions *m_draw_options;

    LastFocusedFilter *m_last_focused_filter;
};

#endif
