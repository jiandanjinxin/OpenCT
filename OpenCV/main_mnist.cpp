#include <iostream>
#include <cassert>
#include <algorithm>

#include "EasyCNN.h"
#include "MnistDataLoader.h"

#include <opencv2/opencv.hpp>

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