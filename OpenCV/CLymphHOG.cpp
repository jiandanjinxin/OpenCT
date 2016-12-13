#include "HLymphHOG.h"

void LymphHOG::SVMTraining(bool flag)
{  
	// detective window (64, 128), block size (16, 16), block stride (8, 8), cell size (8, 8), nbins 9
	HOGDescriptor hog(Size(32, 32), Size(8, 8), Size(4, 4), Size(4, 4), 9);  // hog descriptor, calculate the hog descriptor.
	int DescriptorDim;                                                       // hog descriptor dimension depend on size of picture, detective window, block size, cell bin.
	HOGSVM svm;                                                              // hog classifier

	// if train is true, retrain the classifier
	if (flag)
	{
		// ����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��
		Mat sampleFeatureMat;
		// ѵ����������������������������������ĸ�������������1:1��ʾ���ˣ�-1��ʾ����
		Mat sampleLabelMat;
	}
}

