#include "splitcontainer.hpp"

SplitContainer::SplitContainer(Qt::Orientation orientation, QWidget *parent) : Container(orientation, parent)
{
}

void SplitContainer::split(QWidget *source_widget, QWidget *new_widget, Qt::Orientation orientation)
{
    int index = m_splitter->indexOf(source_widget);

    SplitContainer *container = new SplitContainer(orientation, this);
    m_splitter->replaceWidget(
        index,
        container); // NOTE: The container widget has to be added to the splitter before the source_widget is added to
                    // it. Or else the source widget will be removed from the splitter so that there is only one widget
                    // left, and then the splitter ratio won't be preserved when the container is added to the splitter.

    container->addWidget(source_widget);
    container->addWidget(new_widget);
    container->m_splitter->setSizes(QList<int>({INT_MAX, INT_MAX}));

    connectWidget(container);
}
