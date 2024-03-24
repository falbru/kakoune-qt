#include "kakounemenu.hpp"
#include "rpc/line.hpp"

KakouneMenu::KakouneMenu(KakouneClient *client, DrawOptions *draw_options, QWidget *parent)
    : QWidget(parent), m_selected_item(-1), max_item_grid_columns(10), m_client(client), m_draw_options(draw_options)
{
    connect(m_client, &KakouneClient::showMenu, this, &KakouneMenu::showMenu);
    connect(m_client, &KakouneClient::hideMenu, this, &KakouneMenu::hide);
    connect(m_client, &KakouneClient::selectMenuItem, this, &KakouneMenu::selectItem);

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

    return max_item_contentsize * m_draw_options->getCellSize().width();
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
    resize(item_width, item_grid_height);
}

void KakouneMenu::applyPromptStyle()
{
    QList<RPC::Line> items = m_client->getMenuItems();

    int item_width = getItemWidth();

    m_item_grid_rows = qMax(parentWidget()->width() / item_width, 1);
    m_item_grid_columns = qMin(items.size() / m_item_grid_rows + 1, max_item_grid_columns);

    int item_grid_height = m_item_grid_columns * m_draw_options->getCellSize().height();

    move(parentWidget()->x(), parentWidget()->y() + parentWidget()->height() - item_grid_height);
    resize(parentWidget()->width(), item_grid_height);
}

void KakouneMenu::showMenu()
{
    show();

    if (m_client->getMenuStyle() == RPC::MenuStyle::INLINE)
    {
        applyInlineStyle();
    }
    else
    {
        applyPromptStyle();
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

    int item_grid_capacity = m_item_grid_rows * m_item_grid_columns;
    int scrolling_index_offset =
        m_selected_item == -1 ? 0 : (m_selected_item / item_grid_capacity) * item_grid_capacity;

    for (int i = 0; i < qMin(items.size() - scrolling_index_offset, item_grid_capacity); ++i)
    {
        int index = scrolling_index_offset + i;
        int item_width = width() / m_item_grid_rows;
        int item_height = m_draw_options->getCellSize().height();

        QPoint position(i / m_item_grid_columns * item_width, (i % m_item_grid_columns) * item_height);

        if (m_selected_item == index)
        {
            painter.fillRect(position.x(), position.y(), item_width, item_height,
                             m_client->getSelectedMenuItemFace().getBg().toQColor());
            items[index].draw(context, position, m_client->getSelectedMenuItemFace());
        }
        else
        {
            items[index].draw(context, position, m_client->getMenuFace());
        }
    }
}
