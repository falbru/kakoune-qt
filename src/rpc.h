#ifndef RPC_H
#define RPC_H

#include <QString>
#include <QList>
#include <qjsonobject.h>
#include <qjsonarray.h>

namespace RPC
{
    typedef QString Color;

    enum Attribute {
        underline,
        reverse,
        blink,
        bold,
        dim,
        italic,
        final_fg,
        final_bg,
        final_attr
    };

    struct Face {
        Color fg;
        Color bg;
        QList<Attribute> attributes;
    };

    struct Atom {
        Face face;
        QString contents;
    };

    typedef QList<Atom> Line;

    struct Coord {
        int line;
        int column;
    };

    struct DrawRequest {
        QList<Line> lines;
        Face default_face;
        Face padding_face;
    };

    Atom deserializeAtom(QJsonObject serialized_atom);
    Line deserializeLine(QJsonArray serialized_line);
    Face deserializeFace(QJsonObject serialized_face);

    DrawRequest deserializeDrawRequest(QJsonArray request_params);
}

#endif
