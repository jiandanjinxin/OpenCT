#include "HCandidate.h"

/*
* ��ȡCandidate�ļ���������������,��ת��Ϊ�������ݣ�д��pos��neg�ļ���
*/
void Candidate::OutputCandidate()
{
	// �����������б���
	string InitData;
	ifstream finInitData("F:\\lymph node detection dataset\\DATA\\init.txt");

	string PosFileName;
	ifstream finFile("F:\\lymph node detection dataset\\candidatepos.lst");

	string NegFileName;
	ifstream finFileNeg("F:\\lymph node detection dataset\\candidateneg.lst");

	int count = 0;
	// ��ȡÿ����Ա�ĳ�ʼ��������
	while (getline(finInitData, InitData))
	{
		//cout << InitData << endl;
		vector<string> initData;
		split(InitData, " ", initData);
		double initx = atof(initData[0].c_str());
		double inity = atof(initData[1].c_str());
		double initz = atof(initData[2].c_str());
		double pixelstep = atof(initData[3].c_str());
		double zstep = atof(initData[4].c_str());
		string PatientName = initData[5];
		// �������е�����pos�ĺ�ѡ���ļ�����ȻҪ���Ѿ����ɼ�¼�ļ�
		// ���������ļ���ȡ
		
		getline(finFile, PosFileName);
        cout << PosFileName << endl;
		string PosCoordidate;
		ifstream finPos(PosFileName);
		while (getline(finPos, PosCoordidate))
		{
			// cout << PosCoordidate << endl; ÿ����������������
			
			vector<string> posCoordidate;
			split(PosCoordidate, " ", posCoordidate);
			double realx = atof(posCoordidate[0].c_str());
			double realy = atof(posCoordidate[1].c_str());
			double realz = atof(posCoordidate[2].c_str());

			int pixelx = (int)((realx - initx) / pixelstep + 0.5);
			int pixely = (int)((realy - inity) / pixelstep + 0.5);
			int pixelz = (int)((realz - initz) / zstep + 0.5);
			
			if (pixelz > 0)
			{
				cout << pixelx << " " << pixely << " " << pixelz << endl;
				// ����posͼ��·��
				char PosBmpName[256];
				char CandidateImgName[256];
				sprintf_s(PosBmpName, "F:\\lymph node detection dataset\\DOI\\%s\\%06d.bmp", PatientName.c_str(), pixelz + 1);
				sprintf_s(CandidateImgName, "F:\\lymph node detection dataset\\DATA\\Pos\\%06d_%s.bmp", ++count, PatientName.c_str());
				//cout << PosBmpName << endl;
				//cout << count << endl;
				BmpConvert bmp = BmpConvert(PosBmpName);
				bmp.OutputCandidateImage(CandidateImgName, pixelx, pixely);
				bmp.~BmpConvert();
			}
		}
		
		// �������еĸ���neg�ĺ�ѡ���ļ�����ȻҪ���Ѿ����ɼ�¼�ļ�
		// ���и����ļ���ȡ

		getline(finFileNeg, NegFileName);
		cout << NegFileName << endl;
		string NegCoordidate;
		ifstream finNeg(NegFileName);
		while (getline(finNeg, NegCoordidate))
		{
			// cout << PosCoordidate << endl; ÿ����������������

			vector<string> negCoordidate;
			split(NegCoordidate, " ", negCoordidate);
			double realx = atof(negCoordidate[0].c_str());
			double realy = atof(negCoordidate[1].c_str());
			double realz = atof(negCoordidate[2].c_str());

			int pixelx = (int)((realx - initx) / pixelstep + 0.5);
			int pixely = (int)((realy - inity) / pixelstep + 0.5);
			int pixelz = (int)((realz - initz) / zstep + 0.5);

			if (pixelz > 0)
			{
				cout << pixelx << " " << pixely << " " << pixelz << endl;
				// ����posͼ��·��
				char NegBmpName[256];
				char CandidateImgName[256];
				sprintf_s(NegBmpName, "F:\\lymph node detection dataset\\DOI\\%s\\%06d.bmp", PatientName.c_str(), pixelz + 1);
				sprintf_s(CandidateImgName, "F:\\lymph node detection dataset\\DATA\\Neg\\%06d_%s.bmp", ++count, PatientName.c_str());
				//cout << PosBmpName << endl;
				//cout << count << endl;
				BmpConvert bmp = BmpConvert(NegBmpName);
				bmp.OutputCandidateImage(CandidateImgName, pixelx, pixely);
				bmp.~BmpConvert();
			}
		}
	}
}


