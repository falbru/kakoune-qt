#ifndef RPCCOLOR_H
#define RPCCOLOR_H

#include <QColor>
#include <QString>

namespace RPC
{
class Color
{
  public:
    Color(QString value);

    QString getValue() const;
    QColor toQColor(const Color &default_color);

  private:
    QString m_value;
};
} // namespace RPC

#endif
