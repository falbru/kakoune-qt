#ifndef DRAWCONTEXT_HPP
#define DRAWCONTEXT_HPP

#include "colorpalette.hpp"
#include <QPainter>
#include <QSize>

struct DrawContext
{
    QPainter &painter;
    const ColorPalette color_palette;
    const QSize &cell_size;
};

#endif
