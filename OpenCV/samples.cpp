#include <iostream>
#include <fstream>
#include <stdlib.h>                           //srand() and rand() function
#include <time.h>                             //time() function
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

int CropImageCount = 0;                        // the amount of the croped of neg samples. 

int main()
{
	Mat src;
	string ImgName;

	char saveName[256];                                                                 // file name of the croped neg samples pic.
	ifstream fin("D:\\Computer Vision\\INRIAPerson\\Train\\neg.lst");                   // open neg list, it already included by the dataset.


	while (getline(fin, ImgName))
	{
		cout << "processing :" << ImgName << endl;                                      // read the neg samples by line.
		
		ImgName = "D:\\Computer Vision\\INRIAPerson\\" + ImgName;                       // construct the absoluted path of neg files.

		//src = imread(ImgName, 1);//读取图片


		////src =cvLoadImage(imagename,1);
		////cout<<"宽："<<src.cols<<"，高："<<src.rows<<endl;

		////图片大小应该能能至少包含一个64*128的窗口
		//if (src.cols >= 64 && src.rows >= 128)
		//{
		//	srand(time(NULL));//设置随机数种子

		//	//从每张图片中随机裁剪10个64*128大小的不包含人的负样本
		//	for (int i = 0; i<10; i++)
		//	{
		//		int x = (rand() % (src.cols - 64)); //左上角x坐标
		//		int y = (rand() % (src.rows - 128)); //左上角y坐标
		//		//cout<<x<<","<<y<<endl;
		//		Mat imgROI = src(Rect(x, y, 64, 128));
		//		sprintf(saveName, "E:\\运动目标检测\\INRIAPerson\\negphoto\\noperson%06d.jpg", ++CropImageCount);//生成裁剪出的负样本图片的文件名
		//		imwrite(saveName, imgROI);//保存文件
		//	}
		//}
	}

	system("pause");
}