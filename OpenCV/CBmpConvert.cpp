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

	waitKey();

	//image.release();
}