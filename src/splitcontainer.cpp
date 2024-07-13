#include "splitcontainer.hpp"

SplitContainer::SplitContainer(Qt::Orientation orientation, QWidget *parent) : Container(orientation, parent)
{
}

void SplitContainer::split(QWidget *source_widget, QWidget *new_widget, Qt::Orientation orientation)
{
    disconnectWidget(source_widget);

    int index = m_splitter->indexOf(source_widget);

    SplitContainer *container = new SplitContainer(orientation, this);

    // NOTE: replaceWidget() will move focus to another widget, which will screw up the focus order in
    // LastFocusedFilter. We therefore clear the focus before calling the function, and reapply the focus after.
    QWidget *regain_focus_widget = nullptr;
    if (source_widget->isAncestorOf(focusWidget()))
    {
        regain_focus_widget = focusWidget();
        regain_focus_widget->clearFocus();
    }

    m_splitter->replaceWidget(index, container);

    if (regain_focus_widget)
        regain_focus_widget->setFocus();

    // NOTE: Adding the source_widget to the container after calling replaceWidget() will ensure that the splitter ratio
    // will be preserved.
    container->addWidget(source_widget);
    container->addWidget(new_widget);
    container->m_splitter->setSizes(QList<int>({INT_MAX, INT_MAX}));

    connectWidget(container);
}
