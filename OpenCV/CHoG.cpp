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

#define TRAIN false       // need training or not, true means training, false means read SVM model form xml file.
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
	float get_rho()
	{
		return (float)this->decision_func->rho;
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

		/*
		// 输出样本的HOG特征向量矩阵到文件
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

		// 训练SVM分类器
		// 迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代
		CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
		// SVM参数：SVM类型为C_SVC；线性核函数；松弛因子C=0.01
		CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);

		cout << "开始训练SVM分类器" << endl;
		svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param); // 训练分类器
		cout << "训练完成" << endl;
		svm.save("SVM_HOG.xml");  // 将训练好的SVM模型保存为xml文件
	}
	else // 若TRAIN为false，从XML文件读取训练好的分类器
	{
		// 从XML文件读取训练好的SVM模型
		svm.load("SVM_HOG.xml");
	}

	/*************************************************************************************************
	线性SVM训练完成后得到的XML文件里面，有一个数组，叫做support vector，还有一个数组，叫做alpha,有一个浮点数，叫做rho;
	将alpha矩阵同support vector相乘，注意，alpha*supportVector,将得到一个列向量。之后，再该列向量的最后添加一个元素rho
	如此，变得到了一个分类器，利用该分类器，直接替换opencv中行人检测默认的那个分类器（cv::HOGDescriptor::setSVMDetector()），
	就可以利用你的训练样本训练出来的分类器进行行人检测了。
	***************************************************************************************************/
	
	DescriptorDim = svm.get_var_count(); // 特征向量的维数，即HOG描述子的维数
	int supportVectorNum = svm.get_support_vector_count(); // 支持向量的个数
	cout << "支持向量个数：" << supportVectorNum << endl;

	Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1); // alpha向量，长度等于支持向量个数
	Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1); // 支持向量矩阵
	Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1); // alpha向量乘以支持向量矩阵的结果

	// 将支持向量的数据复制到supportVectorMat矩阵中
	for (int i = 0; i < supportVectorNum; i++)
	{
		const float *pSVData = svm.get_support_vector(i); // 返回第i个支持向量的数据指针
		for (int j = 0; j < DescriptorDim; j++)
		{
			supportVectorMat.at<float>(i, j) = pSVData[j];
		}
	}
	// 将alpha向量的数据复制到alphaMat中
	double *pAlphaData = svm.get_alpha_vector(); // 返回SVM的决策函数中的alpha向量
	for (int i = 0; i < supportVectorNum; i++)
	{
		alphaMat.at<float>(0, i) = (float)pAlphaData[i];
	}
	// 计算-(alphaMat * supportVectorMat),结果放到resultMat中
	resultMat = -1 * alphaMat * supportVectorMat;

	// 得到最终的setSVMDetector(const vector<float>& detector)参数中可用的检测子
	vector<float> myDetector;
	// 将resultMat中的数据复制到数组myDetector中
	for (int i = 0; i < DescriptorDim; i++)
	{
		myDetector.push_back(resultMat.at<float>(0, i));
	}

	// 最后添加偏移量rho，得到检测子
	myDetector.push_back(svm.get_rho());

	cout << "检测子维数：" << myDetector.size() << endl;

	// 设置HOGDescriptor的检测子
	HOGDescriptor myHOG;
	myHOG.setSVMDetector(myDetector);
	// myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	// 保存检测子参数到文件
	ofstream fout("HOGDetectorForOpenCV.txt");
	for (unsigned int i = 0; i < myDetector.size(); i++)
	{
		fout << myDetector[i] << endl;
	}

	/**************读入图片进行HOG行人检测******************/
	Mat src = imread("00000.png");

	vector<Rect> found, found_filtered; // 矩形框数组
	cout << "进行多尺度HOG人体检测" << endl;
	// src为输入待检测的图片；found为检测到目标区域列表；参数3为程序内部计算为行人目标的阈值，也就是检测到的特征到SVM分类超平面的距离;
	// 参数4为滑动窗口每次移动的距离。它必须是块移动的整数倍；参数5为图像扩充的大小；参数6为比例系数，即测试图片每次尺寸缩放增加的比例；
	// 参数7为组阈值，即校正系数，当一个目标被多个窗口检测出来时，该参数此时就起了调节作用，为0时表示不起调节作用。
	myHOG.detectMultiScale(src, found, 0, Size(8, 8), Size(32, 32), 1.05, 2); // 对图片进行多尺度行人检测
	cout << "找到的矩形框个数：" << found.size() << endl;

	// 找出所有没有嵌套的矩形框r,并放入found_filtered中,如果有嵌套的话,则取外面最大的那个矩形框放入found_filtered中
	for (unsigned int i = 0; i < found.size(); i++)
	{
		Rect r = found[i];
		unsigned int j = 0;
		for (; j < found.size(); j++)
		if (j != i && (r & found[j]) == r)
			break;
		if (j == found.size())
			found_filtered.push_back(r);
	}

	// 画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
	for (unsigned int i = 0; i < found_filtered.size(); i++)
	{
		Rect r = found_filtered[i];
		r.x += cvRound(r.width * 0.1);
		r.width = cvRound(r.width * 0.8);
		r.y += cvRound(r.height * 0.07);
		r.height = cvRound(r.height * 0.8);
		rectangle(src, r.tl(), r.br(), Scalar(0, 255, 0), 3);
	}

	imwrite("ImgProcessed.jpg", src);
	namedWindow("src", 0);
	imshow("src", src);
	waitKey();    //注意：imshow之后必须加waitKey，否则无法显示图像

	/******************读入单个64*128的测试图并对其HOG描述子进行分类*********************/
	////读取测试图片(64*128大小)，并计算其HOG描述子
	//Mat testImg = imread("person014142.jpg");
	//Mat testImg = imread("noperson000026.jpg");
	//vector<float> descriptor;
	//hog.compute(testImg,descriptor,Size(8,8));//计算HOG描述子，检测窗口移动步长(8,8)
	//Mat testFeatureMat = Mat::zeros(1,3780,CV_32FC1);//测试样本的特征向量矩阵
	//将计算好的HOG描述子复制到testFeatureMat矩阵中
	//for(int i=0; i<descriptor.size(); i++)
	//	testFeatureMat.at<float>(0,i) = descriptor[i];

	//用训练好的SVM分类器对测试图片的特征向量进行分类
	//int result = svm.predict(testFeatureMat);//返回类标
	//cout<<"分类结果："<<result<<endl;

	system("pause");
}
