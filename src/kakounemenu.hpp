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

  protected:
    void paintEvent(QPaintEvent *ev) override;

  private slots:
    void showMenu();
    void selectItem(int selected);

  private:
    const int menu_max_height;

    bool m_visible;
    int m_selected_item;

    KakouneClient *m_client;
    DrawOptions *m_draw_options;
};

#endif
