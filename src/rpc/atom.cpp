#include "atom.hpp"
#include "attribute.hpp"

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
    QColor fg = m_face.getFg().toQColor(default_face.getFg(), context.color_palette);
    QColor bg = m_face.getBg().toQColor(default_face.getBg(), context.color_palette);

    int width = context.cell_size.width() * m_contents.size();
    int height = context.cell_size.height();

    context.painter.setPen(fg);

    QFont font = QFont{context.painter.font()};
    font.setItalic(default_face.hasAttribute(Attribute::italic) || m_face.hasAttribute(Attribute::italic));
    font.setBold(default_face.hasAttribute(Attribute::bold) || m_face.hasAttribute(Attribute::bold));
    context.painter.setFont(font);

    context.painter.fillRect(position.x(), position.y(), width, height, bg);

    for (int i = 0; i < m_contents.size(); i++)
    {
        context.painter.drawText(QRect(position.x() + i * context.cell_size.width(), position.y(), width, height),
                                 Qt::AlignTop, m_contents.at(i));
    }

    if (default_face.hasAttribute(Attribute::underline) || m_face.hasAttribute(Attribute::underline))
    {
        context.painter.drawLine(position.x(), position.y() + context.cell_size.height() - 1,
                                 position.x() + context.cell_size.width() * m_contents.size(),
                                 position.y() + context.cell_size.height() - 1);
    }
}
} // namespace RPC
