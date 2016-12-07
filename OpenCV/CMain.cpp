#include "HNegativeSample.h"
#include "HUtil.h"
#include "HDcmFileFormat.h"
#include "HDisplayBMP.h"

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
			count++;
			// ��ȡԭʼdcmͼ���ǰ׺Ŀ¼
			string subImageName = ImgName.substr(0, 50);
			// ��ԭʼdcmͼ���stringת��Ϊchar*
			const char *chImageName = ImgName.c_str();

			const char *chSubImageName = subImageName.c_str();

			TDcmFileFormat dcm = TDcmFileFormat(chImageName);
			int InstancePosition = dcm.getPositionNumber();

			char BmpName[256];

			sprintf_s(BmpName, "%s%06d.bmp", chSubImageName, InstancePosition);
			
			dcm.setWindow(715, 3478);
			dcm.saveToBmp(BmpName);

			cout << count << endl;
		}
	}

	/*
	DcmFileFormat fileformat;
	OFCondition oc = fileformat.loadFile("000001.dcm");
	DcmDataset *pDataset = fileformat.getDataset();
	char query[1000];
	memset(query, 0, sizeof(char)* 1000);

	const char *tString;
	pDataset->findAndGetString(DCM_InstanceNumber, tString);
	int i = atoi(tString);

	cout << i << endl;
	*/



	system("pause");
	return 0;
}


