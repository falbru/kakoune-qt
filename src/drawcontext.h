#ifndef DRAWCONTEXT_H
#define DRAWCONTEXT_H

#include <QPainter>
#include <QSize>

struct DrawContext
{
    QPainter &painter;
    const QSize &cell_size;
};

#endif
