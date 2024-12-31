#include "atom.hpp"
#include "attribute.hpp"
#include <qtextboundaryfinder.h>

namespace RPC
{
Atom::Atom(Face face, QString contents) : m_face(face), m_contents(contents)
{
}

QString Atom::getContents()
{
    return m_contents;
}

Face Atom::getFace()
{
    return m_face;
}

void Atom::draw(const DrawContext &context, const QPoint &position, const Face &default_face)
{
    QColor fg = m_face.getFgAsQColor(context.color_palette, default_face);
    QColor bg = m_face.getBgAsQColor(context.color_palette, default_face);

    int width = context.cell_size.width() * m_contents.size();
    int height = context.cell_size.height();

    context.painter.setPen(fg);

    QFont font = QFont{context.painter.font()};
    font.setItalic(default_face.hasAttribute(Attribute::italic) || m_face.hasAttribute(Attribute::italic));
    font.setBold(default_face.hasAttribute(Attribute::bold) || m_face.hasAttribute(Attribute::bold));
    context.painter.setFont(font);

    // Draw background
    context.painter.fillRect(position.x(), position.y(), width, height, bg);

    // Draw foreground
    QTextBoundaryFinder grapheme_finder(QTextBoundaryFinder::Grapheme, m_contents);
    int start_offset = 0;
    QPoint grapheme_position = position;

    while (grapheme_finder.toNextBoundary() >= 0) {
        int length = grapheme_finder.position() - start_offset;
        QString grapheme = m_contents.mid(start_offset, length);

        context.painter.drawText(QRect(grapheme_position.x(), grapheme_position.y(), context.cell_size.width() * length, context.cell_size.height()),
                         Qt::AlignCenter, grapheme);

        grapheme_position.setX(grapheme_position.x() + context.cell_size.width() * length);
        start_offset = grapheme_finder.position();
    }

    // Draw underline
    if (default_face.hasAttribute(Attribute::underline) || m_face.hasAttribute(Attribute::underline))
    {
        context.painter.drawLine(position.x(), position.y() + context.cell_size.height() - 1,
                                 position.x() + context.cell_size.width() * m_contents.size(),
                                 position.y() + context.cell_size.height() - 1);
    }
}
} // namespace RPC
