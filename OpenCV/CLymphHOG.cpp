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

void LymphHOG::SVMTraining(bool flag)
{  
	// detective window (64, 128), block size (16, 16), block stride (8, 8), cell size (8, 8), nbins 9
	HOGDescriptor hog(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);  // hog descriptor, calculate the hog descriptor.
	int DescriptorDim;                                                       // hog descriptor dimension depend on size of picture, detective window, block size, cell bin.
	HOGSVM svm;                                                              // hog classifier

	// if train is true, retrain the classifier
	if (flag)
	{
		// 所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
		Mat sampleFeatureMat;
		// 训练样本的类别向量，行数等于所有样本的个数，列数等于1:  1表示有，-1表示无
		Mat sampleLabelMat;



		string AnnotationName;
		ifstream finAnnotation("F:\\lymph node detection dataset\\pixelfile.lst");  // 正样本图片名列表文件

		string AnnotationRecord;
		ifstream finRecord();

		// 依次读取注释文件目录，其中AnnotationName为样例的完整路径
		for (int num = 0; num < 1 && getline(finAnnotation, AnnotationName); num++)
		{
			//cout << AnnotationName << endl;

			// 对应一个人的数据处理
			hash_map<string, bool> records;
			
			string AnnotationRecord;
			ifstream finRecord(AnnotationName);

			for (int point = 0; getline(finRecord, AnnotationRecord); point++)
			{
				vector<string> IntPixel;
				split(AnnotationRecord, " ", IntPixel);
				
				
				if (!(records.find(IntPixel[2]) != records.end()))
				{
					cout << "process";
				}
				
			}
		}

	}
}

