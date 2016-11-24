#ifndef __OPENCV_TEST_H  
#define __OPENCV_TEST_H  

#include <opencv2\opencv.hpp>  

using namespace cv;

class OpenCVTest
{
public:
	void SetImage(string filename);
	void ShowImage();
private:
	Mat img;
};

#endif //__OPENCV_TEST_H