#include <algorithm>
#include <sstream>
#include "PoolingLayer.h"

#if WITH_OPENCV_DEBUG
#include "opencv2/opencv.hpp"
#endif

EasyCNN::PoolingLayer::PoolingLayer()
{

}

EasyCNN::PoolingLayer::~PoolingLayer()
{

}

void EasyCNN::PoolingLayer::setParamaters(const PoolingType _poolingType, const ParamSize _poolingKernelSize, const size_t _widthStep, const size_t _heightStep)
{
	easyAssert(_poolingKernelSize.number == 1 && _poolingKernelSize.channels > 0 && _poolingKernelSize.width > 1 && _poolingKernelSize.height > 1 && _widthStep > 0 && _heightStep > 0,
		"parameters invalidate.");
	poolingKernelSize = _poolingKernelSize;
	poolingType = _poolingType;
	widthStep = _widthStep;
	heightStep = _heightStep;
}