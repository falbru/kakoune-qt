#include "line.h"

namespace RPC {
    Line::Line() : m_atoms() {

    }

    Line::Line(QList<Atom> atoms) : m_atoms(atoms) {

    }

    QList<Atom> Line::getAtoms() {
        return m_atoms;
    }

    void Line::draw(const DrawContext& context, QPoint position, const Face& default_face) {
        for (Atom atom : m_atoms) {
            atom.draw(context, position, default_face);
            position.setX(position.x() + context.cell_size.width() * atom.getContents().size());
        }
    }
}
