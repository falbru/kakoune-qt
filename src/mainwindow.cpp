#include "mainwindow.hpp"
#include "keybindings.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(1024, 768);

    m_draw_options = new DrawOptions();
    m_draw_options->setFont("monospace", 11);

    m_session = new KakouneSession();

    m_root = new QSplitter(this);
    this->newClient();

    setCentralWidget(m_root);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    delete m_session;
}

void MainWindow::newClient()
{
    newClient("");
}

void MainWindow::newClient(const QString &arguments)
{
    KakouneWidget *kakwidget = new KakouneWidget(m_session->getSessionId(), m_draw_options, arguments, m_root);
    kakwidget->installEventFilter(new KeyBindingsFilter(this));
    connect(kakwidget, &KakouneWidget::finished, m_root, [=]() {
        kakwidget->setParent(nullptr);
        m_windows.removeOne(kakwidget);

        if (m_root->count() == 0)
        {
            close();
        }
    });

    m_windows.append(kakwidget);
    m_root->addWidget(kakwidget);
}

void MainWindow::focusWindow(const QString &uuid)
{
    for (KakouneWidget *window : m_windows)
    {
        if (window->getID().toString() == uuid)
        {
            window->setFocus();
            return;
        }
    }
}

void MainWindow::focusLeft()
{
    QWidget *focused_widget = qApp->focusWidget();
    int index = m_root->indexOf((QWidget *)focused_widget->parent()); // TODO
    if (index <= 0)
    {
        return;
    }

    m_windows[index - 1]->setFocus();
}

void MainWindow::focusRight()
{
    QWidget *focused_widget = qApp->focusWidget();
    int index = m_root->indexOf((QWidget *)focused_widget->parent()); // TODO
    if (index >= m_windows.size() - 1)
    {
        return;
    }

    m_windows[index + 1]->setFocus();
}
