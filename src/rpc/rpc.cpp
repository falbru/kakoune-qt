#include "rpc.h"

namespace RPC {
    Atom deserializeAtom(QJsonObject serialized_atom) {
        return Atom{
            deserializeFace(serialized_atom["face"].toObject()),
            serialized_atom["contents"].toString()
        };
    }

    Line deserializeLine(QJsonArray serialized_line) {
        QList<Atom> atoms;
        for (const QJsonValue& serialized_atom : serialized_line) {
            atoms.append(deserializeAtom(serialized_atom.toObject()));
        }
        return Line(atoms);
    }

    Face deserializeFace(QJsonObject serialized_face) {
        return Face{
            serialized_face["fg"].toString(),
            serialized_face["bg"].toString(),
            QList<Attribute>()
        };
    }

    DrawRequest deserializeDrawRequest(QJsonArray request_params) {
        QJsonArray serialized_lines = request_params.at(0).toArray();
        QJsonObject serialized_default_face = request_params.at(1).toObject();
        QJsonObject serialized_padding_face = request_params.at(2).toObject();

        DrawRequest request;
        for (const QJsonValue& serialized_line : serialized_lines) {
            request.lines.append(deserializeLine(serialized_line.toArray()));
        }

        request.default_face = deserializeFace(serialized_default_face);
        request.padding_face = deserializeFace(serialized_padding_face);
        return request;
    }

    DrawStatusRequest deserializeDrawStatusRequest(QJsonArray request_params) {
        QJsonArray serialized_status_line = request_params.at(0).toArray();
        QJsonArray serialized_mode_line = request_params.at(1).toArray();
        QJsonObject serialized_default_face = request_params.at(2).toObject();

        DrawStatusRequest request;
        request.status_line = deserializeLine(serialized_status_line);
        request.mode_line = deserializeLine(serialized_mode_line);
        request.default_face = deserializeFace(serialized_default_face);
        return request;
    }
}
