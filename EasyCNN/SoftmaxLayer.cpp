#include <algorithm>
#include "SoftmaxLayer.h"

EasyCNN::SoftmaxLayer::SoftmaxLayer()
{

}

EasyCNN::SoftmaxLayer::~SoftmaxLayer()
{

}

DEFINE_LAYER_TYPE(EasyCNN::SoftmaxLayer, "SoftmaxLayer");
std::string EasyCNN::SoftmaxLayer::getLayerType() const
{
	return layerType;
}

//SoftmaxLayer forward
void EasyCNN::SoftmaxLayer::forward(const std::shared_ptr<DataBucket> prevDataBucket, std::shared_ptr<DataBucket> nextDataBucket)
{
	const DataSize prevDataSize = prevDataBucket->getSize();
	const DataSize nextDataSize = nextDataBucket->getSize();

	for (size_t nn = 0; nn < nextDataSize.number; nn++)
	{
		const float* prevData = prevDataBucket->getData().get() + nn * prevDataSize._3DSize();
		float* nextData = nextDataBucket->getData().get() + nn * nextDataSize._3DSize();

		//step1 : find max value
		float maxVal = prevData[0];
		for (size_t prevDataIdx = 0; prevDataIdx < prevDataSize._3DSize(); prevDataIdx++)
		{
			maxVal = std::max(maxVal, prevData[prevDataIdx]);
		}

		//step2 : sum
		float sum = 0;
		for (size_t prevDataIdx = 0; prevDataIdx < prevDataSize._3DSize(); prevDataIdx++)
		{
			nextData[prevDataIdx] = std::exp(prevData[prevDataIdx] - maxVal);
			sum += nextData[prevDataIdx];
		}

		//step3 : div
		for (size_t prevDataIdx = 0; prevDataIdx < prevDataSize._3DSize(); prevDataIdx++)
		{
			nextData[prevDataIdx] = nextData[prevDataIdx] / sum;
		}
	}
}