#pragma once
#include <QGraphicsItem>
#include <qpainter.h>
#include <QMouseEvent>
#include <qgraphicssceneevent.h>
#include <qgraphicsitem.h>
#include <qpoint.h>
#include <qdrag.h>
#include <QGraphicsScene>
#include <QThread>
//basical image widget,it contains a scene to additem(self)
//dragging function has some problems still
class ImageWidget : public QGraphicsObject

{
	Q_OBJECT

public:

	ImageWidget();
	~ImageWidget();

	QRectF boundingRect() const;
	//***************Zoom In/Out***************
	void wheelEvent(QGraphicsSceneWheelEvent *event);
	void ResetItemPos();
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	qreal getScaleValue() const;
	qreal getScaleDafault() const;
	//***************Defects***************NOT USING in this project
	void loadDefectRand();
	void loadDefect(QList<QRectF>&defects);
	void setDefectsVisible(bool);
	void drawLine(qreal x1, qreal y1, double k);
	QList<QGraphicsItem*>				defectItems;
	//***************Image***************
	void								reloadImage(QPixmap *pixmap, double pw, double ph,QPointF p1 = QPointF(0.,0.), QPointF p2 = QPointF(0., 0.));
	bool								isLoaded();
	QGraphicsScene						scene;
	QPointF								transLTOrigin(QPointF);
	void								clearImage();
private:
	//***************Not using***************
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setQGraphicsViewWH(int nwidth, int nheight);
	bool m_isDrawRect;
	//***************Zoom In/Out***************
	qreal	m_scaleValue = 0;
	qreal	m_scaleDafault = 0;
	//***************Image***************
	QPixmap m_pix;
	bool m_isMove;
	QPointF m_startPos;
	QPointF m_point1;
	QPointF m_point2;
	//double	mainW, mainH;
signals:

public slots:
	void	sltResize(double, double);

};
