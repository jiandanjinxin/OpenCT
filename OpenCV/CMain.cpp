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
using namespace std;

#define DirGenerationDcm false     // dcm全部数据的文件目录生成
#define DcmToBmp false             // 是否进行dcm到bmp的格式转换，附：一般性图像格式转换可以调用CxImage

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

	// 对所有dcm文件根据已经生成的file.lst进行全部dcm图像的格式转换
	if (DcmToBmp)
	{
		string ImgName; // image filename, absoluted path
		ifstream finDcm("F:\\lymph node detection dataset\\DOI\\file.lst");  // Dcm图片名列表文件

		int count = 0;

		// 循环遍历所有dcm文件
		while ( getline(finDcm, ImgName) )
		{
			count++;
			// 获取原始dcm图像的前缀目录
			string subImageName = ImgName.substr(0, 50);
			// 将原始dcm图像的string转换为char*
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


