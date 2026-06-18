#ifndef KAKOUNECONTENT_HPP
#define KAKOUNECONTENT_HPP

#include "drawoptions.hpp"
#include "rpc/line.hpp"
#include <QtWidgets>

class KakouneContent : public QWidget
{
    Q_OBJECT

  public:
    KakouneContent(DrawOptions *draw_options, RPC::Face default_face, QWidget *parent = nullptr);
    ~KakouneContent();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int width) const override;

    void setContent(QList<RPC::Line> content);
    const QList<RPC::Line> &getContent() const;

    void setDefaultFace(RPC::Face default_face);
    const RPC::Face &getDefaultFace() const;

    void setPreferredSize(QSize preferred_size);
    const QSize &getPreferredSize() const;

  protected:
    void paintEvent(QPaintEvent *ev) override;

    QList<RPC::Line> m_content;
    DrawOptions *m_draw_options;
    RPC::Face m_default_face;

    QSize m_preferred_size;
};

#endif
