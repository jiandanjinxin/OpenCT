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

		/*
		// ���������HOG�������������ļ�
		ofstream fout("SampleFeatureMat.txt");

		for (int i = 0; i < PosSamNO + NegSamNO; i++)
		{
			fout << i << endl;
			for (int j = 0; i < DescriptorDim; j++)
			{
				fout << sampleFeatureMat.at<float>(i, j) << " ";
			}
			fout << endl;
		}
		*/

		// ѵ��SVM������
		// ������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
		CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
		// SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C=0.01
		CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);

		cout << "��ʼѵ��SVM������" << endl;
		svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param); // ѵ��������
		cout << "ѵ�����" << endl;
		svm.save("SVM_HOG.xml");  // ��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�
	}
	else // ��TRAINΪfalse����XML�ļ���ȡѵ���õķ�����
	{
		// ��XML�ļ���ȡѵ���õ�SVMģ��
		svm.load("SVM_HOG.xml");
	}

	/*************************************************************************************************
	����SVMѵ����ɺ�õ���XML�ļ����棬��һ�����飬����support vector������һ�����飬����alpha,��һ��������������rho;
	��alpha����ͬsupport vector��ˣ�ע�⣬alpha*supportVector,���õ�һ����������֮���ٸ���������������һ��Ԫ��rho
	��ˣ���õ���һ�������������ø÷�������ֱ���滻opencv�����˼��Ĭ�ϵ��Ǹ���������cv::HOGDescriptor::setSVMDetector()����
	�Ϳ����������ѵ������ѵ�������ķ������������˼���ˡ�
	***************************************************************************************************/
	
	DescriptorDim = svm.get_var_count(); // ����������ά������HOG�����ӵ�ά��
	int supportVectorNum = svm.get_support_vector_count(); // ֧�������ĸ���
	cout << "֧������������" << supportVectorNum << endl;

	Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1); // alpha���������ȵ���֧����������
	Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1); // ֧����������
	Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1); // alpha��������֧����������Ľ��

	// ��֧�����������ݸ��Ƶ�supportVectorMat������
	for (int i = 0; i < supportVectorNum; i++)
	{
		const float *pSVData = svm.get_support_vector(i); // ���ص�i��֧������������ָ��
		for (int j = 0; j < DescriptorDim; j++)
		{
			supportVectorMat.at<float>(i, j) = pSVData[j];
		}
	}
	// ��alpha���������ݸ��Ƶ�alphaMat��
	double *pAlphaData = svm.get_alpha_vector(); // ����SVM�ľ��ߺ����е�alpha����
	for (int i = 0; i < supportVectorNum; i++)
	{
		alphaMat.at<float>(0, i) = pAlphaData[i];
	}

	system("pause");
}
