#include "m_GraphicsView.h"
#include <qdebug.h>
m_GraphicsItem::m_GraphicsItem(QPixmap* pixmap)
{
	mainPixmap = *pixmap;
	setAcceptDrops(true);
	m_scaleValue = 0;
	m_scaleDafault = 0;
	m_isMove = false;
}

m_GraphicsItem::~m_GraphicsItem()
{

}

QRectF m_GraphicsItem::boundingRect() const
{
	int w = mainPixmap.width();
	return QRectF(-mainPixmap.width()/2,-mainPixmap.height()/2,mainPixmap.width(),mainPixmap.height());
}

void m_GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawPixmap(-mainPixmap.width() / 2, -mainPixmap.height() / 2, mainPixmap);
}

void m_GraphicsItem::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if ((event->delta() > 0) && (m_scaleValue >= 50))//最大放大到原始图像的50倍
	{
		return;
	}
	else if ((event->delta() < 0) && (m_scaleValue <= m_scaleDafault))//图像缩小到自适应大小之后就不继续缩小
	{
		ResetItemPos();//重置图片大小和位置，使之自适应控件窗口大小
	}
	else
	{
		qreal qrealOriginScale = m_scaleValue;
		if (event->delta() > 0)//鼠标滚轮向前滚动
		{
			m_scaleValue *= 1.1;//每次放大10%
		}
		else
		{
			m_scaleValue *= 0.9;//每次缩小10%
		}
		setScale(m_scaleValue);
		QPointF pt;
		if (event->delta() > 0)
		{
			pt = QPointF(-event->pos().x() * qrealOriginScale * 0.1, -event->pos().y() * qrealOriginScale * 0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
		}
		else
		{
			pt = QPointF(event->pos().x() * qrealOriginScale * 0.1, event->pos().y() * qrealOriginScale * 0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
		}
		moveBy(pt.x(), pt.y());
	}
	//scene.setSceneRect(scene.itemsBoundingRect());
	
}

void m_GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
		m_isMove = true;//标记鼠标左键被按下
	}
	else if (event->button() == Qt::RightButton)
	{
		ResetItemPos();//右击鼠标重置大小
	}
	
}

void m_GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_isMove)
	{
		QPointF point = (event->pos() - m_startPos) * m_scaleValue;
		moveBy(point.x(), point.y());
	}
}

void m_GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	m_isMove = false;//标记鼠标左键已经抬起
}

void m_GraphicsItem::setQGraphicsViewWH(int nwidth, int nheight)//将主界面的控件QGraphicsView的width和height传进本类中，并根据图像的长宽和控件的长宽的比例来使图片缩放到适合控件的大小
{
	int nImgWidth = mainPixmap.width();
	int nImgHeight = mainPixmap.height();
	qreal temp1 = nwidth * 1.0 / nImgWidth;
	qreal temp2 = nheight * 1.0 / nImgHeight;
	if (temp1 > temp2)
	{
		m_scaleDafault = temp2;
	}
	else
	{
		m_scaleDafault = temp1;
	}
	setScale(m_scaleDafault);
	m_scaleValue = m_scaleDafault;
}

void m_GraphicsItem::ResetItemPos()//重置图片位置
{
	m_scaleValue = m_scaleDafault;//缩放比例回到一开始的自适应比例
	setScale(m_scaleDafault);//缩放到一开始的自适应大小
	setPos(0, 0);
}

qreal m_GraphicsItem::getScaleValue() const
{
	return m_scaleValue;
}




//**********************graphicsview******************/
m_GraphicsView::m_GraphicsView(QWidget* parent):QGraphicsView(parent),scene(nullptr)//
{
	//this->resize(540, 240);
	scene = new QGraphicsScene(this);
	
	int width = this->width();
	int height = this->height();
	scene->setSceneRect(-width / 2, -height / 2, width, height);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setScene(scene);
}

m_GraphicsView::~m_GraphicsView()
{
}

void m_GraphicsView::slotShowImg(QImage qimg)
{
	//QImage qimg = ToolClass::MatToQImage(mat);
	if (qimg.isNull())
	{
		qDebug() << "NUll";
	}
	scene->clear();
	QPixmap pixmap = QPixmap::fromImage(qimg);
	item = new m_GraphicsItem(&pixmap);
	item->setQGraphicsViewWH(this->width(), this->height());
	scene->addItem(item);
	update();
}
