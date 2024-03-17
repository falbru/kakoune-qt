#ifndef KAKOUNEWIDGETWITHSTATUSBAR_HPP
#define KAKOUNEWIDGETWITHSTATUSBAR_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include "kakounewidget.hpp"
#include <qwidget.h>
class KakouneWidgetWithStatusBar : public QWidget
{
    Q_OBJECT

    public:
      KakouneWidgetWithStatusBar(const QString &session_id, DrawOptions *draw_options, QWidget *parent = nullptr);
      ~KakouneWidgetWithStatusBar();

      KakouneClient *getClient();
    signals:
      void finished();
    private:
      KakouneWidget *m_kakoune_widget;
};

#endif
