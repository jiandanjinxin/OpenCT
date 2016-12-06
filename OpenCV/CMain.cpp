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

using namespace THU_STD_NAMESPACE; // 加载DCM格式转换命名空间

#define DirGenerationDcm false // dcm全部数据的文件目录生成
#define DcmToBmp true // 是否进行dcm到bmp的格式转换

int main()
{
	
	/*
	// Negative Samples generation
	NegativeSample negativeSample = NegativeSample("D:\\Computer Vision\\INRIAPerson\\Train\\neg.lst");
	negativeSample.HandleNegativeSamples();
	*/

	// 生成所有dcm文件的目录 F:\\lymph node detection dataset\\DOI\\file.lst
	if (DirGenerationDcm)
	{
		//构造类对象  
		CStatDir statdir;

		char buf[256] = "F:\\lymph node detection dataset\\DOI";
		char posfilename[] = "F:\\lymph node detection dataset\\DOI\\file.lst";
		ofstream fout(posfilename);
		//设置要遍历的目录  
		if (!statdir.SetInitDir(buf))
		{
			puts("目录不存在");
			return 0;
		}

		//开始遍历  
		vector<string> file_vec = statdir.BeginBrowseFilenames("*.*");
		for (vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it)
			fout << *it << endl;

		// printf("文件总数: %d\n", file_vec.size());
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

	// 对应文件夹的所有文件进行遍历
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


