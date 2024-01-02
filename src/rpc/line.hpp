#ifndef RPCLINE_HPP
#define RPCLINE_HPP

#include "../drawcontext.hpp"
#include "atom.hpp"
#include <QList>
#include <QPainter>

namespace RPC
{
class Line
{
  public:
    Line();
    Line(QList<Atom> atoms);

    int contentSize();

    QList<Atom> getAtoms();
    void draw(const DrawContext &context, QPoint position, const Face &default_face);

  private:
    QList<Atom> m_atoms;
};
} // namespace RPC

#endif
