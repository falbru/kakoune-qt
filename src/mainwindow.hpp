#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "drawoptions.hpp"
#include "kakounesession.hpp"
#include "kakounewidget.hpp"
#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  public slots:
    void newClient();
    void newClient(const QString &arguments);
    void focusWindow(const QString &uuid);

  protected:
    void closeEvent(QCloseEvent *ev) override;

  private:
    QSplitter *m_root;
    KakouneSession *m_session;

    QList<KakouneWidget *> m_windows;

    DrawOptions *m_draw_options;
};

#endif
