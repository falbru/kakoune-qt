#include "rpc.h"
#include <qjsondocument.h>

namespace RPC
{
Face deserializeFace(QJsonObject face_serialized)
{
    return Face{face_serialized["fg"].toString(), face_serialized["bg"].toString(), QList<Attribute>()};
}

Atom deserializeAtom(QJsonObject atom_serialized)
{
    return Atom{deserializeFace(atom_serialized["face"].toObject()), atom_serialized["contents"].toString()};
}

Line deserializeLine(QJsonArray line_serialized)
{
    QList<Atom> atoms;
    for (const QJsonValue &atom_serialized : line_serialized)
    {
        atoms.append(deserializeAtom(atom_serialized.toObject()));
    }
    return Line(atoms);
}

QList<Line> deserializeLines(QJsonArray line_serialized)
{
    QList<Line> lines;
    for (const QJsonValue &atom_serialized : line_serialized)
    {
        lines.append(deserializeLine(atom_serialized.toArray()));
    }
    return lines;
}

DrawRequest deserializeDrawRequest(QJsonArray request_params)
{
    return DrawRequest{deserializeLines(request_params.at(0).toArray()),
                       deserializeFace(request_params.at(1).toObject()),
                       deserializeFace(request_params.at(2).toObject())};
}

DrawStatusRequest deserializeDrawStatusRequest(QJsonArray request_params)
{
    return DrawStatusRequest{deserializeLine(request_params.at(0).toArray()),
                             deserializeLine(request_params.at(1).toArray()),
                             deserializeFace(request_params.at(2).toObject())};
}

RefreshRequest deserializeRefreshRequest(QJsonArray request_params)
{
    return RefreshRequest{request_params.at(0).toBool()};
}

QByteArray serializeRequest(const QString &method_name, QJsonArray params)
{
    QJsonObject req{{{"jsonrpc", "2.0"}, {"method", method_name}, {"params", params}}};

    return QJsonDocument(req).toJson(QJsonDocument::Compact);
}
} // namespace RPC
