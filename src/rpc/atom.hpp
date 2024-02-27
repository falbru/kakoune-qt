#ifndef RPCATOM_HPP
#define RPCATOM_HPP

#include "../drawcontext.hpp"
#include "face.hpp"
#include <QPainter>

namespace RPC
{
class Atom
{
  public:
    Atom(Face face, QString contents);

    void draw(const DrawContext &context, const QPoint &position, const Face &default_face);

    QString getContents();
    Face getFace();

  private:
    Face m_face;
    QString m_contents;
};
} // namespace RPC

#endif
