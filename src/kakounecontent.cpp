#include "kakounecontent.hpp"

KakouneContent::KakouneContent(QList<RPC::Line> lines, RPC::Face default_face,
                               DrawOptions *draw_options, QWidget *parent)
    : QWidget(parent), m_lines(lines), m_default_face(default_face),
      m_draw_options(draw_options) {}

KakouneContent::~KakouneContent() {}

QSize KakouneContent::sizeHint() const {
  int longest_line_length = 0;
  for (int i = 0; i < m_lines.length(); i++) {
    longest_line_length = qMax(longest_line_length, m_lines[i].contentSize());
  }

  int width = longest_line_length * m_draw_options->getCellSize().width();
  int height = m_lines.size() * m_draw_options->getCellSize().height();

  return QSize(width, height);
}

void KakouneContent::paintEvent(QPaintEvent *ev) {
  QPainter painter(this);
  DrawContext context{painter, m_draw_options->getColorPalette(),
                      m_draw_options->getCellSize()};

  painter.setFont(m_draw_options->getFont());
  painter.fillRect(rect(), m_default_face.getBgAsQColor(context.color_palette));

  for (int i = 0; i < m_lines.size(); ++i) {
    QPoint position(0, i * m_draw_options->getCellSize().height());
    m_lines[i].draw(context, position, m_default_face);
  }
}

QList<RPC::Line> KakouneContent::getLines() { return m_lines; }

void KakouneContent::setLines(QList<RPC::Line> lines) {
  m_lines = lines;
  updateGeometry();
}

RPC::Face KakouneContent::getDefaultFace() { return m_default_face; }

void KakouneContent::setDefaultFace(RPC::Face face) { m_default_face = face; }
