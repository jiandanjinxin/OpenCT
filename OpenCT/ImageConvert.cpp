#include "ImageConvert.h"

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

void ImageConvert::markRect(cv::Mat& mat, int xpos, int ypos)
{
	cv::rectangle(mat, cv::Point(xpos - 16, ypos - 16), cv::Point(xpos + 16, ypos + 16), cv::Scalar(0, 0, 255), 1);
}