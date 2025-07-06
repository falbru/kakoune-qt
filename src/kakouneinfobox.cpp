#include "kakouneinfobox.hpp"
#include "kakounecontent.hpp"
#include <qnamespace.h>

KakouneInfoBox::KakouneInfoBox(KakouneClient *client, KakouneMenu *menu, DrawOptions *draw_options, QWidget *parent)
    : KakouneOverlay(parent), m_client(client), m_menu(menu), m_draw_options(draw_options)
{
    connect(m_client, &KakouneClient::showInfoBox, this, &KakouneInfoBox::showInfoBox);
    connect(m_client, &KakouneClient::hideInfoBox, this, &KakouneInfoBox::hide);
    connect(m_client, &KakouneClient::refresh, this, [=]() {
        adjustSize();
        repaint();
    });

    m_content = new KakouneContent(draw_options, client->getInfoFace());
    m_title = new KakouneContent(draw_options, client->getInfoFace());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_title, 0, Qt::AlignHCenter);

    m_title_separator = new QFrame();
    m_title_separator->setFrameShape(QFrame::HLine);
    layout->addWidget(m_title_separator);

    layout->addWidget(m_content);

    hide();
}

void KakouneInfoBox::applyPromptStyle()
{
    int x = parentWidget()->width() - width();
    int y = parentWidget()->height() - height();

    if (m_menu->isVisible() && m_client->getMenuStyle() == RPC::MenuStyle::PROMPT)
    {
        y = m_menu->y() - height();
    }

    move(qMax(x, 0), qMax(0, y));
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

    move(qMax(0, info_box_position.x()), qMax(0, info_box_position.y()));
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

    move(qMax(0, info_box_position.x()), qMax(0, info_box_position.y()));
}

void KakouneInfoBox::applyModalStyle()
{
    int x = (parentWidget()->width() - width()) / 2;
    int y = (parentWidget()->height() - height()) / 2;
    move(qMax(x, 0), qMax(0, y));
}

void KakouneInfoBox::showInfoBox()
{
    RPC::InfoStyle style = m_client->getInfoStyle();

    if (m_client->getInfoTitle().contentSize() > 0)
    {
        m_title->setContent(QList{m_client->getInfoTitle()});
        m_title->show();
        m_title_separator->show();
    }
    else
    {
        m_title->hide();
        m_title_separator->hide();
    }
    m_content->setContent(m_client->getInfoContent());
    m_title->setDefaultFace(m_client->getInfoFace());
    m_content->setDefaultFace(m_client->getInfoFace());
    setFace(m_client->getInfoFace());

    updateOverlay();

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
