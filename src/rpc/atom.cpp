#include "atom.hpp"
#include "attribute.hpp"
#include <qtextboundaryfinder.h>

namespace RPC {
Atom::Atom(Face face, QString contents) : m_face(face), m_contents(contents) {}

QString Atom::getContents() { return m_contents; }

Face Atom::getFace() { return m_face; }

class CustomGraphemeFinder {
public:
  CustomGraphemeFinder(const QString &text) : m_text(text) {
    m_finder = QTextBoundaryFinder(QTextBoundaryFinder::Grapheme, text);
  }

  int nextBoundary() {
    // If we're at the end of the text, return -1
    if (m_finder.position() >= m_text.length()) {
        return -1;
    }

    // If we're starting at a ZWJ, skip it
    int start_pos = m_finder.position();
    if (start_pos < m_text.length() && m_text[start_pos].unicode() == 0x200D) {
        m_finder.setPosition(start_pos + 1);
        return start_pos + 1;
    }

    int pos = m_finder.toNextBoundary();

    // If no more boundaries or invalid position, return -1
    if (pos < 0 || pos > m_text.length()) {
        return -1;
    }

    // Scan for ZWJ in the current segment
    for (int i = start_pos; i < pos; ++i) {
        if (m_text[i].unicode() == 0x200D) {
            // Found ZWJ - set position after it and continue
            m_finder.setPosition(i + 1);
            return i;
        }
    }

    // If we're at a ZWJ, skip it and find next boundary
    if (pos < m_text.length() && m_text[pos].unicode() == 0x200D) {
        m_finder.setPosition(pos + 1);
        return nextBoundary();
    }

    return pos;
}

private:
  QString m_text;
  QTextBoundaryFinder m_finder;
};

void Atom::draw(const DrawContext &context, const QPoint &position,
                const Face &default_face) {
  QColor fg = m_face.getFgAsQColor(context.color_palette, default_face);
  QColor bg = m_face.getBgAsQColor(context.color_palette, default_face);

  int width = context.cell_size.width() * m_contents.size();
  int height = context.cell_size.height();

  context.painter.setPen(fg);

  QFont font = QFont{context.painter.font()};
  font.setItalic(default_face.hasAttribute(Attribute::italic) ||
                 m_face.hasAttribute(Attribute::italic));
  font.setBold(default_face.hasAttribute(Attribute::bold) ||
               m_face.hasAttribute(Attribute::bold));
  context.painter.setFont(font);

  // Draw background
  context.painter.fillRect(position.x(), position.y(), width, height, bg);

  // Draw foreground
  CustomGraphemeFinder grapheme_finder(m_contents);
  int start_offset = 0;
  QPoint grapheme_position = position;
  int next_boundary;
  while ((next_boundary = grapheme_finder.nextBoundary()) >= 0) {
    int length = next_boundary - start_offset;
    QString grapheme = m_contents.mid(start_offset, length);
    context.painter.drawText(QRect(grapheme_position.x(), grapheme_position.y(),
                                   context.cell_size.width() * length,
                                   context.cell_size.height()),
                             Qt::AlignCenter, grapheme);
    grapheme_position.setX(grapheme_position.x() +
                           context.cell_size.width() * length);
    start_offset = next_boundary;
  }

  // Draw underline
  if (default_face.hasAttribute(Attribute::underline) ||
      m_face.hasAttribute(Attribute::underline)) {
    context.painter.drawLine(
        position.x(), position.y() + context.cell_size.height() - 1,
        position.x() + context.cell_size.width() * m_contents.size(),
        position.y() + context.cell_size.height() - 1);
  }
}
} // namespace RPC
