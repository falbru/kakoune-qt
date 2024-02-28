#include "line.hpp"

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

Line Line::slice(int start)
{
    return slice(start, contentSize());
}

Line Line::slice(int start, int end)
{
    QList<Atom> atoms;

    int content_index = 0;
    auto atom_it = m_atoms.begin();

    while (atom_it != m_atoms.end() && content_index < start)
    {
        int atom_size = atom_it->getContents().size();

        int next_index = content_index + atom_size;
        if (next_index > start)
        {
            QString substring = atom_it->getContents().mid(start - content_index);
            atoms.append(Atom(atom_it->getFace(), substring));
        }

        content_index = qMin(next_index, start);
        atom_it++;
    }

    while (atom_it != m_atoms.end() && content_index < end)
    {
        int atom_size = atom_it->getContents().size();

        int next_index = content_index + atom_size;
        if (next_index > end)
        {
            QString substring = atom_it->getContents().mid(0, end - content_index);
            atoms.append(Atom(atom_it->getFace(), substring));
        }
        else if (next_index < end)
        {
            atoms.append(*atom_it);
        }

        content_index = qMin(next_index, end);
        atom_it++;
    }
    return Line(atoms);
}

} // namespace RPC
