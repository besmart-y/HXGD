#include "Alg.h"

HXGDAlg::HXGDAlg()
{
}

HXGDAlg::~HXGDAlg()
{
}




int HXGDAlg::SplitImage(cv::Mat src, LineScanImages& images)
{
	int H = src.rows / 3;
	int W = src.cols;

	images.CH0_SRC = cv::Mat(cv::Size(W, H), CV_8UC1);
	images.CH1_SRC = cv::Mat(cv::Size(W, H), CV_8UC1);
	images.CH2_SRC = cv::Mat(cv::Size(W, H), CV_8UC1);
	uchar* CH1_Mat_ptr = (uchar*)images.CH0_SRC.data;
	uchar* CH2_Mat_ptr = (uchar*)images.CH1_SRC.data;
	uchar* CH3_Mat_ptr = (uchar*)images.CH2_SRC.data;
	uchar* src_ptr = (uchar*)src.data;

	for (int i = 0, k = 0; i < H; ++i, k += 3)
	{
		for (int j = 0; j < W; ++j)
		{
			int step1 = i * W + j;
			int step2 = k * W + j;
			CH1_Mat_ptr[step1] = src_ptr[step2];
			CH2_Mat_ptr[step1] = src_ptr[step2 + W];
			CH3_Mat_ptr[step1] = src_ptr[step2 + 2 * W];
		}
	}
	return 0;
}

//void main()
//{
//	cv::Mat src = cv::imread("D:/yong/upan/test.bmp");
//	LineScanImages line_img;
//	splitImage(src, line_img);
//}