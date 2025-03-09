#include "kakounecontent.hpp"
#include "rpc/face.hpp"
#include <iostream>
#include <qsizepolicy.h>

KakouneContent::KakouneContent(DrawOptions* draw_options, RPC::Face default_face, QWidget *parent) : QWidget(parent), m_draw_options(draw_options), m_default_face(default_face) {
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setHeightForWidth(true);
    policy.setVerticalStretch(1);
    setSizePolicy(policy);
}

KakouneContent::~KakouneContent() {

}

QSize KakouneContent::sizeHint() const {
    QList<RPC::Line> lines = getContent();

    int max_item_contentsize = 0;
    for (int i = 0; i < lines.length(); i++)
    {
        max_item_contentsize = qMax(max_item_contentsize, lines[i].contentSize());
    }

    int width = max_item_contentsize * m_draw_options->getCellSize().width();
    int height = lines.size() * m_draw_options->getCellSize().height();

    return QSize(width, height);
}

void KakouneContent::setContent(QList<RPC::Line> content) {
    m_content = content;
    updateGeometry();
    parentWidget()->layout()->invalidate();
    parentWidget()->adjustSize();
}

const QList<RPC::Line>& KakouneContent::getContent() const {
    return m_content;
}

void KakouneContent::setDefaultFace(RPC::Face default_face) {
    m_default_face = default_face;
}

const RPC::Face &KakouneContent::getDefaultFace() const {
    return m_default_face;
}

int KakouneContent::heightForWidth(int width) const {
    int height = sizeHint().height();
    if (width < sizeHint().width()) {
        int cutoff_index = width / (float)m_draw_options->getCellSize().width();
        int wrapped_lines_count = 0;

        for (RPC::Line line : m_content) {
          wrapped_lines_count += line.contentSize() / cutoff_index;
        }

        height += wrapped_lines_count * m_draw_options->getCellSize().height();
    }
    return height;
}


void KakouneContent::paintEvent(QPaintEvent *)
{
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

            line.draw(context, position, m_default_face);
            position.setY(position.y() + m_draw_options->getCellSize().height());

            line = cutoff;
        }while (line.contentSize() > 0);
    }
}

QSize KakouneContent::minimumSizeHint() const {
    return QSize(0, 0);

}

bool KakouneContent::hasHeightForWidth() const {
    return true;
}
