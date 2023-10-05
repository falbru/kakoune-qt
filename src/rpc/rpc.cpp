#include "rpc.h"
#include <qjsondocument.h>

namespace RPC {
    Atom deserializeAtom(QJsonObject atom_serialized) {
        return Atom{
            deserializeFace(atom_serialized["face"].toObject()),
            atom_serialized["contents"].toString()
        };
    }

    Line deserializeLine(QJsonArray line_serialized) {
        QList<Atom> atoms;
        for (const QJsonValue& atom_serialized : line_serialized) {
            atoms.append(deserializeAtom(atom_serialized.toObject()));
        }
        return Line(atoms);
    }

    Face deserializeFace(QJsonObject face_serialized) {
        return Face{
            face_serialized["fg"].toString(),
            face_serialized["bg"].toString(),
            QList<Attribute>()
        };
    }

    DrawRequest deserializeDrawRequest(QJsonArray request_params) {
        QJsonArray lines_serialized = request_params.at(0).toArray();
        QJsonObject default_face_serialized = request_params.at(1).toObject();
        QJsonObject padding_face_serialized = request_params.at(2).toObject();

        QList<Line> lines;
        for (const QJsonValue& line_serialized : lines_serialized) {
            lines.append(deserializeLine(line_serialized.toArray()));
        }

        return DrawRequest {
            lines,
            deserializeFace(default_face_serialized),
            deserializeFace(padding_face_serialized)
        };
    }

    DrawStatusRequest deserializeDrawStatusRequest(QJsonArray request_params) {
        QJsonArray status_line_serialized = request_params.at(0).toArray();
        QJsonArray mode_line_serialized = request_params.at(1).toArray();
        QJsonObject default_face_serialized = request_params.at(2).toObject();

        return DrawStatusRequest{
            deserializeLine(status_line_serialized),
            deserializeLine(mode_line_serialized),
            deserializeFace(default_face_serialized)
        };
    }

    RefreshRequest deserializeRefreshRequest(QJsonArray request_params) {
        return RefreshRequest{
            request_params.at(0).toBool()
        };
    }

    QByteArray serializeRequest(const QString& method_name, QJsonArray params) {
        QJsonObject req{
            {{"jsonrpc", "2.0"}, {"method", method_name}, {"params", params}}
        };

        return QJsonDocument(req).toJson(QJsonDocument::Compact);
    }
}
