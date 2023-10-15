#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

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
    KakouneSession* m_session;
};

#endif
