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
