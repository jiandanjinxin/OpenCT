#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

#define PosSamNO 2400     // amount of pos samples 
#define NegSamNO 12000    // amount of neg samples

#define TRAIN true        // need training or not, true means training, false means read SVM model form xml file.
#define CENTRAL_CROP true // true:aim to crop the human size of 64 * 128 from INRIA pos samples which is size of 96 * 160 when training.

// HardExample£ºamount of neg samples, if HardExampleNO over 0, means after handling the inital neg dataset, continue to handle the HardExample neg samples.
// HardExample must set to 0 when don't use this. When init dimension of the matrix of feature vector and the matrix of feature class, is will be used.
#define HardExampleNO 0

// Extends from CvSVM, when generate detection parameters in setSVMDetector(), we need to use the trained parameters in SVM's decision_func,
// the parameters of decision_func is type of protected, we cannot visit it directly, only to extend it and using function to visit it.
class HoGSVM : public CvSVM
{
public:
	// get alpha array in SVM decision function.
	double* get_alpha_vector()
	{
		return this->decision_func->alpha;
	}

	// get rho parameter in SVM decision function, which is offset.
	float get_rho()
	{
		return this->decision_func->rho;
	}
};

//int main()
//{
//	// detective window (64, 128), block size (16, 16), block stride (8, 8), cell size (8, 8), nbins 9  
//	hogdescriptor hog(size(64, 128), size(16, 16), size(8, 8), size(8, 8), 9);  // hog descriptor, calculate the hog descriptor.
//	int descriptordim;                                                          // hog descriptor dimension depend on size of picture, detective window, block size, cell bin.
//	hogsvm svm;                                                                 // hog classifier
//
//	// if train is true, retrain the classifier
//	if (train)
//	{
//		string imgname; // image filename, absoluted path
//		ifstream finpos("")
//	}
//
//	system("pause");
//}