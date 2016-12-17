#ifndef __OPENCV_HOG_H  
#define __OPENCV_HOG_H  

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;

class LymphHOG
{

public:



public:
	/*
	* 提取HOG图像特征并训练SVM模型，flag表示是否需要训练，或直接提取训练好的模型
	*/
	void SVMTraining(bool flag);
	// 字符串split
	void split(const string& src, const string& separator, vector<string>& dest);

private:
	
};


#endif