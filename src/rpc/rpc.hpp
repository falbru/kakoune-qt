#ifndef RPC_HPP
#define RPC_HPP

#include <QList>
#include <QString>
#include <qjsonarray.h>
#include <qjsonobject.h>

#include "line.hpp"

namespace RPC
{
struct Coord
{
    int line;
    int column;
};

struct DrawRequest
{
    QList<Line> lines;
    Face default_face;
    Face padding_face;
};

struct DrawStatusRequest
{
    Line status_line;
    Line mode_line;
    Face default_face;
};

struct RefreshRequest
{
    bool force;
};

Face deserializeFace(QJsonObject serialized_face);
Atom deserializeAtom(QJsonObject serialized_atom);
Line deserializeLine(QJsonArray serialized_line);
QList<Line> deserializeLines(QJsonObject serialized_lines);

DrawRequest deserializeDrawRequest(QJsonArray request_params);
DrawStatusRequest deserializeDrawStatusRequest(QJsonArray request_params);
RefreshRequest deserializeRefreshRequest(QJsonArray request_params);

QByteArray serializeRequest(const QString &method_name, QJsonArray params);
} // namespace RPC

#endif
