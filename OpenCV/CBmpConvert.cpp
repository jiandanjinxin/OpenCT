#include "HBmpConvert.h"

// ���캯��
BmpConvert::BmpConvert(const char *BmpFileName)
{
	image = imread(BmpFileName);
}

// ��������
BmpConvert::~BmpConvert()
{
	image.release();
}

// �ü�Bmpͼ��ı�Ե
void BmpConvert::CroppedEdge()
{
	Mat croppedImage;
	// ��ȡ��Ե����
	Rect roi(128, 96, image.cols - 224, image.rows - 224);
	croppedImage = image(roi);
	// �������ǰ��������
	image = croppedImage.clone();
	// �ͷ���ʱ������Դ
	croppedImage.release();
	imshow("CroppedImage", image);
	// �����ˢ��ͼ������ʾ��
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