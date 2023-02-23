#pragma once
//自定义Layout面板
#include <QWidget>
#include <QPainter>
#include <QTimer>

struct PanelInfo
{
	double x;
	double y;
	double width;
	double height;
	QString str;
	PanelInfo()
	{
		x = 0.;
		y = 0.;
		width = 0.;
		height = 0.;
		str = "";
	}
	PanelInfo(double ix, double iy,
		double iwidth, double iheight,
		QString istr) {
		x = ix;
		y = iy;
		width = iwidth;
		height = iheight;
		str = istr;
	}
};

enum StartOrPOS
{
	START_LT = 0,
	START_RT,
	START_LD,
	START_RD
};

class LayoutBar :public QWidget
{
	Q_OBJECT
		Q_PROPERTY(double XValue READ getXValue WRITE setXValue)
		Q_PROPERTY(double YValue READ getYValue WRITE setYValue)
		Q_PROPERTY(int StartPos READ getStartPos WRITE setStartPos)
		Q_PROPERTY(int precision READ getPrecision WRITE setPrecision)
		Q_PROPERTY(double xprecision READ getXPrecision WRITE setXPrecision)
		Q_PROPERTY(double yprecision READ getYPrecision WRITE setYPrecision)

		Q_PROPERTY(int longStep READ getLongStep WRITE setLongStep)
		Q_PROPERTY(int shortStep READ getShortStep WRITE setShortStep)
		Q_PROPERTY(int space READ getSpace WRITE setSpace)
		Q_PROPERTY(bool bLeft READ getbLeft WRITE setbLeft)

		Q_PROPERTY(QColor bgColorStart READ getBgColorStart WRITE setBgColorStart)
		Q_PROPERTY(QColor bgColorEnd READ getBgColorEnd WRITE setBgColorEnd)
		Q_PROPERTY(QColor lineColor READ getLineColor WRITE setLineColor)
		Q_PROPERTY(QColor barBgColor READ getBarBgColor WRITE setBarBgColor)
		Q_PROPERTY(QColor barColor READ getBarColor WRITE setBarColor)
public:
	LayoutBar::LayoutBar(QWidget *parent) : QWidget(parent)
	{
		StartPos = START_RD;
		XValue = 2600;
		//XmaxValue = 1300;
		YValue = 2600;
		//YmaxValue = 1300;

		precision = 1;
		xprecision = 0.;
		yprecision = 0.;
		longStep = 150;
		shortStep = 50;
		space = 20;
		spaceY = 100;
		bgColorStart = QColor(100, 100, 100);
		bgColorEnd = QColor(60, 60, 60);
		lineColor = QColor(0, 0, 0);
		barBgColor = QColor(85, 170, 255);//QColor(255, 255, 0);
		barColor = QColor(0, 255, 0);
		defBarColor = QColor(255, 0, 0);
		orAngleColor = QColor(85, 170, 255);//QColor(255, 255, 0);
		bLeft = false;
		setFont(QFont("Arial", 8));
	}

