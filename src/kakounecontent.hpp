#ifndef KAKOUNECONTENT_HPP
#define KAKOUNECONTENT_HPP

#include "drawoptions.hpp"
#include "rpc/line.hpp"
#include <qtmetamacros.h>
#include <qwidget.h>
class KakouneContent : public QWidget {
    Q_OBJECT

    public:
        KakouneContent(RPC::Face default_face, DrawOptions *draw_options, QWidget *parent = nullptr);
        ~KakouneContent();

        void setLines(QList<RPC::Line> lines);

  protected:
    void paintEvent(QPaintEvent *ev) override;

    private:
        DrawOptions *m_draw_options;
        RPC::Face m_default_face;
        QList<RPC::Line> m_lines;
};

#endif
