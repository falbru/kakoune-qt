#ifndef KAKOUNEMENU_HPP
#define KAKOUNEMENU_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include "kakounecontent.hpp"
#include "kakouneoverlay.hpp"
#include <QtWidgets>
#include <qscrollarea.h>

class KakouneMenu : public KakouneOverlay
{
    Q_OBJECT
  public:
    KakouneMenu(KakouneClient *client, DrawOptions *draw_options, QWidget *parent = nullptr);
    ~KakouneMenu();

  private slots:
    void showMenu();
    void selectItem(int selected);

  private:
    int m_selected_item;
    KakouneContent* m_items;
    QScrollArea* m_scroll_area;

    const int max_item_grid_columns;

    int m_item_grid_rows;
    int m_item_grid_columns;

    KakouneClient *m_client;
    DrawOptions *m_draw_options;

    void applyInlineStyle();
    void applyPromptStyle();

    int getItemWidth();
};

#endif
