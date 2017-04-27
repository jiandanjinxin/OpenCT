#include "LymphHOG.h"
#include <hash_map>

// 字符串split
void LymphHOG::split(const string& src, const string& separator, vector<string>& dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator, start);
		if (index != string::npos)
		{
			substring = str.substr(start, index - start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator, index);
			if (start == string::npos) return;
		}
	} while (index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}

// 对应Patch提取
void LymphHOG::CroppedPatch(const Mat &img, Mat &result, int Xpos, int Ypos, int Xlength, int Ylength)
{
	// 截取边缘部分
	Rect roi(128 + Xpos * 32, 96 + Ypos * 32, Xlength, Ylength);
	result = img(roi);

}

// 获取标注文件的正负样本数, [0]为正样本数，[1]为负样本数
vector<int> LymphHOG::GetSamplesNumber(const char *filename)
{
	string ImgName; // image filename, absoluted path
	ifstream finRecords(filename);  // 样本图片名列表文件

	vector<int> result; // 返回的标注文件正负样本数
	int pos = 0;
	int neg = 0;

	for (int num = 0; getline(finRecords, ImgName); num++)
	{
		// 解析标记数据
		vector<string> records;
		split(ImgName, " ", records);

		if (atoi(records[2].c_str()) == -1)
			neg++;
		else
			pos++;
	}

	result.push_back(pos);
	result.push_back(neg);

	return result;
}

// 训练SVM识别模型
void LymphHOG::SVMTraining(bool flag)
{
	// detective window (32, 32), block size (8, 8), block stride (4, 4), cell size (4, 4), nbins 9
	HOGDescriptor hog(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);  // hog descriptor, calculate the hog descriptor.
	int DescriptorDim;                                                       // hog descriptor dimension depend on size of picture, detective window, block size, cell bin.

	// if train is true, retrain the classifier
	if (flag)
	{
		string ImgName; // image filename, absoluted path
		ifstream finRecords("F:\\lymph node detection dataset\\label.txt");  // 样本图片名列表文件


		// 所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
		Mat sampleFeatureMat;
		// 训练样本的类别向量，行数等于所有样本的个数，列数等于1 : 1表示有，-1表示无
		Mat sampleLabelMat;
		// 读取的样本图像源文件
		Mat src;

        // 获取标注文件的正负样本数
		vector<int> SamplesNumber = GetSamplesNumber("F:\\lymph node detection dataset\\label.txt");
		cout << SamplesNumber[0] << " " << SamplesNumber[1] << endl;
		// 依次读取所有记录，生成hog描述子
		for (int num = 0; getline(finRecords, ImgName); num++)
		{
			// 解析标记数据
			vector<string> records;
			split(ImgName, " ", records);
			// Xposition, Yposition, label, InstanceNumber, FilePosition
			//cout << records[0] << " " << records[1] << " " << records[2] << " " << records[3] << " " << records[4] << endl;
		
			// 标记数据读入,若超过81个为一组则更新图片
			if (num % 81 == 0)
			{
				// 生成图像存储路径
				char BmpName[256];
				sprintf_s(BmpName, "%s%s%s%06d.bmp", "F:\\lymph node detection dataset\\DOI\\", records[4].c_str(), "\\", atoi(records[3].c_str()) + 1);
 				src = imread(BmpName); // 读取图片
			}
			// 读取样本的对应Patch
			Mat patch;
			CroppedPatch(src, patch, atoi(records[0].c_str()), atoi(records[1].c_str()), 32, 32);
 			//cout << patch.cols << " " << patch.rows << endl;

			vector<float> descriptors;                   // HOG描述子向量
			hog.compute(patch, descriptors, Size(4, 4)); // 计算HOG描述子，检测窗口移动步长（4，4）
			cout << "处理:" << num << " 描述子维数:" << descriptors.size() << endl;

			// 处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵
			if (0 == num)
			{
				DescriptorDim = descriptors.size(); // HOG描述子的维数
			
				// 初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat
				sampleFeatureMat = Mat::zeros(SamplesNumber[0] + SamplesNumber[1], DescriptorDim, CV_32FC1);
				// 初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，0表示无人
				sampleLabelMat = Mat::zeros(SamplesNumber[0] + SamplesNumber[1], 1, CV_32FC1);
			}

			// 将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num, i) = descriptors[i];  // 第num个样本的特征向量中的第i个元素
			
			sampleLabelMat.at<float>(num, 0) = (float)atoi(records[2].c_str());  // 正样本类别为1，表示有; -1表示无

		}

		/*
		// 输出样本的hog特征向量矩阵到文件
		ofstream fout("SampleFeatureMat.txt");
		
		for (int i = 0; i < SamplesNumber[0] + SamplesNumber[1]; i++)
		{
			fout << i << endl;
			for (int j = 0; j < DescriptorDim; j++)
			{
				fout << sampleFeatureMat.at<float>(i, j) << " ";
			}
			fout << endl;
		}
		*/	

		// 训练SVM分类器
		// 迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代
	    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
		// SVM参数：SVM类型为C_SVC；线性核函数；松弛因子C = 0.01
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

	//	线性SVM训练完成后得到的XML文件里面，有一个数组，叫做support vector，还有一个数组，叫做alpha,有一个浮点数，叫做rho;
	//	将alpha矩阵同support vector相乘，注意，alpha * supportVector,将得到一个列向量。之后，再该列向量的最后添加一个元素rho
	//	如此，变得到了一个分类器，利用该分类器，直接替换opencv中行人检测默认的那个分类器（cv::HOGDescriptor::setSVMDetector()），
	//	就可以利用你的训练样本训练出来的分类器进行检测。

	DescriptorDim = svm.get_var_count();                   // 特征向量的维数，即HOG描述子的维数
	int supportVectorNum = svm.get_support_vector_count(); // 支持向量的个数
    cout << "支持向量个数:" << supportVectorNum << endl;

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
	double *pAlphaData = svm.get_alpha_vector();          // 返回SVM的决策函数中的alpha向量
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

	cout << "检测子维数:" << myDetector.size() << endl;
	
	// 设置HOGDescriptor的检测子
	HOGDescriptor myHOG(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);
	myHOG.setSVMDetector(myDetector);
    //myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	// 保存检测子参数到文件
	ofstream fout("HOGDetectorForOpenCV.txt");
	for (unsigned int i = 0; i < myDetector.size(); i++)
	{
		fout << myDetector[i] << endl;
	}

	/*
	// 读入图片进行HOG病灶检测
	Mat img = imread("F:\\lymph node detection dataset\\DOI\\ABD_LYMPH_001\\000419.bmp");
	Mat src;
	CroppedPatch(img, src, 5, 5, 32, 32);
	
	vector<Rect> found, found_filtered; // 矩形框数组
	cout << "进行多尺度HOG病灶检测:" << endl;
    // src为输入待检测的图片；found为检测到目标区域列表；参数3为程序内部计算为目标的阈值，也就是检测到的特征到SVM分类超平面的距离;
    // 参数4为滑动窗口每次移动的距离。它必须是块移动的整数倍；参数5为图像扩充的大小；参数6为比例系数，即测试图片每次尺寸缩放增加的比例；
    // 参数7为组阈值，即校正系数，当一个目标被多个窗口检测出来时，该参数此时就起了调节作用，为0时表示不起调节作用。
	myHOG.detectMultiScale(src, found, 0, Size(4, 4), Size(8, 8), 1.05, 2); // 对图片进行多尺度检测
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
	waitKey();    
	*/
}

// 统计分类结果
void LymphHOG::classify()
{
	// 读入单个的测试图像并对其HOG描述子进行分类
	Mat img = imread("F:\\lymph node detection dataset\\DOI\\ABD_LYMPH_002\\000349.bmp");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Mat testImg;
			CroppedPatch(img, testImg, i, j, 32, 32);

			HOGDescriptor hog(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);  // hog descriptor, calculate the hog descriptor.
			vector<float> descriptor;
			hog.compute(testImg, descriptor, Size(4, 4));//计算HOG描述子，检测窗口移动步长(8,8)
			Mat testFeatureMat = Mat::zeros(1, 1764, CV_32FC1);//测试样本的特征向量矩阵
			// 将计算好的HOG描述子复制到testFeatureMat矩阵中
			for (int i = 0; i < descriptor.size(); i++)
				testFeatureMat.at<float>(0, i) = descriptor[i];

			// 用训练好的SVM分类器对测试图片的特征向量进行分类
			int result = (int)svm.predict(testFeatureMat); // 返回类标
			cout << "分类结果：" << result << endl;
		}
	}
}

