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

//#define EDGELENGTH 128  // 定义512*512原始图像的裁剪边缘长度


class BmpConvert
{
	// 构造函数
    public:
		BmpConvert(const char *BmpFileName);
		~BmpConvert();

    // 各类bmp图像处理函数
    public:
		/*
		* 查看边缘裁剪后的效果，基本在128*96 length 228
		*/
		void CroppedEdge();

		/*
		* OpenCV同一窗口下显示多张图片，主要测试切分效果 32*32*81
		*/
		void MultiImageShow(const std::string& MultiShow_WinName, const vector<Mat>& SrcImg_V, CvSize SubPlot, CvSize ImgMax_Size = cvSize(400, 280));

    // 读取的Mat数据结构
    private:
		Mat image;

};


#endif