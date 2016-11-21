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

		//src = imread(ImgName, 1);//��ȡͼƬ


		////src =cvLoadImage(imagename,1);
		////cout<<"��"<<src.cols<<"���ߣ�"<<src.rows<<endl;

		////ͼƬ��СӦ���������ٰ���һ��64*128�Ĵ���
		//if (src.cols >= 64 && src.rows >= 128)
		//{
		//	srand(time(NULL));//�������������

		//	//��ÿ��ͼƬ������ü�10��64*128��С�Ĳ������˵ĸ�����
		//	for (int i = 0; i<10; i++)
		//	{
		//		int x = (rand() % (src.cols - 64)); //���Ͻ�x����
		//		int y = (rand() % (src.rows - 128)); //���Ͻ�y����
		//		//cout<<x<<","<<y<<endl;
		//		Mat imgROI = src(Rect(x, y, 64, 128));
		//		sprintf(saveName, "E:\\�˶�Ŀ����\\INRIAPerson\\negphoto\\noperson%06d.jpg", ++CropImageCount);//���ɲü����ĸ�����ͼƬ���ļ���
		//		imwrite(saveName, imgROI);//�����ļ�
		//	}
		//}
	}

	system("pause");
}