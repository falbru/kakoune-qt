#include "lastfocusedfilter.hpp"
#include "kakounewidget.hpp"

LastFocusedFilter::LastFocusedFilter(QObject *parent) : QObject(parent), m_counter(0) {}

size_t LastFocusedFilter::getLastTimeFocused(KakouneWidget* kak_widget) const {
    return m_last_focused_widgets.value(kak_widget, 0);
}

bool LastFocusedFilter::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::FocusIn) {
    QWidget *widget = qobject_cast<QWidget *>(obj);

    if (widget) {
      if (KakouneWidget *kak_widget =
              KakouneWidget::findParentKakouneWidget(widget)) {
        m_last_focused_widgets[kak_widget] = ++m_counter;
      }
    }
  }

  return QObject::eventFilter(obj, event);
}
