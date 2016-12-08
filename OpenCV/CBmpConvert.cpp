#include "HBmpConvert.h"

// 构造函数
BmpConvert::BmpConvert(const char *BmpFileName)
{
	image = imread(BmpFileName);
}

// 析构函数
BmpConvert::~BmpConvert()
{
	image.release();
}

// 裁剪Bmp图像的边缘
void BmpConvert::CroppedEdge()
{
	Mat croppedImage;
	// 截取边缘部分
	Rect roi(EDGELENGTH, EDGELENGTH, image.rows - 2 * EDGELENGTH, image.cols - 2 * EDGELENGTH);
	croppedImage = image(roi);
	// 深拷贝到当前操作对象
	image = croppedImage.clone();
	// 释放临时变量资源
	croppedImage.release();
	imshow("CroppedImage", image);

	waitKey();

	//image.release();
}