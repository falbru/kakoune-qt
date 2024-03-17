#include "kakounewidgetwithstatusbar.hpp"
#include "statusbar.hpp"

KakouneWidgetWithStatusBar::KakouneWidgetWithStatusBar(const QString &session_id, DrawOptions *draw_options, QWidget *parent)
{
      KakouneWidget *kakwidget = new KakouneWidget(session_id, draw_options, parent);
      connect(kakwidget, &KakouneWidget::finished, this, [=]() { emit finished();});

      StatusBar *status_bar = new StatusBar(draw_options);
      connect(kakwidget, &KakouneWidget::refresh, this, [=]() { status_bar->repaint(); });
      status_bar->setActiveClient(kakwidget->getClient());

      QVBoxLayout *layout = new QVBoxLayout();
      layout->setSpacing(0);
      layout->setContentsMargins(0, 0, 0, 0);
      layout->addWidget(kakwidget);
      layout->addWidget(status_bar);

      this->setLayout(layout); 
}

KakouneWidgetWithStatusBar::~KakouneWidgetWithStatusBar()
{

}

KakouneClient* KakouneWidgetWithStatusBar::getClient()
{
  return m_kakoune_widget->getClient();
}