	~LayoutBar() {
	};



protected:
	void paintEvent(QPaintEvent *) {

		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

		//绘制背景
		drawBg(&painter);
		//绘制标尺
		drawRuler(&painter);
		//绘制大板背景
		drawBarBg(&painter);
		//绘制定向角
		drawOrangle(&painter);
		//绘制小板前景
		drawBar(&painter);
		//绘制方向尺
		drawLeftRightBar(&painter);
	};
	void drawBg(QPainter *painter) {
		//绘制背景
		painter->save();
		painter->setPen(Qt::NoPen);
		QLinearGradient bgGradient(QPointF(0, 0), QPointF(0, height()));
		bgGradient.setColorAt(0.0, bgColorStart);
		bgGradient.setColorAt(1.0, bgColorEnd);
		painter->setBrush(bgGradient);
		painter->drawRect(rect());
		painter->restore();
	};
	void drawRuler(QPainter *painter) {
		painter->save();
		painter->setPen(lineColor);
		painter->setFont(QFont("Arial", 7));
		//绘制纵向标尺线 20的长度为刻度尺文字的宽度
		double initX = space + 20;
		double initY = space + 40;
		QPointF topPot(initX, initY);
		QPointF bottomPot(initX, height() - spaceY);
		painter->drawLine(topPot, bottomPot);

		//绘制纵向标尺刻度
		double length = bottomPot.y() - topPot.y();
		//计算每一格移动多少
		double increment = length / YValue;
		setYPrecision(increment);
		//长线条短线条长度
		int longLineLen = 15;
		int shortLineLen = 5;

		//根据范围值绘制刻度值及刻度值
		for (int i = 0; i < 96; i++)
		{
			if (i % 4 == 0)
			{
				QPointF leftPot(initX + longLineLen, initY);
				QPointF rightPot(initX, initY);
				painter->drawLine(leftPot, rightPot);
				QString strValue;
				switch (StartPos)
				{
				case START_LT:
				case START_RT:
					strValue = QString("%1").arg((double)(YValue) / 96 * i, 0, 'f', precision);
					break;
				case START_LD:
				case START_RD:
					strValue = QString("%1").arg((double)(YValue) / 96 * (96 - i), 0, 'f', precision);
					break;
				default:
					break;
				}

				double fontWidth = painter->fontMetrics().width(strValue);
				double fontHeight = painter->fontMetrics().height();
				QPointF textPot(initX - fontWidth - 5, initY + fontHeight / 3);
				painter->drawText(textPot, strValue);

			}
			else {
				if (i % (longStep / 2) == 0) {
					shortLineLen = 7;
				}
				else {
					shortLineLen = 4;
				}

				QPointF leftPot(initX + shortLineLen, initY);
				QPointF rightPot(initX, initY);
				painter->drawLine(leftPot, rightPot);
			}
			initY += length / 96;
		}
		painter->drawLine(QPointF(initX + longLineLen, initY), QPointF(initX, initY));
		QString strValue;
		switch (StartPos)
		{
		case START_LT:
		case START_RT:
			strValue = QString("%1").arg((double)YValue, 0, 'f', precision);
			break;
		case START_LD:
		case START_RD:
			strValue = QString("%1").arg((double)0, 0, 'f', precision);
			break;
		default:
			break;
		}

		double fontWidth = painter->fontMetrics().width(strValue);
		double fontHeight = painter->fontMetrics().height();
		QPointF textPot(initX - fontWidth - 5, initY + fontHeight / 3);
		painter->drawText(textPot, strValue);

		//绘制横向坐标尺
		double HinitX = space + 40;
		double HinitY = space + 20;
		QPointF leftPot(HinitX, HinitY);
		QPointF rightPot(width() - space, HinitY);
		painter->drawLine(leftPot, rightPot);

		//绘制横向标度尺
		length = rightPot.x() - leftPot.x();
		increment = length / XValue;
		setXPrecision(increment);
		//根据范围值绘制刻度及刻度值

		for (int i = 0; i < 96; i++)
		{
			if (i % 4 == 0) {
				QPointF dtopPot(HinitX, HinitY + longLineLen);
				QPointF dbotPot(HinitX, HinitY);
				painter->drawLine(dtopPot, dbotPot);

				/*QString strValue = QString("%1").arg((double)XminValue + (XmaxValue - XminValue) / 96 * i, 0, 'f', precision);
				double fontWidth = painter->fontMetrics().width(strValue);
				double fontHeight = painter->fontMetrics().height();
				QPointF textPot(HinitX - fontWidth / 2, HinitY - fontHeight / 3);
				painter->drawText(textPot, strValue);*/
			}
			else {
				if (i % (longStep / 2) == 0) {
					shortLineLen = 7;
				}
				else {
					shortLineLen = 4;
				}

				QPointF dtopPot(HinitX, HinitY + shortLineLen);
				QPointF dbotPot(HinitX, HinitY);
				painter->drawLine(dtopPot, dbotPot);
			}
			HinitX += length / 96;
		}
		QPointF dtopPot1(HinitX, HinitY + longLineLen);
		QPointF dbotPot1(HinitX, HinitY);
		painter->drawLine(dtopPot1, dbotPot1);



		QTransform trans;
		trans.rotate(-90);
		painter->setWorldTransform(trans);
		HinitX = space + 40 + 20;
		HinitY = space + 20;
		for (int i = 0; i < 96; i++)
		{
			if (i % 4 == 0) {
				QString strValue;
				switch (StartPos)
				{
				case START_LT:
				case START_LD:
					strValue = QString("%1").arg((double)(XValue) / 96 * i, 0, 'f', precision);
					break;
				case START_RT:
				case START_RD:
					strValue = QString("%1").arg((double)(XValue) / 96 * (96 - i), 0, 'f', precision);
					break;
				default:
					break;
				}

				double fontWidth = painter->fontMetrics().width(strValue);
				double fontHeight = painter->fontMetrics().height();
				QPointF textPot(-(HinitY - fontHeight / 3), HinitX - fontWidth / 2);
				painter->drawText(textPot, strValue);
			}
			HinitX += length / 96;
		}
		QString strValue1;
		switch (StartPos)
		{
		case START_LT:
		case START_LD:
			strValue1 = QString("%1").arg((double)XValue, 0, 'f', precision);
			break;
		case START_RT:
		case START_RD:
			strValue1 = QString("%1").arg((double)0, 0, 'f', precision);
			break;
		default:
			break;
		}
		double fontWidth1 = painter->fontMetrics().width(strValue1);
		double fontHeight1 = painter->fontMetrics().height();
		QPointF textPot1(-(HinitY - fontHeight1 / 3), HinitX - fontWidth1 / 2);

		painter->drawText(textPot1, strValue1);
		painter->restore();
	};
	void drawBarBg(QPainter *painter) {
		painter->save();
		painter->setPen(barBgColor);


		//绘制纵向标尺线 20的长度为刻度尺文字的宽度
		double initX = space + 40;
		double initY = space + 40;
		QPointF topLeftPot(initX, initY);
		QPointF bottomRightPot(width() - space, height() - spaceY);
		QRectF barbgRect = QRectF(topLeftPot, bottomRightPot);

		//painter->setBrush(barBgColor);
		painter->drawRect(barbgRect);
		painter->restore();
	};
	void drawOrangle(QPainter *painter)
	{
		//绘制定向角,以起始坐标为定向角
		painter->save();
		painter->setPen(orAngleColor);

		//找到起始位置
		QPainterPath path;
		int initX = space + 40;
		int initY = space + 40;
		int EndX = width() - 20;
		int EndY = height() - 100;
		switch (StartPos)
		{
		case START_LT:
			// 跳到第一个点
			path.moveTo(initX, initY);
			path.lineTo(initX + 10, initY);
			path.lineTo(initX, initY + 10);
			break;
		case START_RT:
			path.moveTo(EndX, initY);
			path.lineTo(EndX - 10, initY);
			path.lineTo(EndX, initY + 10);
			break;
		case START_LD:
			path.moveTo(initX, EndY);
			path.lineTo(initX + 10, EndY);
			path.lineTo(initX, EndY - 10);
			break;
		case START_RD:
			path.moveTo(EndX, EndY);
			path.lineTo(EndX - 10, EndY);
			path.lineTo(EndX, EndY - 10);
			break;
		default:
			break;
		}



		//结束，没有的画也可以画出来，但是最后封闭的那条线就没有了
		path.closeSubpath();
		painter->drawPath(path);    //画三角形
		painter->fillPath(path, orAngleColor);
		painter->restore();
	}

