#ifndef __OPENCV_BMPCONVERT_H  
#define __OPENCV_BMPCONVERT_H

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2\opencv.hpp>  

using namespace std;
using namespace cv;

//#define EDGELENGTH 128  // ����512*512ԭʼͼ��Ĳü���Ե����


class BmpConvert
{
	// ���캯��
    public:
		BmpConvert(const char *BmpFileName);
		~BmpConvert();

    // ����bmpͼ������
    public:
		/*
		* �鿴��Ե�ü����Ч����������128*96 length 228
		*/
		void CroppedEdge(int x, int y, int LengthX, int LengthY);

		/*
		* �Ա�Ե�ü����ͼ�����Patch�з�
		*/
		vector<Mat> Segmentation(CvSize SubPlot);

		/*
		* OpenCVͬһ��������ʾ����ͼƬ����Ҫ�����з�Ч�� 32*32*81
		*/
		void MultiImageShow(const std::string& MultiShow_WinName, const vector<Mat>& SrcImg_V, CvSize SubPlot, CvSize ImgMax_Size = cvSize(400, 280));

    // ��ȡ��Mat���ݽṹ
    private:
		Mat image;

};


#endif