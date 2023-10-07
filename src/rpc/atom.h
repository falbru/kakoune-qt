#ifndef RPCATOM_H
#define RPCATOM_H

#include "../drawcontext.h"
#include "face.h"
#include <QPainter>

namespace RPC
{
class Atom
{
  public:
    Atom(Face face, QString contents);

    void draw(const DrawContext &context, const QPoint &position, const Face &default_face);

    QString getContents();

  private:
    Face m_face;
    QString m_contents;
};
} // namespace RPC

#endif
