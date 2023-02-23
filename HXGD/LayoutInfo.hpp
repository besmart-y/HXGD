#pragma once

#include <qwidget.h>
#include <qpainter.h>



class Layout:public QWidget
{
	Q_OBJECT
public:
	Layout::Layout(QWidget* parent) : QWidget(parent){
		bg_color = QColor(0, 102, 0);
		bgColorStart = QColor(0, 255, 0);
		bgColorEnd = QColor(0, 20, 0);
	}

	Layout::~Layout(){
	}
	void DrawBackGround(QPainter *painter) {
		painter->save();
		painter->setPen(bg_color);
		/*QLinearGradient bgGradient(QPointF(0, 0), QPointF(0, height()));
		bgGradient.setColorAt(0.0, bgColorStart);
		bgGradient.setColorAt(1.0, bgColorEnd);*/
		painter->setBrush(QBrush(bg_color));
		painter->drawRect(rect().x()+10,rect().y()+10, rect().width()-20, rect().height()-20);
		painter->restore();
	}
protected:
	void paintEvent(QPaintEvent*){
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		DrawBackGround(&painter);
	}
private:
	QColor bg_color;
	QColor bgColorStart;
	QColor bgColorEnd;

};

