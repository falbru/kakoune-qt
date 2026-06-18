#include "kakounelistcontent.hpp"
#include "kakounecontent.hpp"

KakouneListContent::KakouneListContent(DrawOptions *draw_options, RPC::Face default_face, QWidget *parent)
    : KakouneContent(draw_options, default_face, parent)
{
    m_selected_item = -1;
}

KakouneListContent::~KakouneListContent()
{
}

void KakouneListContent::setContent(QList<RPC::Line> content)
{
    m_selected_item = -1;
    KakouneContent::setContent(content);
}

void KakouneListContent::setSelectedItem(int selected_item)
{
    m_selected_item = selected_item;
    repaint();
}

int KakouneListContent::getSelectedItem() const
{
    return m_selected_item;
}

void KakouneListContent::paintEvent(QPaintEvent *)
{
    // TODO this code is mostly copy-pasted from KakouneContent. Reuse in a smarter way?
    QPainter painter(this);
    DrawContext context{painter, m_draw_options->getColorPalette(), m_draw_options->getCellSize()};

    painter.setFont(m_draw_options->getFont());
    painter.fillRect(rect(), getDefaultFace().getBgAsQColor(context.color_palette));

    QList<RPC::Line> lines = getContent();
    int max_characters_per_line = width() / (float)m_draw_options->getCellSize().width();
    QPoint position(0, 0);
    for (int i = 0; i < lines.size(); ++i)
    {
        RPC::Line line = lines[i];
        RPC::Line cutoff;

        do
        {
            if (line.contentSize() > max_characters_per_line)
            {
                cutoff = line.slice(max_characters_per_line);
                line = line.slice(0, max_characters_per_line);
            }
            else
            {
                cutoff = RPC::Line();
            }

            line.draw(context, position, m_selected_item == i ? m_selected_face : m_default_face);
            position.setY(position.y() + m_draw_options->getCellSize().height());

            line = cutoff;
        } while (line.contentSize() > 0);
    }
}

void KakouneListContent::setSelectedFace(RPC::Face selected_face)
{
    m_selected_face = selected_face;
}

const RPC::Face &KakouneListContent::getSelectedFace() const
{
    return m_selected_face;
}
