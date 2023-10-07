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

  private:
    Color m_fg;
    Color m_bg;
    QList<Attribute> m_attributes;
};
} // namespace RPC

#endif
