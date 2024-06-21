#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "kakounewidget.hpp"
#include <qnamespace.h>
#include <qtmetamacros.h>
#include <qvariant.h>
#include <qwidget.h>

class Container : public QWidget
{
    Q_OBJECT

  public:
    Container(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr);

    void addWidget(QWidget *widget);

    Qt::Orientation getOrientation() const;

    static Container *findParentContainer(QWidget *widget);

  protected:
    void connectWidget(QWidget *widget);
    void connectKakouneWidget(KakouneWidget *kak_widget);

    void focusInEvent(QFocusEvent *event) override;

    QSplitter *m_splitter;
};

#endif
