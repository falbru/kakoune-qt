#include "mainwindow.hpp"
#include "kakounewidgetwithstatusbar.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(1024, 768);

    m_draw_options = new DrawOptions();
    m_draw_options->setFont("monospace", 11);

    m_session = new KakouneSession();

    QSplitter *root = new QSplitter(parent);

    for (int i = 0; i < 2; i++) {
      KakouneWidgetWithStatusBar* kakwidget = new KakouneWidgetWithStatusBar(m_session->getSessionId(), m_draw_options, root);
      connect(kakwidget, &KakouneWidgetWithStatusBar::finished, root, [=]() {
        kakwidget->setParent(nullptr);
      });

      root->addWidget(kakwidget);
    }

    // TODO: MainWindow::close when there are no more widgets in root
    // TODO: Create the KakouneCode class, which is basically what KakouneWidget is now minus the handling of KakouneClient. Move that into its own class, KakouneWidget, which includes KakouneCode and StatusBar.

    setCentralWidget(root);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    delete m_session;
}
