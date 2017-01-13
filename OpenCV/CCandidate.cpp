#include "HCandidate.h"

/*
* 读取Candidate文件的世界坐标数据,并转换为像素数据，写入pos及neg文件中
*/
void Candidate::OutputCandidate()
{
	// 根据人数进行遍历
	string InitData;
	ifstream finInitData("F:\\lymph node detection dataset\\DATA\\init.txt");

	string PosFileName;
	ifstream finFile("F:\\lymph node detection dataset\\candidatepos.lst");

	int count = 0;
	// 读取每个人员的初始世界坐标
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
		// 遍历所有的正例pos的候选集文件，当然要求已经生成记录文件
		// 进行正例文件读取

		getline(finFile, PosFileName);
        cout << PosFileName << endl;
		string PosCoordidate;
		ifstream finPos(PosFileName);
		while (getline(finPos, PosCoordidate))
		{
			// cout << PosCoordidate << endl; 每个正例的世界坐标
			
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
				// 生成pos图像路径
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

	}
}


/*
* 由于世界坐标起始位置每个病人都有所不同，所以对应首先需要生成世界坐标标注文件
* 包括：所属人员，起始Sagittal世界坐标，起始Coronal世界坐标，起始Axial世界坐标，移动步长
*/
void Candidate::WorldCoordinateConvert()
{
	//读取每个人的样例dcm文件获取初始信息
	CStatDir statdir;

	char buf[256] = "F:\\lymph node detection dataset\\DOI";
	char filename[] = "F:\\lymph node detection dataset\\DATA\\coordinate.txt";
	ofstream fout(filename);
	//设置要遍历的目录  
	if (!statdir.SetInitDir(buf))
	{
		puts("目录不存在");
		return ;
	}

	//开始遍历  
	vector<string> file_vec = statdir.BeginBrowseFilenames("*000000.dcm");
	for (vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it)
		fout << *it << endl;

	// 读取每个人的初始信息文件以获取初始世界坐标
	string DcmFileName;
	ifstream finFile("F:\\lymph node detection dataset\\DATA\\coordinate.txt");
	ofstream coordinateout("F:\\lymph node detection dataset\\DATA\\init.txt");

	while (getline(finFile, DcmFileName))
	{
		// 实例化dcm解析类
		TDcmFileFormat dcm = TDcmFileFormat(DcmFileName.c_str());
		// 获取dcm图像的ImagePositionPatient
		string result = dcm.getImagePositionPatient();
		vector<string> ImagePosition;
		split(result, "\\", ImagePosition);
		double initx = atof(ImagePosition[0].c_str());
		double inity = atof(ImagePosition[1].c_str());
		double realz = atof(ImagePosition[2].c_str());
		//cout << value0 << " " << value1 << " " << value2 << endl;

		// 获取dcm图像的PixelSpacing
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