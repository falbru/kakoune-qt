#include "face.hpp"
#include "attribute.hpp"
#include <qdebug.h>

namespace RPC
{
Face::Face() : m_fg("default"), m_bg("default"), m_attributes()
{
}

Face::Face(Color fg, Color bg, QList<Attribute> attributes) : m_fg(fg), m_bg(bg), m_attributes(attributes)
{
}

Color Face::getFg() const
{
    return m_fg;
}

Color Face::getBg() const
{
    return m_bg;
}

QColor Face::getFgAsQColor(const ColorPalette &color_palette) const
{
    return getFg().toQColor(color_palette, color_palette.getFg());
}

QColor Face::getFgAsQColor(const ColorPalette &color_palette, const Face &default_face) const
{
    return getFg().toQColor(color_palette, default_face.getFgAsQColor(color_palette));
}

QColor Face::getBgAsQColor(const ColorPalette &color_palette) const
{
    return getBg().toQColor(color_palette, color_palette.getBg());
}

QColor Face::getBgAsQColor(const ColorPalette &color_palette, const Face &default_face) const
{
    return getBg().toQColor(color_palette, default_face.getBgAsQColor(color_palette));
}

bool Face::hasAttribute(Attribute attribute) const
{
    return m_attributes.contains(attribute);
}

} // namespace RPC
