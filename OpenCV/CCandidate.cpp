#include "HCandidate.h"

void Candidate::OutputCandidate()
{

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

		coordinateout << initx << " " << inity << " " << initz << " " << pixelstep << endl;

	}
}