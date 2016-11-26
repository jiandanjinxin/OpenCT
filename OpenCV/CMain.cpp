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


	//���������  
	CStatDir statdir;

	char buf[256] = "D:\\Computer Vision\\INRIAPerson\\96X160H96";

	//����Ҫ������Ŀ¼  
	if (!statdir.SetInitDir(buf))
	{
		puts("Ŀ¼������");
		return 0;
	}

	//��ʼ����  
	vector<string> file_vec = statdir.BeginBrowseFilenames("*.*");
	for (vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it)
		std::cout << *it << std::endl;

	printf("�ļ�����: %d\n", file_vec.size());

	waitKey();

	return 0;
}


