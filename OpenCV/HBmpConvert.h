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

#define EDGELENGTH 64  // 定义512*512原始图像的裁剪边缘长度


class BmpConvert
{
	// 构造函数
    public:
		BmpConvert(const char *BmpFileName);
		~BmpConvert();

    // 各类bmp图像处理函数
    public:
		void CroppedEdge();

    // 读取的Mat数据结构
    private:
		Mat image;

};


#endif