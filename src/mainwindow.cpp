#include "mainwindow.hpp"
#include "container.hpp"
#include "kakounewidget.hpp"
#include "keybindings.hpp"
#include "lastfocusedfilter.hpp"
#include <climits>
#include <qaccessible_base.h>
#include <qkeysequence.h>
#include <qnamespace.h>
#include <qpainter.h>
#include <quuid.h>
#include <qwidget.h>

MainWindow::MainWindow(QString session_id, QWidget *parent) : QMainWindow(parent)
{
    m_id = QUuid::createUuid();
    resize(1024, 768);

    m_last_focused_filter = new LastFocusedFilter(this);

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

void MainWindow::setWindowVisible(const QString &client_name, bool visible)
{
    KakouneWidget *kak_widget = findKakouneWidget(client_name);

    if (kak_widget == nullptr)
    {
        qWarning() << "MainWindow::setWindowVisible: Could not find KakouneWidget with client name: " << client_name;
        return;
    }


    if (!visible && focusWidget() == kak_widget) {
        QList<KakouneWidget*> find_candidates = QList<KakouneWidget*>();
        for (KakouneWidget* candidate : m_windows) {
            if (candidate == kak_widget)
                continue;

            find_candidates.append(candidate);
        }

        QWidget* last_focused = m_last_focused_filter->findLastFocusedWidget(find_candidates);
        if (last_focused)
        {
            last_focused->setFocus();
        }
    }

    kak_widget->setVisible(visible);
}

bool MainWindow::getWindowVisible(const QString &client_name)
{
    KakouneWidget *kak_widget = findKakouneWidget(client_name);

    if (kak_widget == nullptr)
    {
        qWarning() << "MainWindow::setWindowVisible: Could not find KakouneWidget with client name: " << client_name;
        return false;
    }

    return kak_widget->isVisible();
}

void MainWindow::focusWindow(const QString &client_name)
{
    KakouneWidget *kak_widget = findKakouneWidget(client_name);

    if (kak_widget == nullptr)
    {
        qWarning() << "MainWindow::focusWindow: Could not find KakouneWidget with client name: " << client_name;
        return;
    }

    if (!kak_widget->isVisible()) {
        kak_widget->setVisible(true);
    }

    kak_widget->setFocus();
}

void MainWindow::focusInDirection(std::function<bool(const QRect &, const QRect &)> isWithinBounds)
{
    QWidget *focused_widget = focusWidget();
    QPointF focused_widget_pos = focused_widget->mapToGlobal(focused_widget->rect().topLeft());

    QList<KakouneWidget *> focus_candidates;

    for (KakouneWidget *kak_widget : m_windows)
    {
        if (kak_widget && kak_widget->isVisible())
        {
            QPoint kak_widget_pos = kak_widget->mapToGlobal(kak_widget->rect().topLeft());

            QRect kak_widget_rect(kak_widget_pos.x(), kak_widget_pos.y(), kak_widget->width(), kak_widget->height());
            QRect focused_widget_rect(focused_widget_pos.x(), focused_widget_pos.y(), focused_widget->width(),
                                      focused_widget->height());

            if (isWithinBounds(focused_widget_rect, kak_widget_rect))
            {
                focus_candidates.append(kak_widget);
            }
        }
    }

    KakouneWidget *last_focused = m_last_focused_filter->findLastFocusedWidget(focus_candidates);

    if (last_focused)
    {
        last_focused->setFocus();
    }
}

const qreal WIDGET_DIFF_TOLERANCE = 32;

void MainWindow::focusLeft()
{
    focusInDirection([](const QRect &focused_widget, const QRect &other_widget) {
        return abs(other_widget.right() - focused_widget.left()) < WIDGET_DIFF_TOLERANCE &&
               other_widget.top() <= focused_widget.bottom() && focused_widget.top() < other_widget.bottom();
    });
}

void MainWindow::focusRight()
{
    focusInDirection([](const QRect &focused_widget, const QRect &other_widget) {
        return abs(other_widget.left() - focused_widget.right()) < WIDGET_DIFF_TOLERANCE &&
               other_widget.top() <= focused_widget.bottom() && focused_widget.top() < other_widget.bottom();
    });
}

void MainWindow::focusUp()
{
    focusInDirection([](const QRect &focused_widget, const QRect &other_widget) {
        return abs(other_widget.bottom() - focused_widget.top()) < WIDGET_DIFF_TOLERANCE &&
               other_widget.left() <= focused_widget.right() && focused_widget.left() < other_widget.right();
    });
}

void MainWindow::focusDown()
{
    focusInDirection([](const QRect &focused_widget, const QRect &other_widget) {
        return abs(other_widget.top() - focused_widget.bottom()) < WIDGET_DIFF_TOLERANCE &&
               other_widget.left() <= focused_widget.right() && focused_widget.left() < other_widget.right();
    });
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
    installLastFocusedFilter(kakwidget);

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

void MainWindow::installLastFocusedFilter(QWidget *widget)
{
    widget->installEventFilter(m_last_focused_filter);
    const QObjectList &children = widget->children();
    for (QObject *child : children)
    {
        QWidget *childWidget = qobject_cast<QWidget *>(child);
        if (childWidget)
        {
            installLastFocusedFilter(childWidget);
        }
    }
}
