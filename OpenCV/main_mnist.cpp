#include <iostream>
#include <cassert>
#include <algorithm>

#include "EasyCNN.h"
#include "MnistDataLoader.h"

#include <opencv2/opencv.hpp>

const int classes = 10;

static EasyCNN::NetWork buildConvNet(const size_t batch, const size_t channels, const size_t width, const size_t height)
{
	EasyCNN::NetWork network;
	network.setPhase(EasyCNN::Phase::Train);
	network.setInputSize(EasyCNN::DataSize(batch, channels, width, height));
	network.setLossFunctor(std::make_shared<EasyCNN::CrossEntropyFunctor>());

	//input data layer 0
	std::shared_ptr<EasyCNN::InputLayer> _0_inputLayer(std::make_shared<EasyCNN::InputLayer>());
	network.addLayer(_0_inputLayer);

	//convolution layer 1
	std::shared_ptr<EasyCNN::ConvolutionLayer> _1_convLayer(std::make_shared<EasyCNN::ConvolutionLayer>());
	_1_convLayer->setParamaters(EasyCNN::ParamSize(6, 1, 5, 5), 1, 1, true);
	network.addLayer(_1_convLayer);
	network.addLayer(std::make_shared<EasyCNN::ReluLayer>());

	//pooling layer 2
	std::shared_ptr<EasyCNN::PoolingLayer> _2_poolingLayer(std::make_shared<EasyCNN::PoolingLayer>());
	_2_poolingLayer->setParamaters(EasyCNN::PoolingLayer::PoolingType::MaxPooling, EasyCNN::ParamSize(1, 6, 2, 2), 2, 2);
	network.addLayer(_2_poolingLayer);
	network.addLayer(std::make_shared<EasyCNN::ReluLayer>());

	//convolution layer 3
	std::shared_ptr<EasyCNN::ConvolutionLayer> _3_convLayer(std::make_shared<EasyCNN::ConvolutionLayer>());
	_3_convLayer->setParamaters(EasyCNN::ParamSize(16, 6, 5, 5), 1, 1, true);
	network.addLayer(_3_convLayer);
	network.addLayer(std::make_shared<EasyCNN::ReluLayer>());

	//pooling layer 4
	std::shared_ptr<EasyCNN::PoolingLayer> _4_pooingLayer(std::make_shared<EasyCNN::PoolingLayer>());
	_4_pooingLayer->setParamaters(EasyCNN::PoolingLayer::PoolingType::MaxPooling, EasyCNN::ParamSize(1, 16, 2, 2), 2, 2);
	network.addLayer(_4_pooingLayer);
	network.addLayer(std::make_shared<EasyCNN::ReluLayer>());

	//full connect layer 5
	std::shared_ptr<EasyCNN::FullconnectLayer> _5_fullconnectLayer(std::make_shared<EasyCNN::FullconnectLayer>());
	_5_fullconnectLayer->setParamaters(EasyCNN::ParamSize(1, 512, 1, 1), true);
	network.addLayer(_5_fullconnectLayer);
	network.addLayer(std::make_shared<EasyCNN::ReluLayer>());

	//full connect layer 6
	std::shared_ptr<EasyCNN::FullconnectLayer> _6_fullconnectLayer(std::make_shared<EasyCNN::FullconnectLayer>());
	_6_fullconnectLayer->setParamaters(EasyCNN::ParamSize(1, classes, 1, 1), true);
	network.addLayer(_6_fullconnectLayer);
	network.addLayer(std::make_shared<EasyCNN::ReluLayer>());

	//soft max layer 6
	std::shared_ptr<EasyCNN::SoftmaxLayer> _7_softmaxLayer(std::make_shared<EasyCNN::SoftmaxLayer>());
	network.addLayer(_7_softmaxLayer);

	return network;
}

//image shuffle using random_shuffle in algorithm
static void shuffle_data(std::vector<image_t>& images, std::vector<label_t>& labels)
{
	assert(images.size() == labels.size());
	std::vector<size_t> indexArray;
	for (size_t i = 0; i < images.size(); i++)
	{
		indexArray.push_back(i);
	}
	std::random_shuffle(indexArray.begin(), indexArray.end());

	std::vector<image_t> tmpImages(images.size());
	std::vector<label_t> tmpLabels(labels.size());
	for (size_t i = 0; i < images.size(); i++)
	{
		const size_t srcIndex = i;
		const size_t dstIndex = indexArray[i];
		tmpImages[srcIndex] = images[dstIndex];
		tmpLabels[srcIndex] = labels[dstIndex];
	}
	images = tmpImages;
	labels = tmpLabels;
}


static void train(const std::string& mnist_train_images_file, const std::string& mnist_train_labels_file)
{
	bool success = false;

	EasyCNN::setLogLevel(EasyCNN::EASYCNN_LOG_LEVEL_CRITICAL);

	//load train images
	EasyCNN::logCritical("loading training data...");
	std::vector<image_t> images;
	success = load_mnist_images(mnist_train_images_file, images);
	assert(success && images.size() > 0);
	//load train labels
	std::vector<label_t> labels;
	success = load_mnist_labels(mnist_train_labels_file, labels);
	assert(success && labels.size() > 0);
	assert(images.size() == labels.size());

	shuffle_data(images, labels);

	//train data & validate data sparated.3:1
	//train
	std::vector<image_t> train_images(static_cast<size_t>(images.size() * 0.75f));
	std::vector<label_t> train_labels(static_cast<size_t>(labels.size() * 0.75f));
	std::copy(images.begin(), images.begin() + train_images.size(), train_images.begin());
	std::copy(labels.begin(), labels.begin() + train_labels.size(), train_labels.begin());
	//validate
	std::vector<image_t> validate_images(images.size() - train_images.size());
	std::vector<label_t> validate_labels(labels.size() - train_labels.size());
	std::copy(images.begin() + train_images.size(), images.end(), validate_images.begin());
	std::copy(labels.begin() + train_labels.size(), labels.end(), validate_labels.begin());
	EasyCNN::logCritical("load training data done. train set's size is %d,validate set's size is %d", train_images.size(), validate_images.size());

	//configuration
	float learningRate = 0.1f;
	const float decayRate = 0.001f;
	const float minLearningRate = 0.001f;

	const size_t testAfterBatches = 200;
	const size_t maxBatches = 10000;
	const size_t max_epoch = 4;
	const size_t batch = 16;
	const size_t channels = images[0].channels;
	const size_t width = images[0].width;
	const size_t height = images[0].height;

	EasyCNN::logCritical("max_epoch:%d, testAfterBatches:%d", max_epoch, testAfterBatches);
	EasyCNN::logCritical("learningRate:%f ,decayRate:%f , minLearningRate:%f", learningRate, decayRate, minLearningRate);
	EasyCNN::logCritical("channels:%d , width:%d , height:%d", channels, width, height);

	/*easycnn::logcritical("construct network begin...");
	easycnn::network network(buildconvnet(batch, channels, width, height));
	easycnn::logcritical("construct network done.");*/


}



int mnist_main(int argc, char* argv[])
{
	//mnist_date file path
	const std::string mnist_train_images_file = "mnist_data/train-images.idx3-ubyte";
	const std::string mnist_train_labels_file = "mnist_data/train-labels.idx1-ubyte";
	train(mnist_train_images_file, mnist_train_labels_file);
	system("pause");

	return 0;
}