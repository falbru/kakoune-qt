#ifndef RPCLINE_H
#define RPCLINE_H

#include "../drawcontext.h"
#include "atom.h"
#include <QList>
#include <QPainter>

namespace RPC
{
class Line
{
  public:
    Line();
    Line(QList<Atom> atoms);

    QList<Atom> getAtoms();
    void draw(const DrawContext &context, QPoint position, const Face &default_face);

  private:
    QList<Atom> m_atoms;
};
} // namespace RPC

#endif
