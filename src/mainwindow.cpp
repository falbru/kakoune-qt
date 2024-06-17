#include "mainwindow.hpp"
#include "container.hpp"
#include "kakounewidget.hpp"
#include "keybindings.hpp"
#include <climits>
#include <qnamespace.h>
#include <quuid.h>
#include <qwidget.h>

MainWindow::MainWindow(QString session_id, QWidget *parent) : QMainWindow(parent)
{
    m_id = QUuid::createUuid();
    resize(1024, 768);

    m_draw_options = new DrawOptions();
    m_draw_options->setFont("monospace", 11);

    m_session = new KakouneSession(session_id);

    m_root = new SplitContainer(Qt::Horizontal, this);
    setCentralWidget(m_root);

    KakouneWidget *kak_widget = createKakouneWidget();
    m_root->addWidget(kak_widget);

    updateWindowTitle();
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    delete m_session;
}

QUuid MainWindow::getID()
{
    return m_id;
}

void MainWindow::newSplit(const QString &client_name, const QString &arguments, const Qt::Orientation &orientation)
{
    QWidget *source_widget = findKakouneWidget(client_name);
    if (source_widget == nullptr)
    {
        qWarning() << "MainWindow::newSplit: Could not find KakouneWidget with client name: " << client_name;
        return;
    }

    if (SplitContainer *parent_split = (SplitContainer *)Container::findParentContainer(source_widget))
    {
        KakouneWidget *new_kak_widget = createKakouneWidget(arguments);
        parent_split->split(source_widget, new_kak_widget, orientation);
        new_kak_widget->setFocus();
    }
    else
    {
        qWarning("MainWindow::newSplit: Couldn't find parent container");
    }
}

void MainWindow::focusWindow(const QString &client_name)
{
    KakouneWidget *kak_widget = findKakouneWidget(client_name);

    if (kak_widget == nullptr)
    {
        qWarning() << "MainWindow::focusWindow: Could not find KakouneWidget with client name: " << client_name;
        return;
    }

    kak_widget->setFocus();
}

void MainWindow::focusLeft()
{
    // TODO
}

void MainWindow::focusRight()
{
    // TODO
    // Container: have a lastFocusedWidget() method
    // Iterate up to closest container that is Horizontal. Focus lastFocusedWidget
    // When a Container gains focus, focus the last focused widget
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(m_session->getSessionId() + " - Kakoune Qt");
}

void MainWindow::renameSession(const QString &session_name)
{
    m_session->setSessionId(session_name);
    updateWindowTitle();
}

KakouneWidget *MainWindow::findKakouneWidget(const QString &client_name)
{
    for (KakouneWidget *window : m_windows)
    {
        if (window->getID().toString() == client_name)
        {
            return window;
        }
    }
    return nullptr;
}

KakouneWidget *MainWindow::createKakouneWidget(const QString &arguments)
{
    KakouneWidget *kakwidget = new KakouneWidget(m_session->getSessionId(), m_id, m_draw_options, arguments, m_root);

    kakwidget->installEventFilter(new KeyBindingsFilter(this));

    connect(kakwidget, &KakouneWidget::finished, m_root, [=]() {
        m_windows.removeOne(kakwidget);

        if (m_windows.size() == 0)
        {
            close();
            return;
        }
    });

    m_windows.append(kakwidget);

    return kakwidget;
}
