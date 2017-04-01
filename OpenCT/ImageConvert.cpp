#include "ImageConvert.h"

#include <opencv2/opencv.hpp>

void ImageConvert::ImageShow(const char* ImageName)
{
	cv::Mat img = cv::imread(ImageName);
	//如果读入图像失败
	if (img.empty())
	{
		fprintf(stderr, "Can not load image %s\n", ImageName);
	}
	cv::imshow("image", img);

	//此函数等待按键，按键盘任意键就返回
	cv::waitKey();
}