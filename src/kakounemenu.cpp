#include "kakounemenu.hpp"
#include "rpc/line.hpp"
#include <qboxlayout.h>
#include <qnamespace.h>
#include <qscrollarea.h>
#include <qwidget.h>
#include <sys/select.h>

KakouneMenu::KakouneMenu(KakouneClient *client, DrawOptions *draw_options, QWidget *parent)
    : KakouneOverlay(parent), max_item_grid_columns(10), m_client(client), m_draw_options(draw_options)
{
    connect(m_client, &KakouneClient::showMenu, this, &KakouneMenu::showMenu);
    connect(m_client, &KakouneClient::hideMenu, this, &KakouneMenu::hide);
    connect(m_client, &KakouneClient::selectMenuItem, this, &KakouneMenu::selectItem);
    connect(m_client, &KakouneClient::refresh, this, [=]() {
        adjustSize();
        repaint();
    });

    m_items = new KakouneContentScroller(m_draw_options, m_client->getMenuFace());
    m_items->setMaximumItems(10);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_items);
    layout->setContentsMargins(2, 2, 2, 2);

    hide();
}

KakouneMenu::~KakouneMenu()
{
}

int KakouneMenu::getItemWidth()
{
    QList<RPC::Line> items = m_client->getMenuItems();

    int max_item_contentsize = 0;
    for (int i = 0; i < items.length(); i++)
    {
        max_item_contentsize = qMax(max_item_contentsize, items[i].contentSize());
    }

    return qMin(max_item_contentsize * m_draw_options->getCellSize().width(), parentWidget()->width());
}

void KakouneMenu::applyInlineStyle()
{
    QList<RPC::Line> items = m_client->getMenuItems();

    m_item_grid_rows = 1;
    m_item_grid_columns = qMin(items.size(), max_item_grid_columns);

    int item_width = getItemWidth();
    int item_grid_height = qMin(max_item_grid_columns, items.size()) * m_draw_options->getCellSize().height();

    RPC::Coord anchor = m_client->getMenuAnchor();
    QPoint anchor_position(anchor.column * m_draw_options->getCellSize().width(),
                           anchor.line * m_draw_options->getCellSize().height());
    QPoint menu_position(anchor_position.x(), anchor_position.y() + m_draw_options->getCellSize().height());

    if (menu_position.x() + item_width > parentWidget()->width())
    {
        menu_position.setX(parentWidget()->width() - item_width);
    }
    if (menu_position.y() + item_grid_height > parentWidget()->height())
    {
        menu_position.setY(anchor_position.y() - item_grid_height);
    }

    move(menu_position);
    setFixedWidth(QWIDGETSIZE_MAX);
}

void KakouneMenu::applyPromptStyle()
{
    QList<RPC::Line> items = m_client->getMenuItems();

    int item_width = getItemWidth();

    m_item_grid_rows = qMax(parentWidget()->width() / item_width, 1);
    m_item_grid_columns = qMin(items.size() / m_item_grid_rows + 1, max_item_grid_columns);

    int item_grid_height = m_item_grid_columns * m_draw_options->getCellSize().height();

    move(parentWidget()->x(), parentWidget()->y() + parentWidget()->height() - item_grid_height);
    setFixedWidth(parentWidget()->width());
}

void KakouneMenu::showMenu()
{
    m_items->setContent(m_client->getMenuItems());
    m_items->setDefaultFace(m_client->getMenuFace());
    m_items->setSelectedItem(-1);
    m_items->setSelectedFace(m_client->getSelectedMenuItemFace());
    setFace(m_client->getMenuFace());

    updateOverlay();

    if (m_client->getMenuStyle() == RPC::MenuStyle::INLINE)
    {
        applyInlineStyle();
    }
    else
    {
        applyPromptStyle();
    }

    show();
}

void KakouneMenu::selectItem(int selected)
{
    if (selected >= m_client->getMenuItems().count())
    {
        m_items->setSelectedItem(-1);
    }
    else
    {
        m_items->setSelectedItem(selected);
    }
}
