#ifndef KAKOUNEMENU_HPP
#define KAKOUNEMENU_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include <QtWidgets>

class KakouneMenu : public QWidget
{
    Q_OBJECT
  public:
    KakouneMenu(KakouneClient *client, DrawOptions *draw_options, QWidget *parent = nullptr);
    ~KakouneMenu();

  protected:
    void paintEvent(QPaintEvent *ev) override;

  private slots:
    void showMenu();
    void selectItem(int selected);

  private:
    int m_selected_item;

    const int max_item_grid_height;

    int item_grid_width;
    int item_grid_height;

    KakouneClient *m_client;
    DrawOptions *m_draw_options;

    void setupInlineMenu();
    void setupPromptMenu();
};

#endif
