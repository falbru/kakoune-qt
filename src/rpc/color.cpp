#include "color.h"

namespace RPC
{
Color::Color(QString value) : m_value(value)
{
}

QString Color::getValue() const
{
    return m_value;
}

QColor Color::toQColor(const Color &default_color)
{
    QString color = m_value;

    if (color == "default")
    {
        color = default_color.getValue();
    }
    else if (color == "black" || color == "bright-black")
    {
        return Qt::gray;
    }
    else if (color == "red" || color == "bright-red")
    {
        return Qt::red;
    }
    else if (color == "green" || color == "bright-green")
    {
        return Qt::green;
    }
    else if (color == "yellow" || color == "bright-yellow")
    {
        return Qt::yellow;
    }
    else if (color == "blue" || color == "bright-blue")
    {
        return Qt::blue;
    }
    else if (color == "magenta" || color == "bright-magenta")
    {
        return Qt::magenta;
    }
    else if (color == "cyan" || color == "bright-cyan")
    {
        return Qt::cyan;
    }
    else if (color == "white" || color == "bright-white")
    {
        return Qt::white;
    }

    QString hexCode = color.mid(color.indexOf(":") + 1);
    return QColor("#" + hexCode);
}
} // namespace RPC
