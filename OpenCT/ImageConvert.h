#ifndef _IMAGECONVERT_H
#define _IMAGECONVERT_H

#include <opencv2/opencv.hpp>

class ImageConvert
{
public:
	//����OpenCV��ʾͼ��
	static void ImageShow(const char* ImageName);

	//��Ǿ�������
	static void markRect(cv::Mat& mat, int xpos, int ypos);
};

#endif