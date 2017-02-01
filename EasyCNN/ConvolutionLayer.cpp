#include <sstream>
#include "ConvolutionLayer.h"
#include "CommonTools.h"

#if WITH_OPENCV_DEBUG
#include "opencv2/opencv.hpp"
#endif

EasyCNN::ConvolutionLayer::ConvolutionLayer()
{

}

EasyCNN::ConvolutionLayer::~ConvolutionLayer()
{

}

void EasyCNN::ConvolutionLayer::setParamaters(const ParamSize _kernelSize, const size_t _widthStep, const size_t _heightStep, const bool _enabledBias)
{
	easyAssert(_kernelSize.number > 0 && _kernelSize.channels > 0 &&
		_kernelSize.width > 0 && _kernelSize.height > 0 && _widthStep > 0 && _heightStep > 0,
		"kernel size or step is invalidate.");

	kernelSize = _kernelSize;
	widthStep = _widthStep;
	heightStep = _heightStep;
	enabledBias = _enabledBias;
}

std::string EasyCNN::ConvolutionLayer::serializeToString() const
{
	const std::string spliter = " ";
	std::stringstream ss;
	//layer desc
	ss << getLayerType() << spliter
		<< kernelSize.number << spliter << kernelSize.channels << spliter << kernelSize.width << spliter << kernelSize.height << spliter
		<< widthStep << spliter << heightStep << spliter << enabledBias << spliter;
	//weight
	const auto kernel = kernelData->getData().get();
	for (size_t i = 0; i < kernelSize._4DSize(); i++)
	{
		ss << kernel[i] << spliter;
	}
	//bias
	if (enabledBias)
	{
		const auto bias = biasData->getData().get();
		const auto biasSize = biasData->getSize();
		for (size_t i = 0; i < biasSize._4DSize(); i++)
		{
			ss << bias[i] << spliter;
		}
	}
	return ss.str();
}

void EasyCNN::ConvolutionLayer::serializeFromString(const std::string content)
{
	std::stringstream ss(content);
	//layer desc
	std::string _layerType;
	ss >> _layerType
		>> kernelSize.number >> kernelSize.channels >> kernelSize.width >> kernelSize.height
		>> widthStep >> heightStep >> enabledBias;
	easyAssert(_layerType == layerType, "layer type is invalidate.");
	solveInnerParams();
	//weight
	auto kernel = kernelData->getData().get();
	for (size_t i = 0; i < kernelSize._4DSize(); i++)
	{
		ss >> kernel[i];
	}
	//bias
	if (enabledBias)
	{
		const auto bias = biasData->getData().get();
		const auto biasSize = biasData->getSize();
		for (size_t i = 0; i < biasSize._4DSize(); i++)
		{
			ss >> bias[i];
		}
	}
}

DEFINE_LAYER_TYPE(EasyCNN::ConvolutionLayer, "ConvolutionLayer");