/*
* ��������������ʼλ��ÿ�����˶�������ͬ�����Զ�Ӧ������Ҫ�������������ע�ļ�
* ������������Ա����ʼSagittal�������꣬��ʼCoronal�������꣬��ʼAxial�������꣬�ƶ�����
*/
void Candidate::WorldCoordinateConvert()
{
	//��ȡÿ���˵�����dcm�ļ���ȡ��ʼ��Ϣ
	CStatDir statdir;

	char buf[256] = "F:\\lymph node detection dataset\\DOI";
	char filename[] = "F:\\lymph node detection dataset\\DATA\\coordinate.txt";
	ofstream fout(filename);
	//����Ҫ������Ŀ¼  
	if (!statdir.SetInitDir(buf))
	{
		puts("Ŀ¼������");
		return ;
	}

	//��ʼ����  
	vector<string> file_vec = statdir.BeginBrowseFilenames("*000000.dcm");
	for (vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it)
		fout << *it << endl;

	// ��ȡÿ���˵ĳ�ʼ��Ϣ�ļ��Ի�ȡ��ʼ��������
	string DcmFileName;
	ifstream finFile("F:\\lymph node detection dataset\\DATA\\coordinate.txt");
	ofstream coordinateout("F:\\lymph node detection dataset\\DATA\\init.txt");

	while (getline(finFile, DcmFileName))
	{
		// ʵ����dcm������
		TDcmFileFormat dcm = TDcmFileFormat(DcmFileName.c_str());
		// ��ȡdcmͼ���ImagePositionPatient
		string result = dcm.getImagePositionPatient();
		vector<string> ImagePosition;
		split(result, "\\", ImagePosition);
		double initx = atof(ImagePosition[0].c_str());
		double inity = atof(ImagePosition[1].c_str());
		double realz = atof(ImagePosition[2].c_str());
		//cout << value0 << " " << value1 << " " << value2 << endl;

		// ��ȡdcmͼ���PixelSpacing
		string result1 = dcm.getPixelSpacing();
		vector<string> PixelSpacing;
		split(result1, "\\", PixelSpacing);
		double pixelstep = atof(PixelSpacing[0].c_str());
		int InstancePosition = dcm.getPositionNumber();
		//cout << InstancePosition << " " << value2 - InstancePosition + 1 << endl;
		double initz = realz - InstancePosition + 1;

		string PatientName = DcmFileName.substr(36, 13);
		//cout << PatientName << endl;
		string DcmFileName1 = DcmFileName.replace(DcmFileName.end() - 5, DcmFileName.end() - 4, "1");
		//cout << next << endl;
		TDcmFileFormat dcmnext = TDcmFileFormat(DcmFileName1.c_str());
		string result2 = dcmnext.getImagePositionPatient();
		vector<string> ImagePosition1;
		split(result2, "\\", ImagePosition1);

		double realz1 = atof(ImagePosition1[2].c_str());
		int InstancePosition1 = dcmnext.getPositionNumber();

		double zstep = (realz1 - realz) / (InstancePosition1 - InstancePosition);
		cout << zstep << endl;

		coordinateout << initx << " " << inity << " " << initz << " " << pixelstep << " " << zstep << " " << PatientName << endl;

	}
}