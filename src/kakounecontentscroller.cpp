#include "kakounecontent.hpp"
#include "kakounecontentscroller.hpp"
#include <qscrollbar.h>

KakouneContentScroller::KakouneContentScroller(DrawOptions *draw_options,
                                               RPC::Face default_face,
                                               QWidget *parent)
    : QWidget(parent), m_draw_options(draw_options) {
  m_kakoune_content = new KakouneContent(draw_options, default_face);
  m_scroll_bar = new QScrollBar();
  connect(m_scroll_bar, &QScrollBar::valueChanged, this, &KakouneContentScroller::scrollBarValueChanged);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_kakoune_content);
  layout->addWidget(m_scroll_bar);

  setSizePolicy(m_kakoune_content->sizePolicy());
}

KakouneContentScroller::~KakouneContentScroller() {}

void KakouneContentScroller::setContent(QList<RPC::Line> content) {
  m_content = content;
  m_scroll_offset = 0;

  updateScrollBar();
  updateScrolledContent();
}

const QList<RPC::Line> &KakouneContentScroller::getContent() const {
  return m_kakoune_content->getContent();
}

void KakouneContentScroller::setDefaultFace(RPC::Face default_face) {
  m_kakoune_content->setDefaultFace(default_face);
}

const RPC::Face &KakouneContentScroller::getDefaultFace() const {
  return m_kakoune_content->getDefaultFace();
}

void KakouneContentScroller::setMaximumItems(int max_items) {
  m_max_items = max_items;
  updateScrollBar();
}

int KakouneContentScroller::getMaximumItems() const { return m_max_items; }

void KakouneContentScroller::ensureVisible(int item_index) {
  if (item_index >= m_scroll_offset + m_max_items) {
    m_scroll_offset = item_index - m_max_items + 1;
  } else if (item_index < m_scroll_offset) {
    m_scroll_offset = item_index;
  }

  updateScrollBar();
  updateScrolledContent();
}

void KakouneContentScroller::updateScrolledContent() {
  m_scroll_offset = qMax(m_scroll_offset, 0);
  m_scroll_offset = qMin(m_scroll_offset, m_content.size() - m_max_items);

  m_kakoune_content->setContent(
      m_content.size() > m_max_items ?
          m_content.sliced(m_scroll_offset, qMin(m_max_items, m_content.size()))
          : m_content);
  updateGeometry();
  parentWidget()->adjustSize();
}

void KakouneContentScroller::updateScrollBar() {
  m_scroll_offset = qMax(m_scroll_offset, 0);
  m_scroll_offset = qMin(m_scroll_offset, m_content.size() - m_max_items);

  m_scroll_bar->setValue(m_scroll_offset);
  m_scroll_bar->setMinimum(0);
  m_scroll_bar->setMaximum(m_kakoune_content->getContent().size());
  m_scroll_bar->setPageStep(m_max_items);
  m_scroll_bar->setSingleStep(1);
}

void KakouneContentScroller::scrollBarValueChanged(int value) {
    m_scroll_offset = value;

    updateScrollBar();
    updateScrolledContent();
}

void KakouneContentScroller::wheelEvent(QWheelEvent *ev) {
    static int scroll = 0;
    static constexpr int scroll_sensitivity = 120;
    static constexpr int scroll_threshold = 4;

    const QPoint num_degrees = ev->angleDelta() / 8;

    if (!num_degrees.isNull())
    {
        const int scroll_amount = num_degrees.y() * 120 / scroll_sensitivity;
        scroll += scroll_amount;

        if (std::abs(scroll) >= scroll_threshold)
        {
            m_scroll_offset += -scroll/scroll_threshold;
            updateScrollBar();
            updateScrolledContent();
            scroll %= scroll_threshold;
        }
    }
    else
    {
        scroll = 0;
    }
}
