#ifndef DRAWCONTEXT_HPP
#define DRAWCONTEXT_HPP

#include <QPainter>
#include <QSize>

struct DrawContext
{
    QPainter &painter;
    const QSize &cell_size;
};

#endif
