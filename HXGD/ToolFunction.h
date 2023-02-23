#pragma once
#include <opencv2\opencv.hpp>
#include <qimage.h>
#include <qdebug.h>

//#pragma comment(lib,"opencv_imgproc248.lib")

class ToolClass
{
public:
	ToolClass();
	~ToolClass();
	static QImage Mat2QImage(cv::Mat mat);
	static cv::Mat	QImage2Mat(QImage& image);
	

private:

};