	void drawBar(QPainter *painter) {
		painter->save();
		painter->setPen(barColor);
		//绘制纵向标尺刻度
		for (int i = 0; i < panelInfoList.size(); i++)
		{
			PanelInfo pl = panelInfoList.at(i);
			QPointF topleftPot(pl.x*xprecision + space + 40, pl.y*yprecision + space + 40);
			QPointF botrightPot((pl.x + pl.width)*xprecision + space + 40, (pl.y + pl.height) * yprecision + space + 40);
			painter->drawRect(QRectF(topleftPot, botrightPot));
			painter->drawText(QPointF(topleftPot.x() + 3, topleftPot.y() + 10), pl.str);
		}

		QPen pen;
		pen.setColor(defBarColor);
		pen.setWidth(10);
		for (int i = 0; i < pointRedList.size(); i++)
		{
			painter->setPen(pen);
			painter->drawPoint(QPointF(pointRedList[i].x() * xprecision + space + 40, pointRedList[i].y() * yprecision + space + 40));
		}

		pen.setColor(QColor(255, 255, 0));
		pen.setWidth(10);
		for (int i = 0; i < pointYelList.size(); i++)
		{
			painter->setPen(pen);
			painter->drawPoint(QPointF(pointYelList[i].x() * xprecision + space + 40, pointYelList[i].y() * yprecision + space + 40));
		}

		pen.setColor(QColor(0, 0, 255));
		pen.setWidth(10);
		for (int i = 0; i < pointBlueList.size(); i++)
		{
			painter->setPen(pen);
			painter->drawPoint(QPointF(pointBlueList[i].x() * xprecision + space + 40, pointBlueList[i].y() * yprecision + space + 40));
		}

		painter->restore();
	};
	void drawLeftRightBar(QPainter *painter)
	{
		QImage image;
		if (bLeft)
		{
			image.load(":/MainWindow/resouse/left.png");
		}
		else
		{
			image.load(":/MainWindow/resouse/right.png");
		}
		painter->save();
		painter->setPen(barColor);

		painter->drawImage(QRect(width() / 2 - 200, height() - 100, 400, 80), image);

		painter->restore();
	}
private:
	double XValue;                //X最小值
	//double XmaxValue;                //X最大值
	double YValue;				//y最小值
	//double YmaxValue;				//y最大值
	int StartPos;					//设置定向角位置
	int precision;					//精确度,小数点后几位
	double xprecision;              //x方向单像素精度
	double yprecision;				//y方向单像素精度
	bool bLeft;

