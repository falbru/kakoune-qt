#ifndef RPCLINE_H
#define RPCLINE_H

#include <QList>
#include <QPainter>
#include "atom.h"
#include "../drawcontext.h"

namespace RPC {
    class Line {
    public:
        Line();
        Line(QList<Atom> atoms);

        QList<Atom> getAtoms();
        void draw(const DrawContext& context, QPoint position, const Face& default_face);
    private:
        QList<Atom> m_atoms;
    };
}

#endif
