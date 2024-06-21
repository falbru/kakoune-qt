#ifndef SPLITCONTAINER_HPP
#define SPLITCONTAINER_HPP

#include "container.hpp"
#include <qnamespace.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class SplitContainer : public Container
{
    Q_OBJECT

  public:
    SplitContainer(Qt::Orientation orientation, QWidget *parent = nullptr);

    void split(QWidget *source_widget, QWidget *new_widget, Qt::Orientation orientation = Qt::Horizontal);
};

#endif
