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

enum class MenuStyle
{
    PROMPT,
    INLINE
};

struct MenuShowRequest
{
    QList<Line> items;
    Coord anchor;
    Face selected_item_face;
    Face menu_face;
    MenuStyle style;
};

struct MenuSelectRequest
{
    int selected;
};

enum class InfoStyle
{
    PROMPT,
    INLINE,
    INLINE_ABOVE,
    INLINE_BELOW,
    MENU_DOC,
    MODAL
};

struct InfoShowRequest
{
    Line title;
    QList<Line> content;
    Coord anchor;
    Face face;
    InfoStyle style;
};

struct RefreshRequest
{
    bool force;
};

Coord deserializeCoord(QJsonObject coord_serialized);
Face deserializeFace(QJsonObject face_serialized);
Atom deserializeAtom(QJsonObject atom_serialized);
Line deserializeLine(QJsonArray line_serialized);
QList<Line> deserializeLines(QJsonObject lines_serialized);
InfoStyle convertToInfoStyle(QString style);

DrawRequest deserializeDrawRequest(QJsonArray request_params);
DrawStatusRequest deserializeDrawStatusRequest(QJsonArray request_params);
MenuShowRequest deserializeMenuShowRequest(QJsonArray request_params);
MenuSelectRequest deserializeMenuSelectRequest(QJsonArray request_params);
InfoShowRequest deserializeInfoShowRequest(QJsonArray request_params);
RefreshRequest deserializeRefreshRequest(QJsonArray request_params);

QByteArray serializeRequest(const QString &method_name, QJsonArray params);
} // namespace RPC

#endif
