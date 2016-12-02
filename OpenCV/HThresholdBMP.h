#ifndef __OPENCV_THRESHOLD_H  
#define __OPENCV_THRESHOLD_H

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

class BMPFileFormat
{
	// ���캯��
    public:
	    BMPFileFormat();
		BMPFileFormat(const char *BMPFileName);

    // ��ֵ������
    public:
		void ThresholdToDCM();

    // ��ȡ��Mat���ݽṹ
    private:

		Mat image;

};


#endif