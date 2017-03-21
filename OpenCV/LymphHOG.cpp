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

		/*
		// ���������hog�������������ļ�
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

		// ѵ��SVM������
		// ������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
	    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
		// SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C = 0.01
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

	//	����SVMѵ����ɺ�õ���XML�ļ����棬��һ�����飬����support vector������һ�����飬����alpha,��һ��������������rho;
	//	��alpha����ͬsupport vector��ˣ�ע�⣬alpha * supportVector,���õ�һ����������֮���ٸ���������������һ��Ԫ��rho
	//	��ˣ���õ���һ�������������ø÷�������ֱ���滻opencv�����˼��Ĭ�ϵ��Ǹ���������cv::HOGDescriptor::setSVMDetector()����
	//	�Ϳ����������ѵ������ѵ�������ķ��������м�⡣

	DescriptorDim = svm.get_var_count();                   // ����������ά������HOG�����ӵ�ά��
	int supportVectorNum = svm.get_support_vector_count(); // ֧�������ĸ���
    cout << "֧����������:" << supportVectorNum << endl;

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
	double *pAlphaData = svm.get_alpha_vector();          // ����SVM�ľ��ߺ����е�alpha����
	for (int i = 0; i < supportVectorNum; i++)
	{
		alphaMat.at<float>(0, i) = (float)pAlphaData[i];
	}

    // ����-(alphaMat * supportVectorMat),����ŵ�resultMat��
	resultMat = -1 * alphaMat * supportVectorMat;

	// �õ����յ�setSVMDetector(const vector<float>& detector)�����п��õļ����
	vector<float> myDetector;
	// ��resultMat�е����ݸ��Ƶ�����myDetector��
	for (int i = 0; i < DescriptorDim; i++)
	{
		myDetector.push_back(resultMat.at<float>(0, i));
	}

	// ������ƫ����rho���õ������
	myDetector.push_back(svm.get_rho());

	cout << "�����ά��:" << myDetector.size() << endl;
	
	// ����HOGDescriptor�ļ����
	HOGDescriptor myHOG(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);
	myHOG.setSVMDetector(myDetector);
    //myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	// �������Ӳ������ļ�
	ofstream fout("HOGDetectorForOpenCV.txt");
	for (unsigned int i = 0; i < myDetector.size(); i++)
	{
		fout << myDetector[i] << endl;
	}

	/*
	// ����ͼƬ����HOG������
	Mat img = imread("F:\\lymph node detection dataset\\DOI\\ABD_LYMPH_001\\000419.bmp");
	Mat src;
	CroppedPatch(img, src, 5, 5, 32, 32);
	
	vector<Rect> found, found_filtered; // ���ο�����
	cout << "���ж�߶�HOG������:" << endl;
    // srcΪ���������ͼƬ��foundΪ��⵽Ŀ�������б�����3Ϊ�����ڲ�����ΪĿ�����ֵ��Ҳ���Ǽ�⵽��������SVM���೬ƽ��ľ���;
    // ����4Ϊ��������ÿ���ƶ��ľ��롣�������ǿ��ƶ���������������5Ϊͼ������Ĵ�С������6Ϊ����ϵ����������ͼƬÿ�γߴ��������ӵı�����
    // ����7Ϊ����ֵ����У��ϵ������һ��Ŀ�걻������ڼ�����ʱ���ò�����ʱ�����˵������ã�Ϊ0ʱ��ʾ����������á�
	myHOG.detectMultiScale(src, found, 0, Size(4, 4), Size(8, 8), 1.05, 2); // ��ͼƬ���ж�߶ȼ��
	cout << "�ҵ��ľ��ο������" << found.size() << endl;

	// �ҳ�����û��Ƕ�׵ľ��ο�r,������found_filtered��,�����Ƕ�׵Ļ�,��ȡ���������Ǹ����ο����found_filtered��
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

	// �����ο���Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,����������Ҫ��һЩ����
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

// ͳ�Ʒ�����
void LymphHOG::classify()
{
	// ���뵥���Ĳ���ͼ�񲢶���HOG�����ӽ��з���
	Mat img = imread("F:\\lymph node detection dataset\\DOI\\ABD_LYMPH_002\\000349.bmp");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Mat testImg;
			CroppedPatch(img, testImg, i, j, 32, 32);

			HOGDescriptor hog(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);  // hog descriptor, calculate the hog descriptor.
			vector<float> descriptor;
			hog.compute(testImg, descriptor, Size(4, 4));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
			Mat testFeatureMat = Mat::zeros(1, 1764, CV_32FC1);//����������������������
			// ������õ�HOG�����Ӹ��Ƶ�testFeatureMat������
			for (int i = 0; i < descriptor.size(); i++)
				testFeatureMat.at<float>(0, i) = descriptor[i];

			// ��ѵ���õ�SVM�������Բ���ͼƬ�������������з���
			int result = svm.predict(testFeatureMat); // �������
			cout << "��������" << result << endl;
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

