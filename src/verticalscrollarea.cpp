#include "verticalscrollarea.hpp"
#include <qcoreevent.h>
#include <qevent.h>
#include <qscrollbar.h>

VerticalScrollArea::VerticalScrollArea(QWidget *parent) : QScrollArea(parent)
{
   setWidgetResizable(true);
   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QSize VerticalScrollArea::minimumSizeHint() const {
	QSize MinSize( QScrollArea::minimumSizeHint() );
	const QWidget * pWidget( widget() );
	if( pWidget )
	{
		const int FrameWidth( 2 * frameWidth() );
		const int TotalWidth( pWidget->sizeHint().rwidth() + FrameWidth );
		if( TotalWidth > MinSize.width() )
			MinSize.setWidth( TotalWidth );
	}
	return MinSize;
}
