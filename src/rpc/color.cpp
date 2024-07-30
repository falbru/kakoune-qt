#include "color.hpp"

namespace RPC
{
Color::Color(QString value) : m_value(value)
{
}

QString Color::getValue() const
{
    return m_value;
}

QColor Color::toQColor(const ColorPalette &color_palette, const QColor &fallback_color) const
{
    QString color = m_value;

    if (color == "default")
    {
        return fallback_color;
    }
    else if (color == "black" || color == "bright-black")
    {
        return color_palette.getBlack();
    }
    else if (color == "red" || color == "bright-red")
    {
        return color_palette.getRed();
    }
    else if (color == "green" || color == "bright-green")
    {
        return color_palette.getGreen();
    }
    else if (color == "yellow" || color == "bright-yellow")
    {
        return color_palette.getYellow();
    }
    else if (color == "blue" || color == "bright-blue")
    {
        return color_palette.getBlue();
    }
    else if (color == "magenta" || color == "bright-magenta")
    {
        return color_palette.getMagenta();
    }
    else if (color == "cyan" || color == "bright-cyan")
    {
        return color_palette.getCyan();
    }
    else if (color == "white" || color == "bright-white")
    {
        return Qt::white;
    }

    QString hexCode = color.mid(color.indexOf(":") + 1);
    return QColor("#" + hexCode);
}
} // namespace RPC
