#ifndef KAKOUNECONTENTSCROLLER
#define KAKOUNECONTENTSCROLLER

#include "drawoptions.hpp"
#include "kakounelistcontent.hpp"
#include "rpc/face.hpp"
#include <QtWidgets>

class KakouneContentScroller : public QWidget
{
    Q_OBJECT

  public:
    KakouneContentScroller(DrawOptions *draw_options, RPC::Face default_face, QWidget *parent = nullptr);
    ~KakouneContentScroller();

    void setContent(QList<RPC::Line> content);
    const QList<RPC::Line> &getContent() const;

    void setDefaultFace(RPC::Face default_face);
    const RPC::Face &getDefaultFace() const;

    void setMaximumItems(int max_items);
    int getMaximumItems() const;

    void setSelectedItem(int selected_item);
    int getSelectedItem() const;

    void setSelectedFace(RPC::Face selected_face);
    RPC::Face getSelectedFace() const;

    void ensureVisible(int item_index);

  protected:
    void wheelEvent(QWheelEvent *event) override;

  private:
    void updateScrolledContent();
    void updateScrollBar();
    void scrollBarValueChanged(int value);

    QList<RPC::Line> m_content;
    KakouneListContent *m_kakoune_content;
    DrawOptions *m_draw_options;
    QScrollBar *m_scroll_bar;

    int m_max_items = 10;
    int m_scroll_offset = 0;
    int m_selected_item = -1;
};

#endif
