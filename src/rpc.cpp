#include "rpc.h"

namespace RPC {
    Atom deserializeAtom(QJsonObject serialized_atom) {
        Atom atom;
        atom.face = deserializeFace(serialized_atom["face"].toObject());
        atom.contents = serialized_atom["contents"].toString();
        return atom;
    }

    Line deserializeLine(QJsonArray serialized_line) {
        Line line;
        for (const QJsonValue& serialized_atom : serialized_line) {
            line.append(deserializeAtom(serialized_atom.toObject()));
        }
        return line;
    }

    Face deserializeFace(QJsonObject serialized_face) {
        Face face;
        face.fg = serialized_face["fg"].toString();
        face.bg = serialized_face["bg"].toString();
        return face;
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
}
