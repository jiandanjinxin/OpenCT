#include "HLymphHOG.h"
#include <hash_map>

// �ַ���split
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
	HOGSVM svm;      // hog classifier

	// if train is true, retrain the classifier
	if (flag)
	{
		string ImgName; // image filename, absoluted path
		ifstream finRecords("F:\\lymph node detection dataset\\label.txt");  // ����ͼƬ���б��ļ�


		// ����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��
		Mat sampleFeatureMat;
		// ѵ����������������������������������ĸ�������������1 : 1��ʾ�У�-1��ʾ��
		Mat sampleLabelMat;

		// ���ζ�ȡ���м�¼������hog������
		for (int num = 0; getline(finRecords, ImgName); num++)
		{
			
		
		}

	}
}

// ��������ļ�����
void LymphHOG::LabelOutput(bool flag)
{  
    if (flag)
	{
		// ȫ��ע���ļ�Ŀ¼
		string AnnotationName;
		// ע���ļ�Ŀ¼����·��
		ifstream finAnnotation("F:\\lymph node detection dataset\\pixelfile.lst");
		ofstream foutLabel("F:\\lymph node detection dataset\\label.txt");
		// ���ζ�ȡע���ļ�Ŀ¼������AnnotationNameΪ����������·��
		for (int num = 0; num < 80 && getline(finAnnotation, AnnotationName); num++)
		{

			// ��¼�Ѵ���ڵ�
			hash_map<string, bool> records;
			
			// ÿ��label��ע
			string AnnotationRecord;
			// ��Ӧ���˵�ȫ����¼��·��annotation_indices
			ifstream finRecord(AnnotationName);

			// ��ȡ���м�¼
			for (int point = 0; getline(finRecord, AnnotationRecord); point++)
			{
				vector<string> InstancePixel;
				split(AnnotationRecord, " ", InstancePixel);
				
				// δ����ǰ�ڵ�ļ�¼
				if (!(records.find(InstancePixel[2]) != records.end()))
				{
					ifstream finAll(AnnotationName);
					// 9 * 9��Ǿ����ʼ��Ϊ0
					int LabelMatrix9[9][9] = {0};

					// ���α�ǵ�ǰͼƬ����������ͬ��Ƭ�Ĳ�ͬlabelλ��
					for (int i = 0; getline(finAll, AnnotationRecord); i++)
					{

						vector<string> Pixel;
						split(AnnotationRecord, " ", Pixel);
						// ����뵱ǰδ����ڵ���ͬ������
 						if (!Pixel[2].compare(InstancePixel[2]))
						{
							//cout << InstancePixel[2] << " " << Pixel[2] << endl;

							int Xpos = (atoi(Pixel[0].c_str()) - 128) / 32;
							int Ypos = (atoi(Pixel[1].c_str()) - 96) / 32;
							LabelMatrix9[Xpos][Ypos] = 1;
							//cout << Xpos << " " << Ypos << endl;
						}					
					}
					
					// ���������ݵ��ļ���
					for (int i = 0; i < 9; i++)
					  for (int j = 0; j < 9; j++)
						  foutLabel << i << " " << j << " " << LabelMatrix9[i][j] << " " << InstancePixel[2] << " " << AnnotationName.substr(84, 13) << endl;

					// ��ǵ�ǰͼƬ�Ѵ���
					records.insert(std::pair<std::string, bool>(InstancePixel[2], true));
				}
				
			}
			//cout << "process:" << num << endl;
		}

	}
}

