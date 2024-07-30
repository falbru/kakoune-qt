#ifndef COLORPALETTE_HPP
#define COLORPALETTE_HPP

#include <QColor>
#include <QString>

class ColorPalette
{
  public:
    ColorPalette();

    void setFg(QColor color);
    void setBg(QColor color);
    void setBlack(QColor color);
    void setRed(QColor color);
    void setGreen(QColor color);
    void setYellow(QColor color);
    void setBlue(QColor color);
    void setMagenta(QColor color);
    void setCyan(QColor color);
    void setWhite(QColor color);

    QColor getFg() const;
    QColor getBg() const;
    QColor getBlack() const;
    QColor getRed() const;
    QColor getGreen() const;
    QColor getYellow() const;
    QColor getBlue() const;
    QColor getMagenta() const;
    QColor getCyan() const;
    QColor getWhite() const;

  private:
    QColor m_fg;
    QColor m_bg;
    QColor m_black;
    QColor m_red;
    QColor m_green;
    QColor m_yellow;
    QColor m_blue;
    QColor m_magenta;
    QColor m_cyan;
    QColor m_white;
};

#endif
