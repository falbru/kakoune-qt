#ifndef RPC_H
#define RPC_H

#include <QString>
#include <QList>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "line.h"

namespace RPC
{
    struct Coord {
        int line;
        int column;
    };

    struct DrawRequest {
        QList<Line> lines;
        Face default_face;
        Face padding_face;
    };

    struct DrawStatusRequest {
        Line status_line;
        Line mode_line;
        Face default_face;
    };

    struct RefreshRequest {
        bool force;
    };

    Atom deserializeAtom(QJsonObject serialized_atom);
    Line deserializeLine(QJsonArray serialized_line);
    Face deserializeFace(QJsonObject serialized_face);

    DrawRequest deserializeDrawRequest(QJsonArray request_params);
    DrawStatusRequest deserializeDrawStatusRequest(QJsonArray request_params);
    RefreshRequest deserializeRefreshRequest(QJsonArray request_params);

    QByteArray serializeRequest(const QString& method_name, QJsonArray params);
}

#endif
