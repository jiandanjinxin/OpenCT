#include "HTest.h"

void OpenCVTest::SetImage(string filename)
{
	img = imread(filename);
}

void OpenCVTest::ShowImage()
{
	imshow("imagetest", img);
}