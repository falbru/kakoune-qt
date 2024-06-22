#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "drawoptions.hpp"
#include "kakounesession.hpp"
#include "kakounewidget.hpp"
#include "lastfocusedfilter.hpp"
#include "splitcontainer.hpp"
#include <QMainWindow>
#include <QtWidgets>
#include <quuid.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QString session_id, QWidget *parent = nullptr);
    ~MainWindow();

    void focusLeft();
    void focusRight();
    void focusUp();
    void focusDown();
    void focusInDirection(std::function<bool(const QRect &, const QRect &)> isWithinBounds);

    QUuid getID();

  public slots:
    void newSplit(const QString &client_name, const QString &arguments, const Qt::Orientation &orientation);
    void focusWindow(const QString &client_name);
    void renameSession(const QString &session_name);

  protected:
    void closeEvent(QCloseEvent *ev) override;
    void updateWindowTitle();
    void installLastFocusedFilter(QWidget *widget);

  private:
    QUuid m_id;
    KakouneSession *m_session;

    KakouneWidget *createKakouneWidget(const QString &arguments = "");
    KakouneWidget *findKakouneWidget(const QString &client_name);

    SplitContainer *m_root;
    QList<KakouneWidget *> m_windows;
    DrawOptions *m_draw_options;

    LastFocusedFilter *m_last_focused_filter;
};

#endif
