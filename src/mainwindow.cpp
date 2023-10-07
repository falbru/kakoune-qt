#include "mainwindow.hpp"
#include "kakounewidget.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(1024, 768);

    KakouneWidget *kakwidget = new KakouneWidget();

    connect(kakwidget, &KakouneWidget::finished, this, &MainWindow::close);

    setCentralWidget(kakwidget);
    kakwidget->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
}
