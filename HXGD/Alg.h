#pragma once
#include <opencv2\opencv.hpp>
#include <qobject.h>
typedef struct LINESCANIMAGES
{
	cv::Mat CH0_SRC; //明场
	cv::Mat CH1_SRC; //暗场
	cv::Mat CH2_SRC; //背光
}LineScanImages;//线阵相机采图存储

class HXGDAlg:public QObject
{
	Q_OBJECT
public:
	HXGDAlg();
	~HXGDAlg();
	int SplitImage(cv::Mat src, LineScanImages& images);
};
