#include "line.hpp"
#include <qdebug.h>

namespace RPC
{
Line::Line() : m_atoms()
{
}

Line::Line(QList<Atom> atoms) : m_atoms(atoms)
{
}

int Line::contentSize()
{
    int content_size = 0;
    for (Atom atom : m_atoms)
    {
        content_size += atom.getContents().size();
    }
    return content_size;
}

QList<Atom> Line::getAtoms()
{
    return m_atoms;
}

void Line::draw(const DrawContext &context, QPoint position, const Face &default_face)
{
    for (Atom atom : m_atoms)
    {
        atom.draw(context, position, default_face);
        position.setX(position.x() + context.cell_size.width() * atom.getContents().size());
    }
}

Line Line::slice(int start) {
  return slice(start, contentSize());
}

Line Line::slice(int start, int end) {
  QList<Atom> atoms;

  auto it = m_atoms.begin();
  int index = 0;

  while (it != m_atoms.end() && index < start) {
    int atom_size = it->getContents().size(); 
    if (index + atom_size < start) {
      index += atom_size;
      it++;
    }else {
      if (index + atom_size > start) {
        QString substring = "";
        for (int i = start - index; i < atom_size; i++) {
          substring.append(it->getContents()[i]);
        }
        atoms.append(Atom(it->getFace(), substring));
      }
      index = start;
      it++;
    }
  }

  while(it != m_atoms.end() && index < end) {
    int atom_size = it->getContents().size(); 
    if (index + atom_size < end) {
      index += atom_size;
      atoms.append(*it);
      it++;
    }else {
      if (index + atom_size > end) {
        QString substring = "";
        for (int i = end - index; i < atom_size; i++) {
          substring.append(it->getContents()[i]);
        }
        atoms.append(Atom(it->getFace(), substring));
      }
      index = end;
    }
  }
  return Line(atoms);
}

Line Line::concat(Line a, Line b){
  return Line(a.getAtoms() + b.getAtoms());
}

} // namespace RPC