	int longStep;                   //长线条等分步长
	int shortStep;                  //短线条等分步长
	int space;                      //间距
	int spaceY;

	QColor bgColorStart;            //背景渐变开始颜色
	QColor bgColorEnd;              //背景渐变结束颜色
	QColor lineColor;               //线条颜色
	QColor barBgColor;              //柱状背景色
	QColor barColor;                //柱状颜色
	QColor defBarColor;
	QColor orAngleColor;			//定向角颜色
	QList<PanelInfo> panelInfoList;
	QList<QPoint> pointRedList;
	QList<QPoint> pointYelList;
	QList<QPoint> pointBlueList;

public:
	double getXValue()            const { return this->XValue; };
	//double getXMaxValue()            const { return this->XmaxValue; };
	double getYValue()            const { return this->YValue; };
	//double getYMaxValue()            const { return this->YmaxValue; };
	int getPrecision()              const { return this->precision; };
	bool getbLeft()					const { return this->bLeft; };
	int getLongStep()               const { return this->longStep; };
	int getShortStep()              const { return this->shortStep; };
	int getSpace()                  const { return this->space; };

	QColor getBgColorStart()        const { return bgColorStart; };
	QColor getBgColorEnd()          const { return bgColorEnd; };
	QColor getLineColor()           const { return lineColor; };
	QColor getBarBgColor()          const { return barBgColor; };
	QColor getBarColor()            const { return barColor; };
	QList<PanelInfo> getPanelInfoList() const { return panelInfoList; }

public Q_SLOTS:
	void setbLeft(bool isLeft) {
		this->bLeft = isLeft;
	}
	//设置最大最小值-范围值
	void setXRange(double Value) {
		if (0 >= Value) {
			return;
		}
		this->XValue = Value;

		this->update();
	};
	void setXRange(int minValue) {
		setXRange((double)minValue);
	};
	void setYRange(double Value) {
		if (0 >= Value) {
			return;
		}
		this->YValue = Value;

		this->update();
	};
	void setYRange(int minValue) {
		setYRange((double)minValue);
	};
	void setYValue(double minValue) {
		setYRange(minValue);
	};
	void setXValue(double Value) {
		setXRange(Value);
	};

	void setStartPos(int pos) {
		this->StartPos = pos;
	};

	//设置目标值
	void setValue(double value) {};
	void setValue(int value) {};

	//设置精确度
	void setPrecision(int precision) {
		//最大精确度为 3
		if (precision <= 3 && this->precision != precision) {
			this->precision = precision;
			this->update();
		}
	};
	//设置线条等分步长
	void setLongStep(int longStep) {
		//短步长不能超过长步长
		if (longStep < shortStep) {
			return;
		}

		if (this->longStep != longStep) {
			this->longStep = longStep;
			this->update();
		}
	};
	void setShortStep(int shortStep) {
		//短步长不能超过长步长
		if (longStep < shortStep) {
			return;
		}

		if (this->shortStep != shortStep) {
			this->shortStep = shortStep;
			this->update();
		}
	};
	//设置间距
	void setSpace(int space) {
		if (this->space != space) {
			this->space = space;
			this->update();
		}
	};
	//设置PanelInfo
	void setPanelInfoList(QList<PanelInfo> panelInfol) {
		this->panelInfoList = panelInfol;
	}
	void addPanelInfoList(PanelInfo panel) {
		this->panelInfoList.append(panel);
	}
	void clearPanelInfoList() {
		this->panelInfoList.clear();
	}

