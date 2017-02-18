#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
//configure
#include "Configure.h"
//layers
#include "Layer.h"
#include "ActivationLayer.h"
#include "InputLayer.h"
#include "ConvolutionLayer.h"
#include "PoolingLayer.h"
#include "FullconnectLayer.h"
#include "SoftmaxLayer.h"
//network
#include "NetWork.h"

EasyCNN::NetWork::NetWork()
{
	logVerbose("NetWork constructed.");
}

EasyCNN::NetWork::~NetWork()
{
	logVerbose("NetWork destructed.");
}

void EasyCNN::NetWork::setPhase(Phase phase)
{
	logVerbose("NetWork setPhase begin.");
	this->phase = phase;
	logVerbose("NetWork setPhase end.");
}

// common
EasyCNN::Phase EasyCNN::NetWork::getPhase() const
{
	return phase;
}

std::string EasyCNN::NetWork::serializeToString() const
{
	const std::string spliter = " ";
	std::stringstream ss;
	const auto inputSize = dataBuckets[0]->getSize();
	ss << inputSize.channels << spliter << inputSize.width << spliter << inputSize.height << spliter;
	for (const auto& layer : layers)
	{
		ss << layer->getLayerType() << spliter;
	}
	return ss.str();
}

std::shared_ptr<EasyCNN::Layer> EasyCNN::NetWork::createLayerByType(const std::string layerType)
{
	if (layerType == InputLayer::layerType)
	{
		return std::make_shared<InputLayer>();
	}
	else if (layerType == ConvolutionLayer::layerType)
	{
		return std::make_shared<ConvolutionLayer>();
	}
	else if (layerType == PoolingLayer::layerType)
	{
		return std::make_shared<PoolingLayer>();
	}
	else if (layerType == FullconnectLayer::layerType)
	{
		return std::make_shared<FullconnectLayer>();
	}
	else if (layerType == SoftmaxLayer::layerType)
	{
		return std::make_shared<SoftmaxLayer>();
	}
	else if (layerType == SigmodLayer::layerType)
	{
		return std::make_shared<SigmodLayer>();
	}
	else if (layerType == TanhLayer::layerType)
	{
		return std::make_shared<TanhLayer>();
	}
	else if (layerType == ReluLayer::layerType)
	{
		return std::make_shared<ReluLayer>();
	}
	else
	{
		logVerbose("layer type : %s", layerType.c_str());
		easyAssert(false, "can't goto here.");
		return nullptr;
	}
}

std::vector<std::shared_ptr<EasyCNN::Layer>> EasyCNN::NetWork::serializeFromString(const std::string content)
{
	int number = 1;
	int channels = 0;
	int width = 0;
	int height = 0;
	std::stringstream ss(content);
	ss >> channels >> width >> height;
	setInputSize(DataSize(number, channels, width, height));
	std::vector<std::shared_ptr<EasyCNN::Layer>> tmpLayers;
	while (!ss.eof())
	{
		std::string layerType;
		ss >> layerType;
		if (layerType.empty())
		{
			continue;
		}
		std::shared_ptr<EasyCNN::Layer> layer = createLayerByType(layerType);
		easyAssert(layer.get() != nullptr, "layer can't be null.");
		tmpLayers.push_back(layer);
	}
	return tmpLayers;
}

std::shared_ptr<EasyCNN::DataBucket> EasyCNN::NetWork::forward(const std::shared_ptr<DataBucket> inputDataBucket)
{
	logVerbose("NetWork forward begin.");
	easyAssert(layers.size() > 1, "layer count is less than 2.");
	easyAssert(layers[0]->getLayerType() == InputLayer::layerType, "first layer is not input layer.");
	easyAssert(dataBuckets.size() > 0, "data buckets is not ready.");

	//copy data from inputDataBucket
	//reshape data bucket
	const auto oldNumber = dataBuckets[0]->getSize().number;
	const auto newNumber = inputDataBucket->getSize().number;

	if (newNumber != oldNumber)
	{
		for (size_t i = 0; i < dataBuckets.size(); i++)
		{
			auto newSize = dataBuckets[i]->getSize();
			newSize.number = newNumber;
			dataBuckets[i].reset(new DataBucket(newSize));
		}
	}

	inputDataBucket->cloneTo(*dataBuckets[0]);

	for (size_t i = 0; i < layers.size(); i++)
	{
		logVerbose("NetWork layer[%d](%s) forward begin.", i, layers[i]->getLayerType().c_str());
		layers[i]->forward(dataBuckets[i], dataBuckets[i + 1]);
		logVerbose("NetWork layer[%d](%s) forward end.", i, layers[i]->getLayerType().c_str());
	}

	logVerbose("NetWork forward end.");
	return dataBuckets[dataBuckets.size() - 1];
}
