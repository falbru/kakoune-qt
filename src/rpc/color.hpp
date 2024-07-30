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
    QColor toQColor(const ColorPalette &color_palette, const QColor &fallback_color) const;

  private:
    QString m_value;
};
} // namespace RPC

#endif
