#include "HLymphHOG.h"
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
	HOGSVM svm;                                                              // hog classifier

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

