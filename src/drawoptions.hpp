#ifndef DRAWOPTIONS_HPP
#define DRAWOPTIONS_HPP

#include <QPainter>
#include <QSize>

class DrawOptions
{
  public:
    DrawOptions();
    ~DrawOptions();

    QFont getFont();
    QSize getCellSize();

    void setFont(const QString &font_name, int font_size);

  private:
    QFont m_font;
    QSize m_cell_size;
};

#endif
