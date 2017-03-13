#include "ImageConvert.h"

#include <opencv2/opencv.hpp>

void ImageConvert::ImageShow(const char* ImageName)
{
	cv::Mat img = cv::imread(ImageName);
	//�������ͼ��ʧ��
	if (img.empty())
	{
		fprintf(stderr, "Can not load image %s\n", ImageName);
	}
	cv::imshow("image", img);

	//�˺����ȴ�������������������ͷ���
	cv::waitKey();
}