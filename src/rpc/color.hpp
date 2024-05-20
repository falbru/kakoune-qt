#ifndef RPCCOLOR_HPP
#define RPCCOLOR_HPP

#include "../colorpalette.hpp"
#include <QColor>
#include <QString>

namespace RPC
{
class Color
{
  public:
    Color(QString value);

    QString getValue() const;
    QColor toQColor(const ColorPalette &color_palette);
    QColor toQColor(const Color &default_color, const ColorPalette &color_palette);

  private:
    QString m_value;
};
} // namespace RPC

#endif
