#include "ToolFunction.h"

ToolClass::ToolClass()
{
}

ToolClass::~ToolClass()
{
}


cv::Mat ToolClass::QImage2Mat(QImage& image)
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
		break;
	}
	return mat;
}
QImage ToolClass::Mat2QImage(cv::Mat mat)
{
	if (!mat.data)
	{
		qWarning() << "Mat2QImage:The Input Is A empty Mat!" << mat.cols << " r:" << mat.rows;

		return QImage();
	}
	// 8-bits unsigned, NO. OF CHANNELS = 1    
	if (mat.type() == CV_8UC1)
	{
		//@20211203下面的代码可能引起异常(未加mat.step)，原因这是因为QImage图的数据是以字节为单位保存的，每一行的字节数必须是4的整数倍。然而在实际运行过程中image的每行字节可能会有所不同，容易导致图像转换出错或图像效果与原图像不同。这时可以添加&mat.step参数解决。
		QImage img = QImage((const unsigned char*)(mat.data), mat.cols, mat.rows, mat.step, QImage::Format_Indexed8).copy();
		//更新后的代码
		//QImage img(mat.cols, mat.rows, QImage::Format_Indexed8);
		QVector<QRgb> colorTable;
		for (int k = 0; k < 256; ++k)
		{
			colorTable.push_back(qRgb(k, k, k));
		}
		img.setColorTable(colorTable);
		// Copy input Mat    
		uchar* pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = img.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return img;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3    
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat    
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat    
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		//QImage image = QImage((const unsigned char*)(mat.data), mat.cols, mat.rows, QImage::Format_RGB888);
		return image.copy();
	}
	else if (mat.type() == CV_8UC4)
	{
		// Copy input Mat    
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat    
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);

		return image.copy();
	}
	else
	{
		return QImage();
	}
}

//QImage ToolClass::MatToQImage(cv::Mat mtx)
//{
//	switch (mtx.type())
//	{
//	case CV_8UC1:
//	{
//		QImage img((const unsigned char*)(mtx.data), mtx.cols, mtx.rows, mtx.cols, QImage::Format_Grayscale8);
//		return img;
//	}
//	break;
//	case CV_8UC3:
//	{
//		QImage img((const unsigned char*)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 3, QImage::Format_RGB888);
//		return img.rgbSwapped();
//	}
//	break;
//	case CV_8UC4:
//	{
//		QImage img((const unsigned char*)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 4, QImage::Format_ARGB32);
//		return img;
//	}
//	break;
//	default:
//	{
//		QImage img;
//		return img;
//	}
//	break;
//	}
//}
//
////cv::Mat ToolClass::QImage2Mat(QImage image)
////{
////	cv::Mat mat;
////	switch (image.format())
////	{
////	case QImage::Format_ARGB32:
////	case QImage::Format_RGB32:
////	case QImage::Format_ARGB32_Premultiplied:
////		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
////		break;
////	case QImage::Format_RGB888:
////		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
////		//cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
////		break;
////	case QImage::Format_Indexed8:
////		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
////		break;
////	}
////	return mat;
////}
////Qt读入彩色图后一般为Format_RGB32格式（4通道），而OpenCV一般用3通道的，因此进行了转换。
////cv::Mat ToolClass::QImage2Mat(QImage image)
////{
////	cv::Mat mat;
////	switch (image.format())
////	{
////	case QImage::Format_RGB32:  //一般Qt读入彩色图后为此格式
////		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
////		cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);   //转3通道
////		break;
////	case QImage::Format_RGB888:
////		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
////		//cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
////		break;
////	case QImage::Format_Indexed8:
////		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
////		break;
////	}
////	return mat;
////}