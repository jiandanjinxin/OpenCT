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