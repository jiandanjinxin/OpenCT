/***
*
*   opencv.cpp - test opencv lib
*
*   Purpose:
*
*       This file test your opencv lib is successful linked or not.
*       If it works, you will be observe a picture.
*
****/


#include "HTest.h"
#include "HNegativeSample.h"
#include "HUtil.h"

#include "ximage.h"

#include "dcmtk/config/osconfig.h"  
#include "dcmtk/dcmdata/dctk.h"  
#include "dcmtk/dcmdata/dcpxitem.h"  
#include "dcmtk/dcmjpeg/djdecode.h"  
#include "dcmtk/dcmjpeg/djencode.h"  
#include "dcmtk/dcmjpeg/djcodece.h"  
#include "dcmtk/dcmjpeg/djrplol.h"  

#include "TDcmFileFormat.h"

using namespace THU_STD_NAMESPACE;

int main()
{
	
	/*
	// Test OpenCV
	OpenCVTest openCVTest;
	openCVTest.SetImage("opencv.jpg");
	openCVTest.ShowImage();

	waitKey();
	*/
	/*
	// Negative Samples generation
	NegativeSample negativeSample = NegativeSample("D:\\Computer Vision\\INRIAPerson\\Train\\neg.lst");
	negativeSample.HandleNegativeSamples();
	*/

	/*
	//构造类对象  
	CStatDir statdir;

	char buf[256] = "D:\\Computer Vision\\INRIAPerson\\Train\\negcrop";
	char posfilename[] = "D:\\Computer Vision\\INRIAPerson\\96X160H96\\Train\\neg.lst";
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

	waitKey();
	*/

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
	TDcmFileFormat dcm = TDcmFileFormat("000001.dcm");
	dcm.saveToBmp("000001.bmp");

	

	return 0;
}


