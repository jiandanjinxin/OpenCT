#include "HDisplayBMP.h"

BMPFileFormat::BMPFileFormat()
{
	image = NULL;
}

BMPFileFormat::BMPFileFormat(const char *BMPFileName)
{
	image = imread(BMPFileName);
}

void BMPFileFormat::ThresholdToDCM()
{
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			
			cout << (int)image.at<uchar>(i, j) << " ";
		}
		cout << endl;
	}
}