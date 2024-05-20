#ifndef DRAWOPTIONS_HPP
#define DRAWOPTIONS_HPP

#include "colorpalette.hpp"
#include <QObject>
#include <QPainter>
#include <QSize>

class DrawOptions : public QObject
{
    Q_OBJECT

  public:
    DrawOptions();
    ~DrawOptions();

    QFont getFont();
    QSize getCellSize();
    ColorPalette getColorPalette();

    void setFont(const QString &font_name, int font_size);
    void setColorPalette(ColorPalette color_palette);

  signals:
    void updated();

  private:
    QFont m_font;
    QSize m_cell_size;
    ColorPalette m_color_palette;
};

#endif
