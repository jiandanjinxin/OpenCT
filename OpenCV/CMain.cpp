#include "HUtil.h"
#include "HDcmFileFormat.h"
#include "HBmpConvert.h"
#include "HLymphHOG.h"

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
#define DirGenerationPixel false   // dcm真实样本像素坐标文件目录生成
#define DcmToBmp false             // 是否进行dcm到bmp的格式转换，附：一般性图像格式转换可以调用CxImage
#define PixelRecords false         // 对所有真实像素坐标进行遍历

// 字符串split
void split(const string& src, const string& separator, vector<string>& dest)
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
			count++; // 记录转换的文件数量

			// 获取原始dcm图像的前缀目录
			string subImageName = ImgName.substr(0, 50);

			// 将原始dcm图像的string转换为char*
			const char *chImageName = ImgName.c_str();
			// 获取原始dcm图像的前缀目录的string转换为char*
			const char *chSubImageName = subImageName.c_str();
			// 基于dcmtk实现类
			TDcmFileFormat dcm = TDcmFileFormat(chImageName);
			// 获取dcm图像的InstanceNumber
			int InstancePosition = dcm.getPositionNumber();
			// 生成bmp图像存储路径
			char BmpName[256];
			sprintf_s(BmpName, "%s%06d.bmp", chSubImageName, InstancePosition);
			// 进行图像转换
			dcm.setWindow(715, 3478);
			dcm.saveToBmp(BmpName);

			cout << count << endl;
		}
	}

	// 生成所有dcm文件真实像素坐标的目录 F:\\lymph node detection dataset\\pixelfile.lst
	if (DirGenerationPixel)
	{
		//构造类对象  
		CStatDir statdir;

		char buf[256] = "F:\\lymph node detection dataset\\MED_ABD_LYMPH_ANNOTATIONS\\MED_ABD_LYMPH_ANNOTATIONS";
		char posfilename[] = "F:\\lymph node detection dataset\\pixelfile.lst";
		ofstream fout(posfilename);
		//设置要遍历的目录  
		if (!statdir.SetInitDir(buf))
		{
			puts("目录不存在");
			return 0;
		}

		//开始遍历  
		vector<string> file_vec = statdir.BeginBrowseFilenames("*indices.txt");
		for (vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it)
			fout << *it << endl;

		printf("文件总数: %d\n", file_vec.size());  // 176
	}

	// 对所有真实label标注进行桶排序标注圈定范围
	if (PixelRecords)
	{
		string PixelFileName; 
		ifstream finPixelFile("F:\\lymph node detection dataset\\pixelfile.lst");  

		int pixelRangeX[512] = { 0 };
		int pixelRangeY[512] = { 0 };
		int pixelRangeZ[662] = { 0 };

		// 循环遍历所有PixelFile文件
		while (getline(finPixelFile, PixelFileName))
		{
			string Pixel;
			ifstream finPixel(PixelFileName);

			// 循环遍历像素坐标
			while (getline(finPixel, Pixel))
			{
				
				//cout << Pixel << endl;
				vector<string> IntPixel;
				::split(Pixel, " ", IntPixel);
				//cout << IntPixel[0] << ", " << IntPixel[1] << ", " << IntPixel[2] << endl;
				pixelRangeX[atoi(IntPixel[0].c_str())]++;
				pixelRangeY[atoi(IntPixel[1].c_str())]++;
				pixelRangeZ[atoi(IntPixel[2].c_str())]++;
			}
		}
		
		// 查看像素分布
		for (int i = 0; i < 512; i++)
			cout << i << ": " << pixelRangeX[i] << endl;

		for (int i = 0; i < 512; i++)
			cout << i << ": " << pixelRangeY[i] << endl;

		for (int i = 0; i < 662 ; i++)
			cout << i << ": " << pixelRangeZ[i] << endl;
			
	}

	/*
	// 图像分片测试
	BmpConvert bmpsample = BmpConvert("F:\\lymph node detection dataset\\DOI\\ABD_LYMPH_001\\000367.bmp");
	bmpsample.CroppedEdge(128, 96, 224, 224);
	bmpsample.InfoShow();
	vector<Mat> result = bmpsample.SegmentationBottom( cvSize(9, 9) );
	bmpsample.MultiImageShow("Multiple Images", result, cvSize(9, 9), cvSize(400, 280));
	*/
	//vector<Mat> result = bmpsample.SegmentationTop(cvSize(8, 8));
	//bmpsample.MultiImageShow("Multiple Images", result, cvSize(8, 8), cvSize(400, 280));
	
	  
	// HoG图像训练
	LymphHOG lymphhog = LymphHOG();
	lymphhog.LabelOutput(true);
	lymphhog.SVMTraining(true);

	system("pause");

	return 0;
}


