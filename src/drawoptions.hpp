#ifndef DRAWOPTIONS_HPP
#define DRAWOPTIONS_HPP

#include <QObject>
#include <QPainter>
#include <QSize>

class DrawOptions : public QObject
{
    Q_OBJECT

  public:
    DrawOptions();
    ~DrawOptions();

    QFont getFont();
    QSize getCellSize();

    void setFont(const QString &font_name, int font_size);

  signals:
    void updated();

  private:
    QFont m_font;
    QSize m_cell_size;
};

#endif
