#include "kakouneinfobox.hpp"
#include <qsizepolicy.h>

KakouneInfoBox::KakouneInfoBox(KakouneClient *client, KakouneMenu *menu, DrawOptions *draw_options, QWidget *parent)
    : QWidget(parent), m_client(client), m_menu(menu), m_draw_options(draw_options)
{
    connect(m_client, &KakouneClient::showInfoBox, this, &KakouneInfoBox::showInfoBox);
    connect(m_client, &KakouneClient::hideInfoBox, this, &KakouneInfoBox::hide);

    hide();

    QFrame *borderedFrame = new QFrame(this);
    borderedFrame->setFrameShape(QFrame::Box);
    borderedFrame->setLineWidth(1);
    borderedFrame->setAutoFillBackground(true);

    QVBoxLayout *borderedFrameLayout = new QVBoxLayout(borderedFrame);
    borderedFrameLayout->setContentsMargins(3, 1, 3, 1);

    m_content = new KakouneContent(client->getMenuItems(), client->getMenuFace(), m_draw_options, borderedFrame);
    borderedFrameLayout->addWidget(m_content);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(borderedFrame);
}

void KakouneInfoBox::resizeToFitParent()
{
    QList<RPC::Line> lines = m_client->getInfoContent();
    int width = this->width();
    int height = this->height();

    if (width > parentWidget()->width())
    {
        width = parentWidget()->width();

        int cutoff_index = width / (float)m_draw_options->getCellSize().width();
        int wrapped_lines_count = 0;
        for (RPC::Line line : lines)
        {
            wrapped_lines_count += line.contentSize() / cutoff_index;
        }

        height += wrapped_lines_count * m_draw_options->getCellSize().height();
    }

    // resize(width, height);
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
    m_content->setLines(m_client->getInfoContent());
    m_content->setDefaultFace(m_client->getInfoFace());
    this->adjustSize();

    RPC::InfoStyle style = m_client->getInfoStyle();

    // resizeToFitParent();

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
