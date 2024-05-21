#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "drawoptions.hpp"
#include "kakounesession.hpp"
#include "kakounewidget.hpp"
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

    QUuid getID();

  public slots:
    void newClient(const QString &arguments);
    void focusWindow(const QString &uuid);
    void renameSession(const QString &session_name);

  protected:
    void closeEvent(QCloseEvent *ev) override;
    void updateWindowTitle();

  private:
    QUuid m_id;
    QSplitter *m_root;
    KakouneSession *m_session;

    QList<KakouneWidget *> m_windows;

    DrawOptions *m_draw_options;
};

#endif
