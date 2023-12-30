#ifndef KAKOUNEMENU_HPP
#define KAKOUNEMENU_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include <QtWidgets>

class KakouneMenu : public QWidget
{
    Q_OBJECT
  public:
    KakouneMenu(KakouneClient *client, DrawOptions* draw_options, QWidget *parent = nullptr);
    ~KakouneMenu();

    KakouneMenu *getClient();
  protected:
    void paintEvent(QPaintEvent *ev) override;
  private:
    bool visible;
    KakouneClient *m_client;
    DrawOptions *m_draw_options;
};

#endif
