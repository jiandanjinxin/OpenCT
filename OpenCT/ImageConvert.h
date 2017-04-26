#ifndef _IMAGECONVERT_H
#define _IMAGECONVERT_H

#include <opencv2/opencv.hpp>

class ImageConvert
{
public:
	//基于OpenCV显示图像
	static void ImageShow(const char* ImageName);

	//标记矩形区域
	static void markRect(cv::Mat& mat, int xpos, int ypos);
};

#endif