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

#define EDGELENGTH 128  // ����512*512ԭʼͼ��Ĳü���Ե����


class BmpConvert
{
	// ���캯��
    public:
		BmpConvert(const char *BmpFileName);
		~BmpConvert();

    // ����bmpͼ������
    public:
		void CroppedEdge();

    // ��ȡ��Mat���ݽṹ
    private:
		Mat image;

};


#endif