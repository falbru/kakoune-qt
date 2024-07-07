#include "lastfocusedfilter.hpp"
#include "kakounewidget.hpp"

LastFocusedFilter::LastFocusedFilter(QObject *parent) : QObject(parent), m_counter(0)
{
}

size_t LastFocusedFilter::getLastTimeFocused(KakouneWidget *kak_widget) const
{
    return m_last_focused_widgets.value(kak_widget, 0);
}

bool LastFocusedFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        QWidget *widget = qobject_cast<QWidget *>(obj);

        if (widget)
        {
            if (KakouneWidget *kak_widget = KakouneWidget::findParentKakouneWidget(widget))
            {
                m_last_focused_widgets[kak_widget] = ++m_counter;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

KakouneWidget* LastFocusedFilter::findLastFocusedWidget(const QList<KakouneWidget*>& kak_widgets) const
{
    KakouneWidget *last_focused_widget = nullptr;
    int max_last_focused = INT_MIN;

    for (auto &kak_widget : kak_widgets)
    {
        int last_focused = getLastTimeFocused(kak_widget);
        if (last_focused > max_last_focused)
        {
            last_focused_widget = kak_widget;
            max_last_focused = last_focused;
        }
    }

    return last_focused_widget;
}