	void setPointRedList(QList<QPoint> panelInfol) {
		this->pointRedList = panelInfol;
	}
	void addPointRedList(QPoint panel) {
		this->pointRedList.append(panel);
	}
	void clearPointRedList() {
		this->pointRedList.clear();
	}

	void setPointYeList(QList<QPoint> panelInfol) {
		this->pointYelList = panelInfol;
	}
	void addPointYeList(QPoint panel) {
		this->pointYelList.append(panel);
	}
	void clearPointYeList() {
		this->pointYelList.clear();
	}

	void setPointBuleList(QList<QPoint> panelInfol) {
		this->pointBlueList = panelInfol;
	}
	void addPointBuleList(QPoint panel) {
		this->pointBlueList.append(panel);
	}
	void clearPointBuleList() {
		this->pointBlueList.clear();
	}

	//设置背景颜色
	void setBgColorStart(const QColor &bgColorStart) {};
	void setBgColorEnd(const QColor &bgColorEnd) {};
	//设置线条颜色
	void setLineColor(const QColor &lineColor) {};
	//设置柱状颜色
	void setBarBgColor(const QColor &barBgColor) {};
	void setBarColor(const QColor &barColor) {};

	double getXPrecision() { return xprecision; };
	double getYPrecision() { return yprecision; };
	void setXPrecision(double& val) { xprecision = val; };
	void setYPrecision(double& val) { yprecision = val; };

	int getStartPos() { return StartPos; };

Q_SIGNALS:
	void valueChanged(double value);
};

class LayoutInfo :public QWidget
{
public:
	LayoutInfo::LayoutInfo(QWidget *parent) : QWidget(parent)
	{
		setFont(QFont("Arial", 8));
	}

	~LayoutInfo() {
	};



protected:
	void paintEvent(QPaintEvent *) {

		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		drawPanelInfo(&painter);
	};
	void drawPanelInfo(QPainter *painter)
	{
		painter->save();
		painter->setPen(barBgColor);


		//绘制Panel详细信息
		double initX = space;
		double initY = space;
		QPointF topLeftPot(initX, initY);
		QPointF bottomRightPot(width() - space, height() - space);
		QRectF barbgRect = QRectF(topLeftPot, bottomRightPot);

		painter->drawRect(barbgRect);
		QPointF leftPot(initX - 20, initY);
		QPointF rightPot(initX - 5, initY);
		painter->drawLine(leftPot, rightPot);
		leftPot.setY(height() - space);
		rightPot.setY(height() - space);
		painter->drawLine(leftPot, rightPot);

		QPointF TopPot(initX, initY - 20);
		QPointF BtmPot(initX, initY - 5);
		painter->drawLine(TopPot, BtmPot);
		TopPot.setX(width() - space);
		BtmPot.setX(width() - space);
		painter->drawLine(TopPot, BtmPot);


		QString strValue = QString("%1mm").arg(infWidth);
		double fontWidth = painter->fontMetrics().width(strValue);
		double fontHeight = painter->fontMetrics().height();
		QPointF textPot(width() / 2 - 20 - fontWidth / 2, space / 2);
		painter->drawText(textPot, strValue);

		//painter->setWindow(width()/2, height()/2, fontWidth,fontHeight);
		//painter->rotate(90);
		QTransform trans;
		trans.rotate(90);
		painter->setWorldTransform(trans);
		strValue = QString("%1mm").arg(infHeight);
		textPot = QPointF(width() / 2 - 20 - fontWidth / 2, -space / 2);
		//textPot = QPointF(0,0);
		painter->drawText(textPot, strValue);
		painter->restore();
	}
public:
	void SetPanelInfo(double dWidth, double dHeight)
	{
		infWidth = dWidth;
		infHeight = dHeight;
		this->update();
	}

private:
	int space = 40;
	QColor barBgColor = QColor(0, 255, 0);
	double infWidth = 0.;
	double infHeight = 0.;
};