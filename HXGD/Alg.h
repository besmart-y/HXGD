#pragma once
#include <opencv2\opencv.hpp>
#include <qobject.h>
typedef struct LINESCANIMAGES
{
	cv::Mat CH0_SRC; //����
	cv::Mat CH1_SRC; //����
	cv::Mat CH2_SRC; //����
}LineScanImages;//���������ͼ�洢

class HXGDAlg:public QObject
{
	Q_OBJECT
public:
	HXGDAlg();
	~HXGDAlg();
	int SplitImage(cv::Mat src, LineScanImages& images);
};
