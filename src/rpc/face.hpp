#ifndef RPCFACE_HPP
#define RPCFACE_HPP

#include "attribute.hpp"
#include "color.hpp"
#include <QList>

namespace RPC
{
class Face
{
  public:
    Face();
    Face(Color fg, Color bg, QList<Attribute> attributes);

    Color getFg() const;
    Color getBg() const;
    QColor getFgAsQColor(const ColorPalette &color_palette) const;
    QColor getFgAsQColor(const ColorPalette &color_palette, const Face &default_face) const;
    QColor getBgAsQColor(const ColorPalette &color_palette) const;
    QColor getBgAsQColor(const ColorPalette &color_palette, const Face &default_face) const;

    bool hasAttribute(Attribute attribute) const;

  private:
    Color m_fg;
    Color m_bg;
    QList<Attribute> m_attributes;
};
} // namespace RPC

#endif
