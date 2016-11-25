#ifndef __NEGATIVE_SAMPLE_H  
#define __NEGATIVE_SAMPLE_H 

#include <iostream>
#include <fstream>
#include <stdlib.h>                           //srand() and rand() function
#include <time.h>                             //time() function
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2\opencv.hpp>  

using namespace cv;
using namespace std;

class NegativeSample
{
public:
	NegativeSample(string filelist);
	void HandleNegativeSamples();
private:
	string filelist;
	int CropImageCount = 0;        // the amount of the croped of neg samples. 
};

#endif //__NEGATIVE_SAMPLE_H