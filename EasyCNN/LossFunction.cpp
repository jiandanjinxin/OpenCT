#include <cmath>
#include "LossFunction.h"

//cross entropy
float EasyCNN::CrossEntropyFunctor::getLoss(const std::shared_ptr<EasyCNN::DataBucket> labelDataBucket, const std::shared_ptr<EasyCNN::DataBucket> outputDataBucket)
{
	const auto outputSize = outputDataBucket->getSize();
	const float* labelData = labelDataBucket->getData().get();
	const float* outputData = outputDataBucket->getData().get();

	float loss = 0.0f;
	for (size_t i = 0; i < outputSize._4DSize(); i++)
	{
		loss -= labelData[i] * std::log(outputData[i]) / outputSize.number;
	}

	return loss;
}
