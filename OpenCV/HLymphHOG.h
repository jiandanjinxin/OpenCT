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
	// 构造函数


public:
	/*
	* 提取HOG图像特征并训练SVM模型，flag表示是否需要训练，或直接提取训练好的模型
	*/
	void SVMTraining(bool flag);

	// 标注数据文件生成
	void LabelOutput(bool flag);

	// 字符串split
	void split(const string& src, const string& separator, vector<string>& dest);

	// 对应Patch提取
	void CroppedPatch(const Mat &img, Mat &result, int Xpos, int Ypos, int Xlength, int Ylength);

	// 获取标注文件的正负样本数, [0]为正样本数，[1]为负样本数
	vector<int> GetSamplesNumber(const char *filename);

private:
	
};

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

#endif