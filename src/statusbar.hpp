#ifndef STATUSBAR_HPP
#define STATUSBAR_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include <QtWidgets>

class StatusBar : public QWidget
{
    Q_OBJECT
  public:
    StatusBar(DrawOptions *draw_options, QWidget *parent = nullptr);
    ~StatusBar();

    void setActiveClient(KakouneClient *client);

  protected:
    void paintEvent(QPaintEvent *ev) override;

  private:
    KakouneClient *m_client;
    DrawOptions *m_draw_options;
};

#endif
