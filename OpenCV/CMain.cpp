#include "HNegativeSample.h"
#include "HUtil.h"
#include "HDcmFileFormat.h"
#include "HBmpConvert.h"

#include "ximage.h"

#include "dcmtk/config/osconfig.h"  
#include "dcmtk/dcmdata/dctk.h"  
#include "dcmtk/dcmdata/dcpxitem.h"  
#include "dcmtk/dcmjpeg/djdecode.h"  
#include "dcmtk/dcmjpeg/djencode.h" 
#include "dcmtk/dcmjpeg/djcodece.h"  
#include "dcmtk/dcmjpeg/djrplol.h"  
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcvrfd.h"
#include "dcmtk/dcmdata/dcistrmf.h"

using namespace THU_STD_NAMESPACE; // ����DCM��ʽת�������ռ�
using namespace std;

#define DirGenerationDcm false     // dcmȫ�����ݵ��ļ�Ŀ¼����
#define DcmToBmp false             // �Ƿ����dcm��bmp�ĸ�ʽת��������һ����ͼ���ʽת�����Ե���CxImage

int main()
{
	
	/*
	// Negative Samples generation
	NegativeSample negativeSample = NegativeSample("D:\\Computer Vision\\INRIAPerson\\Train\\neg.lst");
	negativeSample.HandleNegativeSamples();
	*/

	// ��������dcm�ļ���Ŀ¼ F:\\lymph node detection dataset\\DOI\\file.lst
	if (DirGenerationDcm)
	{
		//���������  
		CStatDir statdir;

		char buf[256] = "F:\\lymph node detection dataset\\DOI";
		char posfilename[] = "F:\\lymph node detection dataset\\DOI\\file.lst";
		ofstream fout(posfilename);
		//����Ҫ������Ŀ¼  
		if (!statdir.SetInitDir(buf))
		{
			puts("Ŀ¼������");
			return 0;
		}

		//��ʼ����  
		vector<string> file_vec = statdir.BeginBrowseFilenames("*.*");
		for (vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it)
			fout << *it << endl;

		// printf("�ļ�����: %d\n", file_vec.size());
	}

	// ������dcm�ļ������Ѿ����ɵ�file.lst����ȫ��dcmͼ��ĸ�ʽת��
	if (DcmToBmp)
	{
		string ImgName; // image filename, absoluted path
		ifstream finDcm("F:\\lymph node detection dataset\\DOI\\file.lst");  // DcmͼƬ���б��ļ�

		int count = 0;

		// ѭ����������dcm�ļ�
		while ( getline(finDcm, ImgName) )
		{
			count++; // ��¼ת�����ļ�����

			// ��ȡԭʼdcmͼ���ǰ׺Ŀ¼
			string subImageName = ImgName.substr(0, 50);

			// ��ԭʼdcmͼ���stringת��Ϊchar*
			const char *chImageName = ImgName.c_str();
			// ��ȡԭʼdcmͼ���ǰ׺Ŀ¼��stringת��Ϊchar*
			const char *chSubImageName = subImageName.c_str();
			// ����dcmtkʵ����
			TDcmFileFormat dcm = TDcmFileFormat(chImageName);
			// ��ȡdcmͼ���InstanceNumber
			int InstancePosition = dcm.getPositionNumber();
			// ����bmpͼ��洢·��
			char BmpName[256];
			sprintf_s(BmpName, "%s%06d.bmp", chSubImageName, InstancePosition);
			// ����ͼ��ת��
			dcm.setWindow(715, 3478);
			dcm.saveToBmp(BmpName);

			cout << count << endl;
		}
	}

	// ͼ���Ƭ����
	BmpConvert bmpsample = BmpConvert("F:\\lymph node detection dataset\\DOI\\ABD_LYMPH_001\\000001.bmp");
	bmpsample.CroppedEdge();




	system("pause");

	return 0;
}


