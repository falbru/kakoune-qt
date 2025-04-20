#ifndef VERTICALSCROLLAREA_HPP
#define VERTICALSCROLLAREA_HPP

#include <qscrollarea.h>
#include <qtmetamacros.h>
class VerticalScrollArea : public QScrollArea
{
    Q_OBJECT

    public:
        VerticalScrollArea(QWidget* parent = nullptr);

        QSize minimumSizeHint() const override;
};

#endif
