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

