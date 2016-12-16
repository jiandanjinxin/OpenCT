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
using namespace cv;

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

class HOGSVM : public CvSVM
{
public:
	// get alpha array in SVM decision function.
	double* get_alpha_vector()
	{
		return this->decision_func->alpha;
	}

	// get rho parameter in SVM decision function, which is offset.
	float get_rho()
	{
		return (float)this->decision_func->rho;
	}
};

#endif