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

// Extends from CvSVM, when generate detection parameters in setSVMDetector(), we need to use the trained parameters in SVM's decision_func,
// the parameters of decision_func is type of protected, we cannot visit it directly, only to extend it and using function to visit it.
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

class LymphHOG
{

public:
	// ���캯��


public:
	/*
	* ��ȡHOGͼ��������ѵ��SVMģ�ͣ�flag��ʾ�Ƿ���Ҫѵ������ֱ����ȡѵ���õ�ģ��
	*/
	void SVMTraining(bool flag);

	// ��ע�����ļ�����
	void LabelOutput(bool flag);

	// �ַ���split
	void split(const string& src, const string& separator, vector<string>& dest);

	// ��ӦPatch��ȡ
	void CroppedPatch(const Mat &img, Mat &result, int Xpos, int Ypos, int Xlength, int Ylength);

	// ��ȡ��ע�ļ�������������, [0]Ϊ����������[1]Ϊ��������
	vector<int> GetSamplesNumber(const char *filename);

	// ͳ�Ʒ�����
	void classify();

private:
	HOGSVM svm;
};

#endif