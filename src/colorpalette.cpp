#include "colorpalette.hpp"
#include <qnamespace.h>

ColorPalette::ColorPalette()
    : m_gray(Qt::gray), m_red(Qt::red), m_green(Qt::green), m_yellow(Qt::yellow), m_blue(Qt::blue),
      m_magenta(Qt::magenta), m_cyan(Qt::cyan), m_white(Qt::white)
{
}

void ColorPalette::setGray(QColor color)
{
    m_gray = color;
}
void ColorPalette::setRed(QColor color)
{
    m_red = color;
}
void ColorPalette::setGreen(QColor color)
{
    m_green = color;
}
void ColorPalette::setYellow(QColor color)
{
    m_yellow = color;
}
void ColorPalette::setBlue(QColor color)
{
    m_blue = color;
}
void ColorPalette::setMagenta(QColor color)
{
    m_magenta = color;
}
void ColorPalette::setCyan(QColor color)
{
    m_cyan = color;
}
void ColorPalette::setWhite(QColor color)
{
    m_white = color;
}

QColor ColorPalette::getGray() const
{
    return m_gray;
}
QColor ColorPalette::getRed() const
{
    return m_red;
}
QColor ColorPalette::getGreen() const
{
    return m_green;
}
QColor ColorPalette::getYellow() const
{
    return m_yellow;
}
QColor ColorPalette::getBlue() const
{
    return m_blue;
}
QColor ColorPalette::getMagenta() const
{
    return m_magenta;
}
QColor ColorPalette::getCyan() const
{
    return m_cyan;
}
QColor ColorPalette::getWhite() const
{
    return m_white;
}
