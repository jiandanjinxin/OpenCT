#include "HNegativeSample.h"

NegativeSample::NegativeSample(string file)
{
	filelist = file;
}

void NegativeSample::HandleNegativeSamples()
{
	Mat src;
	string ImgName;

	char saveName[256];                                                                 // file name of the croped neg samples pic.
	ifstream fin("D:\\Computer Vision\\INRIAPerson\\Train\\neg.lst");                   // open neg list, it already included by the dataset. 

	while (getline(fin, ImgName))
	{
		// cout << "processing :" << ImgName << endl;                                   // read the neg samples by line.	
		ImgName = "D:\\Computer Vision\\INRIAPerson\\" + ImgName;                       // construct the absoluted path of neg files.

		src = imread(ImgName, 1);                                                       //read image by opencv function imread 3 channels

		// the size of pic should contain a 64 * 128 window at least.
		if (src.cols >= 64 && src.rows >= 128)
		{
			srand( (unsigned)time(NULL) ); // set random seed

			// Random crop 10 which is size of 64 * 128 samples from the picture of neg samples.
			for (int i = 0; i < 10; i++)
			{
				int x = (rand() % (src.cols - 64));    // x coordinate in top left corner.
				int y = (rand() % (src.rows - 128));   // y coordinate in top left corner.
				// cout << x << ", " << y << ", " << src.cols - 64 << ", " << src.rows - 128 << endl;

				Mat imgROI = src(Rect(x, y, 64, 128)); // region of interest being cropped.
				// generate filename of cropped picture.
				sprintf_s(saveName, "D:\\Computer Vision\\INRIAPerson\\Train\\negcrop\\noperson%06d.jpg", ++CropImageCount);
				imwrite(saveName, imgROI);             // save file
			}
		}
	}
}
