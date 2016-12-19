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

// ��ӦPatch��ȡ
void LymphHOG::CroppedPatch(const Mat &img, Mat &result, int Xpos, int Ypos, int Xlength, int Ylength)
{
	// ��ȡ��Ե����
	Rect roi(128 + Xpos * 32, 96 + Ypos * 32, Xlength, Ylength);
	result = img(roi);

}

// ��ȡ��ע�ļ�������������, [0]Ϊ����������[1]Ϊ��������
vector<int> LymphHOG::GetSamplesNumber(const char *filename)
{
	string ImgName; // image filename, absoluted path
	ifstream finRecords(filename);  // ����ͼƬ���б��ļ�

	vector<int> result; // ���صı�ע�ļ�����������
	int pos = 0;
	int neg = 0;

	for (int num = 0; getline(finRecords, ImgName); num++)
	{
		// �����������
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

// ѵ��SVMʶ��ģ��
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
		ifstream finRecords("F:\\lymph node detection dataset\\label.txt");  // ����ͼƬ���б��ļ�


		// ����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��
		Mat sampleFeatureMat;
		// ѵ����������������������������������ĸ�������������1 : 1��ʾ�У�-1��ʾ��
		Mat sampleLabelMat;
		// ��ȡ������ͼ��Դ�ļ�
		Mat src;

        // ��ȡ��ע�ļ�������������
		vector<int> SamplesNumber = GetSamplesNumber("F:\\lymph node detection dataset\\label.txt");
		cout << SamplesNumber[0] << " " << SamplesNumber[1] << endl;
		// ���ζ�ȡ���м�¼������hog������
		for (int num = 0; getline(finRecords, ImgName); num++)
		{
			// �����������
			vector<string> records;
			split(ImgName, " ", records);
			// Xposition, Yposition, label, InstanceNumber, FilePosition
			//cout << records[0] << " " << records[1] << " " << records[2] << " " << records[3] << " " << records[4] << endl;
		
			// ������ݶ���,������81��Ϊһ�������ͼƬ
			if (num % 81 == 0)
			{
				// ����ͼ��洢·��
				char BmpName[256];
				sprintf_s(BmpName, "%s%s%s%06d.bmp", "F:\\lymph node detection dataset\\DOI\\", records[4].c_str(), "\\", atoi(records[3].c_str()) + 1);
 				src = imread(BmpName); // ��ȡͼƬ
			}
			// ��ȡ�����Ķ�ӦPatch
			Mat patch;
			CroppedPatch(src, patch, atoi(records[0].c_str()), atoi(records[1].c_str()), 32, 32);
 			//cout << patch.cols << " " << patch.rows << endl;

			vector<float> descriptors;                   // HOG����������
			hog.compute(patch, descriptors, Size(4, 4)); // ����HOG�����ӣ���ⴰ���ƶ�������4��4��
			cout << "����:" << num << " ������ά��:" << descriptors.size() << endl;

			// �����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������
			if (0 == num)
			{
				DescriptorDim = descriptors.size(); // HOG�����ӵ�ά��
			
				// ��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
				sampleFeatureMat = Mat::zeros(SamplesNumber[0] + SamplesNumber[1], DescriptorDim, CV_32FC1);
				// ��ʼ��ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�0��ʾ����
				sampleLabelMat = Mat::zeros(SamplesNumber[0] + SamplesNumber[1], 1, CV_32FC1);
			}

			// ������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num, i) = descriptors[i];  // ��num�����������������еĵ�i��Ԫ��
			
			sampleLabelMat.at<float>(num, 0) = (float)atoi(records[2].c_str());  // ���������Ϊ1����ʾ��; -1��ʾ��

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
		// ���ζ�ȡע���ļ�Ŀ¼������AnnotationNameΪ����������·��, ��������������������
		for (int num = 0; num < 5 && getline(finAnnotation, AnnotationName); num++)
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
					memset(LabelMatrix9, -1, sizeof(LabelMatrix9));  //ʹ��memset��������arr�����и���ֵ-1

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

