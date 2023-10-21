#include "mainwindow.hpp"
#include "kakounewidget.hpp"
#include "statusbar.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(1024, 768);

    m_draw_options = new DrawOptions();
    m_draw_options->setFont("monospace", 11);

    m_session = new KakouneSession();

    KakouneWidget *kakwidget = new KakouneWidget(m_session->getSessionId(), m_draw_options);
    kakwidget->setFocusPolicy(Qt::StrongFocus);

    connect(kakwidget, &KakouneWidget::finished, this, &MainWindow::close);

    StatusBar *status_bar = new StatusBar(m_draw_options);
    connect(kakwidget, &KakouneWidget::refresh, this, [=]() { status_bar->repaint(); });
    status_bar->setActiveClient(kakwidget->getClient());

    QWidget *root = new QWidget(parent);
    QVBoxLayout *rootLayout = new QVBoxLayout();
    rootLayout->setSpacing(0);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addWidget(kakwidget);
    rootLayout->addWidget(status_bar);
    root->setLayout(rootLayout);
    setCentralWidget(root);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    delete m_session;
}
