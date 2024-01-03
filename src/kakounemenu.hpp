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

    const int max_item_grid_columns;

    int item_grid_rows;
    int item_grid_columns;

    KakouneClient *m_client;
    DrawOptions *m_draw_options;

    void applyInlineStyle();
    void applyPromptStyle();

    int getItemWidth();
};

#endif
