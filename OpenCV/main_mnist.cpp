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

	//train data & validate data sparated.
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