#include "mainwindow.hpp"
#include "kakounewidget.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(1024, 768);

    m_draw_options = new DrawOptions();
    m_draw_options->setFont("monospace", 11);

    m_session = new KakouneSession();

    QSplitter *root = new QSplitter(parent);

    for (int i = 0; i < 2; i++)
    {
        KakouneWidget *kakwidget = new KakouneWidget(m_session->getSessionId(), m_draw_options, root);
        connect(kakwidget, &KakouneWidget::finished, root, [=]() {
            kakwidget->setParent(nullptr);

            if (root->count() == 0)
            {
                close();
            }
        });

        root->addWidget(kakwidget);
    }

    setCentralWidget(root);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    delete m_session;
}
