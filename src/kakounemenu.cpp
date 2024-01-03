#include "kakounemenu.hpp"
#include "rpc/line.hpp"

KakouneMenu::KakouneMenu(KakouneClient *client, DrawOptions *draw_options, QWidget *parent)
    : QWidget(parent), m_selected_item(-1), max_item_grid_height(10), m_client(client),
      m_draw_options(draw_options)
{
    connect(m_client, &KakouneClient::showMenu, this, &KakouneMenu::showMenu);
    connect(m_client, &KakouneClient::hideMenu, this, &KakouneMenu::hide);
    connect(m_client, &KakouneClient::selectMenuItem, this, &KakouneMenu::selectItem);

    hide();
}

KakouneMenu::~KakouneMenu()
{
}

void KakouneMenu::setupInlineMenu() {
    RPC::Coord anchor = m_client->getMenuAnchor();
    move(anchor.column * m_draw_options->getCellSize().width(),
         (anchor.line + 1) * m_draw_options->getCellSize().height());

    QList<RPC::Line> items = m_client->getMenuItems();

    int max_item_contentsize = 0;
    for (int i = 0; i < items.length(); i++)
    {
        max_item_contentsize = qMax(max_item_contentsize, items[i].contentSize());
    }

    resize(max_item_contentsize * m_draw_options->getCellSize().width(),
           qMin(max_item_grid_height, items.length()) * m_draw_options->getCellSize().height());

}

void KakouneMenu::setupPromptMenu() {
  QList<RPC::Line> items = m_client->getMenuItems();
  if (items.size() == 0) return;
  int max_item_contentsize = 0;
  for (int i = 0; i < items.length(); i++)
  {
      max_item_contentsize = qMax(max_item_contentsize, items[i].contentSize());
  }
  int grid_x = parentWidget()->width() / (max_item_contentsize * m_draw_options->getCellSize().width());
  int menu_height = qMin(items.length() / grid_x + 1, max_item_grid_height) * m_draw_options->getCellSize().height();

  move(parentWidget()->x(), parentWidget()->y() + parentWidget()->height() - menu_height);
  resize(parentWidget()->width(), menu_height);

  item_grid_width = grid_x;
  item_grid_height = qMin(items.length() / grid_x + 1, max_item_grid_height);
}

void KakouneMenu::showMenu()
{
    show();

    if (m_client->getMenuStyle() == RPC::MenuStyle::INLINE) {
      setupInlineMenu();
    }else {
      setupPromptMenu();
    }
}

void KakouneMenu::selectItem(int selected)
{
    if (selected >= m_client->getMenuItems().count())
    {
        m_selected_item = -1;
    }
    else
    {
        m_selected_item = selected;
    }
}

void KakouneMenu::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setFont(m_draw_options->getFont());

    DrawContext context{painter, m_draw_options->getCellSize()};

    painter.fillRect(0, 0, width(), height(), m_client->getMenuFace().getBg().toQColor());

    QList<RPC::Line> items = m_client->getMenuItems();

    // int scrolling_index_offset = (m_selected_item / menu_max_height) * menu_max_height;
    // for (int i = 0; i < items.size() - scrolling_index_offset && i < menu_max_height; ++i)
    // {
    //     int index = scrolling_index_offset + i;
    //     QPoint position(0, i * m_draw_options->getCellSize().height());
    //     items[index].draw(context, position,
    //                       m_selected_item == index ? m_client->getSelectedMenuItemFace() : m_client->getMenuFace());
    // }

    int scrolling_index_offset = (m_selected_item / (item_grid_width * item_grid_height)) * item_grid_width * item_grid_height;
    for (int i = 0; i < qMin(items.size() - scrolling_index_offset, item_grid_width * item_grid_height); ++i)
    {
        int index = scrolling_index_offset + i;
        QPoint position(i / item_grid_height * width() / item_grid_width, (i % item_grid_height) * m_draw_options->getCellSize().height());
        if (m_selected_item == index) {
          painter.fillRect(position.x(), position.y(), width() / item_grid_width, m_draw_options->getCellSize().height(), m_client->getSelectedMenuItemFace().getBg().toQColor());
        }
        items[index].draw(context, position,
                          m_selected_item == index ? m_client->getSelectedMenuItemFace() : m_client->getMenuFace());
    }
}
