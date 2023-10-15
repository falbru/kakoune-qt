#include "mainwindow.hpp"
#include "kakounewidget.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(1024, 768);

    m_session = new KakouneSession();

    KakouneWidget *kakwidget = new KakouneWidget(m_session->getSessionId());

    connect(kakwidget, &KakouneWidget::finished, this, &MainWindow::close);

    setCentralWidget(kakwidget);
    kakwidget->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *ev) {
  delete m_session;
}
