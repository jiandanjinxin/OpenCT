#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

#define PosSamNO 2400     // amount of pos samples 
#define NegSamNO 12000    // amount of neg samples

#define TRAIN true        // need training or not, true means training, false means read SVM model form xml file.
#define CENTRAL_CROP true // true:aim to crop the human size of 64 * 128 from INRIA pos samples which is size of 96 * 160 when training.

// HardExample��amount of neg samples, if HardExampleNO over 0, means after handling the inital neg dataset, continue to handle the HardExample neg samples.
// HardExample must set to 0 when don't use this. When init dimension of the matrix of feature vector and the matrix of feature class, is will be used.
#define HardExampleNO 0

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
	double get_rho()
	{
		return this->decision_func->rho;
	}
};

int main()
{
	// detective window (64, 128), block size (16, 16), block stride (8, 8), cell size (8, 8), nbins 9  
	HOGDescriptor hog(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);  // hog descriptor, calculate the hog descriptor.
	int DescriptorDim;                                                          // hog descriptor dimension depend on size of picture, detective window, block size, cell bin.
	HOGSVM svm;                                                                 // hog classifier

	// if train is true, retrain the classifier
	if (TRAIN)
	{
		string ImgName; // image filename, absoluted path
		ifstream finPos("D:\\Computer Vision\\INRIAPerson\\96X160H96\\Train\\pos.lst");  // ������ͼƬ���б��ļ�
		ifstream finNeg("D:\\Computer Vision\\INRIAPerson\\96X160H96\\Train\\neg.lst");  // ������ͼƬ���б��ļ�

		// ����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��
		Mat sampleFeatureMat;
		// ѵ����������������������������������ĸ�������������1:1��ʾ���ˣ�-1��ʾ����
		Mat sampleLabelMat;

		// ���ζ�ȡ������ͼƬ������HOG������
		for (int num = 0; num < PosSamNO && getline(finPos, ImgName); num++)
		{
			cout << "process:" << ImgName << endl;

			Mat src = imread(ImgName); // ��ȡͼƬ
			if (CENTRAL_CROP)
			{
				// �� 96 * 160 ��INRIA������ͼƬ����Ϊ 64 * 128������ȥ�������Ҹ�16������
				src = src(Rect(16, 16, 64, 128));
			}

			vector<float> descriptors; // HOG����������
			hog.compute(src, descriptors, Size(8, 8)); // ����HOG�����ӣ���ⴰ���ƶ�������8��8��

			cout << "������ά��:" << descriptors.size() << endl;

			// �����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������
			if (0 == num)
			{
				DescriptorDim = descriptors.size(); // HOG�����ӵ�ά��

				// ��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
				sampleFeatureMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, DescriptorDim, CV_32FC1);
				// ��ʼ��ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�0��ʾ����
				sampleLabelMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, 1, CV_32FC1);
			}

			// ������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num, i) = descriptors[i];  // ��num�����������������еĵ�i��Ԫ��

			sampleLabelMat.at<float>(num, 0) = 1;  // ���������Ϊ1����ʾ����

		}

		// ���ζ�ȡ������ͼƬ������HOG������
		for (int num = 0; num < NegSamNO && getline(finNeg, ImgName); num++)
		{
			cout << "process:" << ImgName << endl;

			Mat src = imread(ImgName); // ��ȡͼƬ
			vector<float> descriptors; // HOG����������
			hog.compute(src, descriptors, Size(8, 8)); // ����HOG�����ӣ���ⴰ���ƶ�����(8,8)

			cout << "������ά��:" << descriptors.size() << endl;
			// ������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];// ��PosSamNO + num�����������������еĵ�i��Ԫ��

			sampleLabelMat.at<float>(num + PosSamNO, 0) = -1; // ���������Ϊ-1������
		}

		// ����HardExample������
		if (HardExampleNO > 0)
		{

		}
	}

	system("pause");
}
