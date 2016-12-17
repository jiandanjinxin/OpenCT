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
	* ��ȡHOGͼ��������ѵ��SVMģ�ͣ�flag��ʾ�Ƿ���Ҫѵ������ֱ����ȡѵ���õ�ģ��
	*/
	void SVMTraining(bool flag);
	// �ַ���split
	void split(const string& src, const string& separator, vector<string>& dest);

private:
	
};


#endif