// 标记数据文件生成
void LymphHOG::LabelOutput(bool flag)
{  
    if (flag)
	{
		// 全部注释文件目录
		string AnnotationName;
		// 注释文件目录所在路径
		ifstream finAnnotation("F:\\lymph node detection dataset\\pixelfile.lst");
		ofstream foutLabel("F:\\lymph node detection dataset\\label.txt");
		// 依次读取注释文件目录，其中AnnotationName为样例的完整路径, 生成样本包含样本人数
		for (int num = 0; num < 5 && getline(finAnnotation, AnnotationName); num++)
		{

			// 记录已处理节点
			hash_map<string, bool> records;
			
			// 每条label标注
			string AnnotationRecord;
			// 对应个人的全部记录的路径annotation_indices
			ifstream finRecord(AnnotationName);

			// 读取所有记录
			for (int point = 0; getline(finRecord, AnnotationRecord); point++)
			{
				vector<string> InstancePixel;
				split(AnnotationRecord, " ", InstancePixel);
				
				// 未处理当前节点的记录
				if (!(records.find(InstancePixel[2]) != records.end()))
				{
					ifstream finAll(AnnotationName);
					// 9 * 9标记矩阵初始化为0
					int LabelMatrix9[9][9] = {0};
					memset(LabelMatrix9, -1, sizeof(LabelMatrix9));  //使用memset函数，将arr数组中赋于值-1

					// 依次标记当前图片号下所有相同切片的不同label位置
					for (int i = 0; getline(finAll, AnnotationRecord); i++)
					{

						vector<string> Pixel;
						split(AnnotationRecord, " ", Pixel);
						// 如果与当前未处理节点相同，则处理
 						if (!Pixel[2].compare(InstancePixel[2]))
						{
							//cout << InstancePixel[2] << " " << Pixel[2] << endl;

							int Xpos = (atoi(Pixel[0].c_str()) - 128) / 32;
							int Ypos = (atoi(Pixel[1].c_str()) - 96) / 32;
							LabelMatrix9[Xpos][Ypos] = 1;
							//cout << Xpos << " " << Ypos << endl;
						}					
					}
					
					// 输出标记数据到文件中
					for (int i = 0; i < 9; i++)
					  for (int j = 0; j < 9; j++)
						  foutLabel << i << " " << j << " " << LabelMatrix9[i][j] << " " << InstancePixel[2] << " " << AnnotationName.substr(84, 13) << endl;

					// 标记当前图片已处理
					records.insert(std::pair<std::string, bool>(InstancePixel[2], true));
				}
				
			}
			//cout << "process:" << num << endl;
		}

	}
}

