#include "kakouneinfobox.hpp"

KakouneInfoBox::KakouneInfoBox(KakouneClient *client, KakouneMenu *menu, DrawOptions *draw_options, QWidget *parent)
    : QWidget(parent), m_client(client), m_menu(menu), m_draw_options(draw_options)
{
    connect(m_client, &KakouneClient::showInfoBox, this, &KakouneInfoBox::showInfoBox);
    connect(m_client, &KakouneClient::hideInfoBox, this, &KakouneInfoBox::hide);

    hide();
}
void KakouneInfoBox::resizeToFitContent()
{
    QList<RPC::Line> lines = m_client->getInfoContent();

    int max_item_contentsize = 0;
    for (int i = 0; i < lines.length(); i++)
    {
        max_item_contentsize = qMax(max_item_contentsize, lines[i].contentSize());
    }

    int width = max_item_contentsize * m_draw_options->getCellSize().width();
    int height = lines.size() * m_draw_options->getCellSize().height();

    if (m_client->getInfoTitle().contentSize() > 0)
    {
        height += m_draw_options->getCellSize().height();
    }

    resize(width, height);
}

void KakouneInfoBox::applyPromptStyle()
{
    int x = parentWidget()->width() - width();

    if (m_menu->isVisible() && m_client->getMenuStyle() == RPC::MenuStyle::PROMPT)
    {
        move(x, m_menu->y() - height());
    }
    else
    {
        move(x, parentWidget()->height() - height());
    }
}

void KakouneInfoBox::applyInlineStyle(InlineType type)
{
    RPC::Coord anchor = m_client->getInfoAnchor();
    QPoint anchor_position(anchor.column * m_draw_options->getCellSize().width(),
                           anchor.line * m_draw_options->getCellSize().height());

    int below_y = anchor_position.y() + m_draw_options->getCellSize().height();
    int above_y = anchor_position.y() - height();

    QPoint info_box_position(anchor_position.x(), type == InlineType::ABOVE ? above_y : below_y);

    if (info_box_position.x() + width() > parentWidget()->width())
    {
        info_box_position.setX(parentWidget()->width() - width());
    }

    if (info_box_position.y() < 0)
    {
        info_box_position.setY(below_y);
    }
    else if (info_box_position.y() + height() > parentWidget()->height())
    {
        info_box_position.setY(above_y);
    }

    move(info_box_position);
}

void KakouneInfoBox::applyMenuDocStyle()
{
    if (!m_menu->isVisible())
    {
        applyPromptStyle();
        return;
    }

    QPoint info_box_position(m_menu->x() + m_menu->width(), m_menu->y());

    if (info_box_position.x() + width() > parentWidget()->width())
    {
        info_box_position.setX(m_menu->x() - width());
    }
    if (info_box_position.y() + height() > parentWidget()->height())
    {
        info_box_position.setY(parentWidget()->height() - height());
    }

    move(info_box_position);
}

void KakouneInfoBox::applyModalStyle()
{
    move((parentWidget()->width() - width()) / 2, (parentWidget()->height() - height()) / 2);
}

void KakouneInfoBox::showInfoBox()
{
    RPC::InfoStyle style = m_client->getInfoStyle();

    resizeToFitContent();

    switch (style)
    {
    case RPC::InfoStyle::PROMPT:
        applyPromptStyle();
        break;
    case RPC::InfoStyle::INLINE:
        applyInlineStyle(InlineType::ANY);
        break;
    case RPC::InfoStyle::INLINE_ABOVE:
        applyInlineStyle(InlineType::ABOVE);
        break;
    case RPC::InfoStyle::INLINE_BELOW:
        applyInlineStyle(InlineType::BELOW);
        break;
    case RPC::InfoStyle::MENU_DOC:
        applyMenuDocStyle();
        break;
    case RPC::InfoStyle::MODAL:
        applyModalStyle();
        break;
    }

    show();
}

KakouneInfoBox::~KakouneInfoBox()
{
}

void KakouneInfoBox::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setFont(m_draw_options->getFont());

    DrawContext context{painter, m_draw_options->getCellSize()};

    painter.fillRect(0, 0, width(), height(), m_client->getInfoFace().getBg().toQColor());

    RPC::Line title = m_client->getInfoTitle();
    if (title.contentSize() > 0)
    {
        title.draw(context, QPoint((width() - title.contentSize() * m_draw_options->getCellSize().width()) / 2, 0),
                   m_client->getInfoFace());
    }

    QList<RPC::Line> lines = m_client->getInfoContent();
    for (int i = 0; i < lines.size(); ++i)
    {
        QPoint position(0, (title.contentSize() > 0 ? (i + 1) : i) * m_draw_options->getCellSize().height());
        lines[i].draw(context, position, m_client->getInfoFace());
    }
}
