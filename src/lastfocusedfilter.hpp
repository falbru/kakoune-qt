#ifndef LASTFOCUSEDFILTER_HPP
#define LASTFOCUSEDFILTER_HPP

#include "kakounewidget.hpp"
#include <QEvent>
#include <QObject>
#include <QWidget>

class LastFocusedFilter : public QObject
{
    Q_OBJECT

  public:
    LastFocusedFilter(QObject *parent = nullptr);

    size_t getLastTimeFocused(KakouneWidget *kak_widget) const;

  protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

  signals:
    void lastFocusedChanged(QWidget *widget);

  private:
    int m_counter;
    QMap<KakouneWidget *, size_t> m_last_focused_widgets;
};

#endif
