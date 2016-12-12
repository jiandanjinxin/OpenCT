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

void BmpConvert::InfoShow()
{
	cout << image.cols << " " << image.rows << endl;
}

// 裁剪Bmp图像的边缘
void BmpConvert::CroppedEdge(int x, int y, int CroppedLengthX, int CroppedLengthY)
{
	Mat croppedImage;
	// 截取边缘部分
	Rect roi(x, y, image.cols - CroppedLengthX, image.rows - CroppedLengthY);
	croppedImage = image(roi);
	// 深拷贝到当前操作对象
	image = croppedImage.clone();
	// 释放临时变量资源
	croppedImage.release();
	//imshow("CroppedImage", image);
	// 不间断刷新图像以显示。
	//waitKey();
}

// 对边缘裁剪后的图像进行Patch切分
vector<Mat> BmpConvert::Segmentation(CvSize SubPlot)
{
	vector<Mat> result;
	
	int height = image.rows;
	int width = image.cols;

	// 每个小Patch的长度和宽度
	int pos_height = height / SubPlot.height;
	int pos_width = width / SubPlot.width;

	for (int i = 0; i < SubPlot.width; i++)
	{
		for (int j = 0; j < SubPlot.height; j++)
		{
			Rect rect(j * pos_height, i * pos_width, pos_width, pos_height);
			Mat roi;
			image(rect).copyTo(roi);
			result.push_back(roi);
			roi.release();
		}
	}
	return result;
}

void BmpConvert::MultiImageShow(const std::string& MultiShow_WinName, const vector<Mat>& SrcImg_V, CvSize SubPlot, CvSize ImgMax_Size)
{
	// Image Window
	Mat Image_Display;
	// Width of source image  
	CvSize Img_OriginSize = cvSize(SrcImg_V[0].cols, SrcImg_V[0].rows);
	//******************** Set the width for displayed image ********************//
	//Width vs height ratio of source image
	float WH_Ratio_Origin = Img_OriginSize.width / (float)Img_OriginSize.height;
	// Initial to 100 * 100
	CvSize ImgDisplay_Size = cvSize(100, 100);

	if (Img_OriginSize.width > ImgMax_Size.width)
		ImgDisplay_Size = cvSize(ImgMax_Size.width, (int)(ImgMax_Size.width / WH_Ratio_Origin));
	else if (Img_OriginSize.height > ImgMax_Size.height)
		ImgDisplay_Size = cvSize((int)(ImgMax_Size.height * WH_Ratio_Origin), ImgMax_Size.height);
	else
		ImgDisplay_Size = cvSize(Img_OriginSize.width, Img_OriginSize.height);

	//******************** Check Image numbers with Subplot layout ********************//
	int Img_Num = (int)SrcImg_V.size();
	if (Img_Num > SubPlot.width * SubPlot.height)
	{
		cout << "Your SubPlot Setting is too small !" << endl;
		exit(0);
	}

	//******************** Blank setting ********************//  
	CvSize DispBlank_Edge = cvSize(80, 60);
	CvSize DispBlank_Gap = cvSize(15, 15);

	//******************** Size for Window ********************//
	Image_Display.create(Size(ImgDisplay_Size.width * SubPlot.width + DispBlank_Edge.width + (SubPlot.width - 1) * DispBlank_Gap.width,
		ImgDisplay_Size.height * SubPlot.height + DispBlank_Edge.height + (SubPlot.height - 1) * DispBlank_Gap.height), CV_8UC3);
	Image_Display.setTo(0); // Background 

	//Left top position for each image  
	int EdgeBlank_X = (Image_Display.cols - (ImgDisplay_Size.width * SubPlot.width + (SubPlot.width - 1) * DispBlank_Gap.width)) / 2;
	int EdgeBlank_Y = (Image_Display.rows - (ImgDisplay_Size.height * SubPlot.height + (SubPlot.height - 1) * DispBlank_Gap.height)) / 2;
	CvPoint LT_BasePos = cvPoint(EdgeBlank_X, EdgeBlank_Y);
	CvPoint LT_Pos = LT_BasePos;

	//Display all images  
	for (int i = 0; i < Img_Num; i++)
	{
		//Obtain the left top position  
		if ((i % SubPlot.width == 0) && (LT_Pos.x != LT_BasePos.x))
		{
			LT_Pos.x = LT_BasePos.x;
			LT_Pos.y += (DispBlank_Gap.height + ImgDisplay_Size.height);
		}
		//Writting each to Window's Image  
		Mat imgROI = Image_Display(Rect(LT_Pos.x, LT_Pos.y, ImgDisplay_Size.width, ImgDisplay_Size.height));
		resize(SrcImg_V[i], imgROI, Size(ImgDisplay_Size.width, ImgDisplay_Size.height));

		LT_Pos.x += (DispBlank_Gap.width + ImgDisplay_Size.width);
	}


	cvNamedWindow(MultiShow_WinName.c_str(), CV_WINDOW_AUTOSIZE);
	cvShowImage(MultiShow_WinName.c_str(), &(IplImage(Image_Display)));
	cvWaitKey(0);
	cvDestroyWindow(MultiShow_WinName.c_str());
}