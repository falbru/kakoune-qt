#include "kakounecontent.hpp"

KakouneContent::KakouneContent(RPC::Face default_face, DrawOptions *draw_options, QWidget *parent) : QWidget(parent), m_draw_options(draw_options), m_default_face(default_face) {

}

KakouneContent::~KakouneContent() {

}

void KakouneContent::setLines(QList<RPC::Line> lines)
{
    m_lines = lines;
}

void KakouneContent::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setFont(m_draw_options->getFont());

    DrawContext context{painter, m_draw_options->getColorPalette(), m_draw_options->getCellSize()};

    QPoint position = pos();
    for (RPC::Line line : m_lines) {
        line.draw(context, position, m_default_face);
        position.setY(position.y() + m_draw_options->getCellSize().height());
    }
}
