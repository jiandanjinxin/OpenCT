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

#define DirGenerationDcm false // dcmȫ�����ݵ��ļ�Ŀ¼����
#define DcmToBmp true // �Ƿ����dcm��bmp�ĸ�ʽת��

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


	

	/*
	CxImage  image;
	// bmp -> jpg   

	char sourceImg[10] = "00000.png";

	TCHAR Img[100];
	TCHAR convertImg1[] = _T("iamge.jpg");
    #ifdef UNICODE  
	MultiByteToWideChar(CP_ACP, 0, sourceImg, -1, Img, 100);
    #else  
	strcpy(Name, strUsr);
    #endif  

	image.Load(Img, CXIMAGE_FORMAT_PNG);
	if (image.IsValid())
	{
		image.SetJpegQuality(80);
		image.Save(convertImg1, CXIMAGE_FORMAT_JPG);
	}
	
	*/

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

	// ��Ӧ�ļ��е������ļ����б���
	if (DcmToBmp)
	{
		
	}

	//TDcmFileFormat dcm = TDcmFileFormat("000001.dcm");
	//dcm.setWindow(715, 3478);
	//dcm.saveToBmp("000001.bmp");

	/*TDcmFileFormat dcm = TDcmFileFormat("000001.dcm");
	dcm.setWindow(715, 3478);
	dcm.saveToBmp("000001.bmp");
	int number = dcm.getPositionNumber();
	cout << number << endl;

	system("pause");*/
	return 0;
}


