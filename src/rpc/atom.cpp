#include "atom.hpp"

namespace RPC
{
Atom::Atom(Face face, QString contents) : m_face(face), m_contents(contents)
{
}

QString Atom::getContents()
{
    return m_contents;
}

void Atom::draw(const DrawContext &context, const QPoint &position, const Face &default_face)
{
    QColor fg = m_face.getFg().toQColor(default_face.getFg());
    QColor bg = m_face.getBg().toQColor(default_face.getBg());

    int width = context.cell_size.width() * m_contents.size();
    int height = context.cell_size.height();

    context.painter.setPen(fg);
    context.painter.fillRect(position.x(), position.y(), width, height, bg);
    context.painter.drawText(QRect(position.x(), position.y(), width, height), Qt::AlignTop, m_contents);
}
} // namespace RPC
