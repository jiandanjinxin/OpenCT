#ifndef _IMAGECONVERT_H
#define _IMAGECONVERT_H

class ImageConvert
{
public:
	//基于OpenCV显示图像
	static void ImageShow(const char* ImageName);

	//对规定图像进行范围标定
	static void ImageMark(const char* ImageName);
};

#endif