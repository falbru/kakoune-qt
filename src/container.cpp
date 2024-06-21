#include "container.hpp"
#include "kakounewidget.hpp"
#include <qnamespace.h>
#include <qwidget.h>

Container::Container(Qt::Orientation orientation, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_splitter = new QSplitter(this);
    m_splitter->setOrientation(orientation);

    layout->addWidget(m_splitter);
}

void Container::addWidget(QWidget *widget)
{
    connectWidget(widget);
    m_splitter->addWidget(widget);
}

Qt::Orientation Container::getOrientation() const
{
    return m_splitter->orientation();
}

void Container::connectWidget(QWidget *widget)
{
    if (KakouneWidget *kak_widget = qobject_cast<KakouneWidget *>(widget))
        connectKakouneWidget(kak_widget);

    connect(widget, &QWidget::destroyed, this, [=]() {
        if (m_splitter->count() == 0)
        {
            deleteLater();
        }
        else
        {
            m_splitter->widget(0)->setFocus();
        }
    });
}

void Container::connectKakouneWidget(KakouneWidget *kak_widget)
{
    connect(kak_widget, &KakouneWidget::finished, this, [=]() {
        kak_widget->setParent(nullptr);
        kak_widget->deleteLater();
    });
}

void Container::focusInEvent(QFocusEvent *event)
{
    m_splitter->widget(0)->setFocus();
}

Container *Container::findParentContainer(QWidget *widget)
{
    QWidget *parent_container = widget->parentWidget();
    while (parent_container)
    {
        Container *container = qobject_cast<Container *>(parent_container);
        if (container)
        {
            return container;
        }
        parent_container = parent_container->parentWidget();
    }
    return nullptr;
}
