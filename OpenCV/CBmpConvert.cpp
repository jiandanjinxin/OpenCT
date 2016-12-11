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
	Rect roi(128, 96, image.cols - 224, image.rows - 224);
	croppedImage = image(roi);
	// 深拷贝到当前操作对象
	image = croppedImage.clone();
	// 释放临时变量资源
	croppedImage.release();
	imshow("CroppedImage", image);
	// 不间断刷新图像以显示。
	waitKey();
}

void BmpConvert::MultiImageShow(const std::string& MultiShow_WinName, const vector<Mat>& SrcImg_V, CvSize SubPlot, CvSize ImgMax_Size = cvSize(400, 280))
{
	// Image Window
	Mat Image_Display ;
	// Width of source image  
	CvSize Img_OriginSize = cvSize(SrcImg_V[0].cols, SrcImg_V[0].rows);
	//******************** Set the width for displayed image ********************//
	//Width vs height ratio of source image
	float WH_Ratio_Origin = Img_OriginSize.width / (float)Img_OriginSize.height;
	CvSize ImgDisplay_Size = cvSize(100, 100);

	if (Img_OriginSize.width > ImgMax_Size.width)
		ImgDisplay_Size = cvSize(ImgMax_Size.width, (int)ImgMax_Size.width / WH_Ratio_Origin);
	else if (Img_OriginSize.height > ImgMax_Size.height)
		ImgDisplay_Size = cvSize((int)ImgMax_Size.height * WH_Ratio_Origin, ImgMax_Size.height);
	else
		ImgDisplay_Size = cvSize(Img_OriginSize.width, Img_OriginSize.height);
}