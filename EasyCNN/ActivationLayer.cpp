#include <algorithm>
#include "ActivationLayer.h"

//Sigmoid Layer
EasyCNN::SigmodLayer::SigmodLayer()
{

}

EasyCNN::SigmodLayer::~SigmodLayer()
{

}

DEFINE_LAYER_TYPE(EasyCNN::SigmodLayer, "SigmodLayer");
std::string EasyCNN::SigmodLayer::getLayerType() const
{
	return layerType;
}

//f(x)=1/(1+e^(-x))
static inline float sigmodOperator(const float x)
{
	float result = 0;
	result = 1.0f / (1.0f + std::exp(-1.0f * x));
	return result;
}

//f'(x) = x(1-x)
static inline float sigmodDfOperator(const float x)
{
	return x * (1.0f - x);
}

//Sigmoid forward
void EasyCNN::SigmodLayer::forward(const std::shared_ptr<DataBucket> prevDataBucket, std::shared_ptr<DataBucket> nextDataBucket)
{
	const DataSize prevDataSize = prevDataBucket->getSize();
	const DataSize nextDataSize = nextDataBucket->getSize();

	const float* prevRawData = prevDataBucket->getData().get();
	float* nextRawData = nextDataBucket->getData().get();

	for (size_t nn = 0; nn < nextDataSize.number; nn++)
	{
		for (size_t nc = 0; nc < nextDataSize.channels; nc++)
		{
			for (size_t nh = 0; nh < nextDataSize.height; nh++)
			{
				for (size_t nw = 0; nw < nextDataSize.width; nw++)
				{
					const size_t nextDataIdx = nextDataSize.getIndex(nn, nc, nh, nw);
					nextRawData[nextDataIdx] = sigmodOperator(prevRawData[nextDataIdx]);
				}
			}
		}
	}
}
//Sigmoid backward
void EasyCNN::SigmodLayer::backward(std::shared_ptr<DataBucket> prevDataBucket, const std::shared_ptr<DataBucket> nextDataBucket, std::shared_ptr<DataBucket>& nextDiffBucket)
{
	easyAssert(getPhase() == Phase::Train, "backward only in train phase.")
	const DataSize prevDataSize = prevDataBucket->getSize();
	const DataSize nextDataSize = nextDataBucket->getSize();
	const DataSize nextDiffSize = nextDiffBucket->getSize();
	const float* prevData = prevDataBucket->getData().get();
	const float* nextData = nextDataBucket->getData().get();
	const float* nextDiff = nextDiffBucket->getData().get();
	easyAssert(prevDataSize == nextDataSize, "size must be equal!");

	//update prevDiff data, initial
	const DataSize prevDiffSize(prevDataSize.number, prevDataSize.channels, prevDataSize.height, prevDataSize.width);
	std::shared_ptr<DataBucket> prevDiffBucket(std::make_shared<DataBucket>(prevDiffSize));
	prevDiffBucket->fillData(0.0f);
	float* prevDiff = prevDiffBucket->getData().get();

	//calculate current inner diff
	for (size_t pn = 0; pn < prevDataSize.number; pn++)
	{
		for (size_t pc = 0; pc < prevDiffSize.channels; pc++)
		{
			for (size_t ph = 0; ph < prevDiffSize.height; ph++)
			{
				for (size_t pw = 0; pw < prevDiffSize.width; pw++)
				{
					const size_t dataIdx = nextDataSize.getIndex(pn, pc, ph, pw);
					const size_t paramIdx = prevDiffSize.getIndex(pn, pc, ph, pw);
					prevDiff[paramIdx] += sigmodDfOperator(nextData[dataIdx]);
				}
			}
		}
	}

	//multiply next diff
	for (size_t i = 0; i < prevDiffBucket->getSize()._4DSize(); i++)
	{
		prevDiff[i] *= nextDiff[i];
	}

	nextDiffBucket = prevDiffBucket;

	//update this layer's param
	//Tanh layer : nop
}

//TanhLayer
EasyCNN::TanhLayer::TanhLayer()
{

}

EasyCNN::TanhLayer::~TanhLayer()
{

}