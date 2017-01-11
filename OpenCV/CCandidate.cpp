#include "HCandidate.h"

void Candidate::OutputCandidate()
{

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

		coordinateout << initx << " " << inity << " " << initz << " " << pixelstep << endl;

	}
}