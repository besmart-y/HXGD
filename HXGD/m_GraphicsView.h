#pragma once
#include <qgraphicsitem.h>
#include <qpainter.h>
#include <qevent.h>
#include <qgraphicsview.h>
#include <opencv2\opencv.hpp>
#include "ToolFunction.h"
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QGraphicsSceneWheelEvent>

enum Enum_ZoomState {
	NO_STATE,
	RESET,
	ZOOM_IN,
	ZOOM_OUT
};
class m_GraphicsItem:public QGraphicsItem
{
public:
	m_GraphicsItem(QPixmap* pixmap);
	~m_GraphicsItem();
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void setQGraphicsViewWH(int nwidth, int nheight);
	void ResetItemPos();
	qreal getScaleValue() const;
protected:
	void wheelEvent(QGraphicsSceneWheelEvent* event);
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);



private:
	QPixmap mainPixmap;
	qreal       m_scaleValue;
	qreal       m_scaleDafault;
	int         m_zoomState;
	bool        m_isMove;
	QPointF     m_startPos;
public:
	int viewW;
	int viewH;
	
};

class m_GraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	m_GraphicsView(QWidget* parent=0);//int width,int height,
	~m_GraphicsView();
	QGraphicsScene* scene;
private:
	
	 m_GraphicsItem* item;

public slots:
	void slotShowImg(QImage qimg);
};




