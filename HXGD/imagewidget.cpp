#include "ImageWidget.h"

ImageWidget::ImageWidget()
{

}

ImageWidget::~ImageWidget()
{
}
void ImageWidget::reloadImage(QPixmap *pixmap, double pw, double ph, QPointF p1 , QPointF p2)
{
	m_pix = *pixmap;
	m_point1 = p1;
	m_point2 = p2;
	setAcceptDrops(true);
	m_isMove = false;
	m_isDrawRect = false;
	double m_rate;
	double img_width = m_pix.width();
	double img_height = m_pix.height();

	if (pw / img_width > ph / img_height) m_rate = ph / img_height;
	else m_rate = pw / img_width;

	m_rate *= 0.99;
	setScale(m_rate);
	m_scaleValue = m_rate;
	m_scaleDafault = m_rate;
	ResetItemPos();
}
void ImageWidget::loadDefectRand()
{
	int a = rand() % 8;
	QRectF rec(-90, -80, (a + 1) * 10, a * 10);
	QGraphicsItem *item = this->scene.addRect(rec, QPen(Qt::red, 2));
	item->setParentItem(this);
	defectItems.append(item);

}
void ImageWidget::loadDefect(QList<QRectF> & defects)
{
	defectItems.clear();
	for (int i = 0; i < defects.count(); ++i)
	{
		QGraphicsItem *item = scene.addRect(defects[i], QPen(Qt::red, 2));
		item->setParentItem(this);
		defectItems.append(item);
	}
}
void ImageWidget::setDefectsVisible(bool isVisible)
{
	for (int i = 0; i < defectItems.count(); ++i)
	{
		defectItems[i]->setVisible(isVisible);
	}
}

void ImageWidget::drawLine(qreal x1, qreal y1, double k)
{
	this->scene.addLine(QLineF(x1,y1,x1+10000,y1+10000*k));
}

QRectF ImageWidget::boundingRect() const
{
	return QRectF(-m_pix.width() / 2, -m_pix.height() / 2,
		m_pix.width(), m_pix.height());
}
QPointF ImageWidget::transLTOrigin(QPointF pt)
{
	return QPointF(pt.x() + m_pix.width() / 2, pt.y() + m_pix.height() / 2);
}
void ImageWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->drawPixmap(-m_pix.width() / 2, -m_pix.height() / 2, m_pix);
	/*if (m_point1 == QPointF(0.,0.) && m_point2 == QPointF(0., 0.))
	{
	}
	else
	{
		painter->setPen(QPen(Qt::red, 1,Qt::SolidLine));
		painter->drawLine(QLineF(m_point1,m_point2));
	}*/
}
void ImageWidget::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	if ((event->delta() > 0) && (m_scaleValue >= 50))
	{
		return;
	}
	else if ((event->delta() < 0) && (m_scaleValue < m_scaleDafault))
	{
		ResetItemPos();
	}
	else
	{
		qreal qrealOriginScale = m_scaleValue;
		if (event->delta() > 0)
		{
			m_scaleValue *= 1.1;
		}
		else
		{
			m_scaleValue *= 0.9;
		}
		setScale(m_scaleValue);
		QPointF pt;
		if (event->delta() > 0)
		{
			pt = QPointF(-event->pos().x()*qrealOriginScale*0.1, -event->pos().y()*qrealOriginScale*0.1);

		}
		else
		{
			pt = QPointF(event->pos().x()*qrealOriginScale*0.1, event->pos().y()*qrealOriginScale*0.1);

		}

		moveBy(pt.x(), pt.y());
	}
	scene.setSceneRect(scene.itemsBoundingRect());
}
void ImageWidget::ResetItemPos()
{

	m_scaleValue = m_scaleDafault;
	setScale(m_scaleDafault);
	setPos(0, 0);
	scene.setSceneRect(scene.itemsBoundingRect());
}
void ImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_isDrawRect)
		{
			m_startPos = event->pos();
			m_isMove = true;
		}
		else
		{
			m_startPos = event->pos();
			m_isMove = true;
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		ResetItemPos();
	}
}

void ImageWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isMove && !m_isDrawRect)
	{
		if (m_scaleValue > m_scaleDafault)
		{
			QPointF point = (event->pos() - m_startPos)*m_scaleValue;
			moveBy(point.x(), point.y());
		}
	}
	if (m_isDrawRect&&m_isMove)
	{

		QLineF line(event->pos(), m_startPos);
	}
}
void ImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	m_isMove = false;
	m_isDrawRect = false;
}
qreal ImageWidget::getScaleValue() const
{
	return m_scaleValue;
}
qreal ImageWidget::getScaleDafault() const
{
	return m_scaleDafault;
}
void ImageWidget::setQGraphicsViewWH(int nwidth, int nheight)
{
	int nImgWidth = m_pix.width();
	int nImgHeight = m_pix.height();
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
void	ImageWidget::sltResize(double pw, double ph)
{
	double m_rate;
	double img_width = m_pix.width();
	double img_height = m_pix.height();

	if (pw / m_pix.width() > ph / m_pix.height()) m_rate = ph / m_pix.height();
	else m_rate = pw / m_pix.width();
	m_rate *= 0.98;
	setScale(m_rate);
	m_scaleValue = m_rate;
	m_scaleDafault = m_rate;
	ResetItemPos();
}
bool	ImageWidget::isLoaded()
{
	if (m_pix.data_ptr() != nullptr)
	{
		return true;
	}
	else
		return false;
}
void	ImageWidget::clearImage()
{
	QPixmap map(0, 0);
	m_pix = map;
}
