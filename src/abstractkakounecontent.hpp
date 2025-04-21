#ifndef AbstractKakouneContent_HPP
#define AbstractKakouneContent_HPP

#include "rpc/line.hpp"
#include <QtWidgets>

class AbstractKakouneContent : public QWidget
{
    Q_OBJECT

    public:
        AbstractKakouneContent(QWidget *parent = nullptr) : QWidget(parent) {}
        virtual ~AbstractKakouneContent();

        virtual void setContent(QList<RPC::Line> content) = 0;
        virtual const QList<RPC::Line> &getContent() const = 0;

        virtual void setDefaultFace(RPC::Face default_face) = 0;
        virtual const RPC::Face &getDefaultFace() const = 0;
};

#endif
