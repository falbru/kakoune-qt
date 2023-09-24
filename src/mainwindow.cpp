#include "mainwindow.h"
#include "kakounewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	resize(1024, 768);

	KakouneWidget* kakwidget = new KakouneWidget();
	setCentralWidget(kakwidget);
	kakwidget->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() { }
