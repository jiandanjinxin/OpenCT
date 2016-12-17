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
	//HOGDescriptor hog(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);  // hog descriptor, calculate the hog descriptor.
	//int DescriptorDim;                                                       // hog descriptor dimension depend on size of picture, detective window, block size, cell bin.
	//HOGSVM svm;                                                              // hog classifier

	if (flag)
	{
		// 所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
		//Mat sampleFeatureMat;
		// 训练样本的类别向量，行数等于所有样本的个数，列数等于1:  1表示有，-1表示无
		//Mat sampleLabelMat;

		// 全部注释文件目录
		string AnnotationName;
		// 注释文件目录所在路径
		ifstream finAnnotation("F:\\lymph node detection dataset\\pixelfile.lst");  

		// 依次读取注释文件目录，其中AnnotationName为样例的完整路径
		for (int num = 0; num < 1 && getline(finAnnotation, AnnotationName); num++)
		{
			//cout << AnnotationName << endl;

			// 
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

					// 依次标记当前图片号下所有相同切片的不同label位置
					for (int i = 0; getline(finAll, AnnotationRecord); i++)
					{

						vector<string> Pixel;
						split(AnnotationRecord, " ", Pixel);
						// 如果与当前未处理节点相同，则处理
 						if (!Pixel[2].compare(InstancePixel[2]))
						{
							cout << InstancePixel[2] << " " << Pixel[2] << endl;
						}					
					}
					// 标记当前图片已处理
					records.insert(std::pair<std::string, bool>(InstancePixel[2], true));
				}
				
			}
		}

	}
}

