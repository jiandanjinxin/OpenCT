#include <opencv2\opencv.hpp>  
#include <iostream>  
#include <string>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("opencv.jpg");

	if (img.empty())
	{
		cout << "cannot found the picture in current directory.";
		return 0;
	}

	imshow("imageshow", img);

	waitKey(); // Wait for a keystroke in the window

	return 0;
}