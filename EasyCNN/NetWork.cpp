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
