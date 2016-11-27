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

// HardExample：amount of neg samples, if HardExampleNO over 0, means after handling the inital neg dataset, continue to handle the HardExample neg samples.
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
		ifstream finPos("D:\\Computer Vision\\INRIAPerson\\96X160H96\\Train\\pos.lst");  // 正样本图片名列表文件
		ifstream finNeg("D:\\Computer Vision\\INRIAPerson\\96X160H96\\Train\\neg.lst");  // 负样本图片名列表文件

		// 所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
		Mat sampleFeatureMat;
		// 训练样本的类别向量，行数等于所有样本的个数，列数等于1:1表示有人，-1表示无人
		Mat sampleLabelMat;

		// 依次读取正样本图片，生成HOG描述子
		for (int num = 0; num < PosSamNO && getline(finPos, ImgName); num++)
		{
			cout << "process:" << ImgName << endl;

			Mat src = imread(ImgName); // 读取图片
			if (CENTRAL_CROP)
			{
				// 将 96 * 160 的INRIA正样本图片剪裁为 64 * 128，即剪去上下左右各16个像素
				src = src(Rect(16, 16, 64, 128));
			}

			vector<float> descriptors; // HOG描述子向量
			hog.compute(src, descriptors, Size(8, 8)); // 计算HOG描述子，检测窗口移动步长（8，8）

			cout << "描述子维数:" << descriptors.size() << endl;

			// 处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵
			if (0 == num)
			{
				DescriptorDim = descriptors.size(); // HOG描述子的维数

				// 初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat
				sampleFeatureMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, DescriptorDim, CV_32FC1);
				// 初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，0表示无人
				sampleLabelMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, 1, CV_32FC1);
			}

			// 将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num, i) = descriptors[i];  // 第num个样本的特征向量中的第i个元素

			sampleLabelMat.at<float>(num, 0) = 1;  // 正样本类别为1，表示有人

		}

		// 依次读取负样本图片，生成HOG描述子
		for (int num = 0; num < NegSamNO && getline(finNeg, ImgName); num++)
		{
			cout << "process:" << ImgName << endl;

			Mat src = imread(ImgName); // 读取图片
			vector<float> descriptors; // HOG描述子向量
			hog.compute(src, descriptors, Size(8, 8)); // 计算HOG描述子，检测窗口移动步长(8,8)

			cout << "描述子维数:" << descriptors.size() << endl;
			// 将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];// 第PosSamNO + num个样本的特征向量中的第i个元素

			sampleLabelMat.at<float>(num + PosSamNO, 0) = -1; // 负样本类别为-1，无人
		}

		// 处理HardExample负样本
		if (HardExampleNO > 0)
		{

		}
	}

	system("pause");
}
