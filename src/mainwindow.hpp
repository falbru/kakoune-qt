#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "drawoptions.hpp"
#include "kakounesession.hpp"
#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  protected:
    void closeEvent(QCloseEvent *ev) override;

  private:
    KakouneSession *m_session;

    DrawOptions* m_draw_options;
};

#endif
