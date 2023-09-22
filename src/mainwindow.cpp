#include "mainwindow.h"
#include "kakounewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	resize(1024, 768);

	KakouneWidget* kakwindow = new KakouneWidget();
	setCentralWidget(kakwindow);
}

MainWindow::~MainWindow()
{

}
