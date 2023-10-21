#include "drawoptions.hpp"

DrawOptions::DrawOptions()
{
}

DrawOptions::~DrawOptions()
{
}

QFont DrawOptions::getFont()
{
    return m_font;
}

QSize DrawOptions::getCellSize()
{
    return m_cell_size;
}

void DrawOptions::setFont(const QString &font_name, int font_size)
{
    m_font = QFont(font_name, font_size);

    QFontMetrics font_metrics(m_font);
    QRect boundingBoxSingleChar = font_metrics.boundingRect("A");
    m_cell_size =
        QSize(boundingBoxSingleChar.width(), boundingBoxSingleChar.height()); // FIXME only works for monospaced fonts
}
