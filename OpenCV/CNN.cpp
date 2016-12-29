#include "CNN.h"

#include <assert.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <Windows.h>
#include <random>
#include <algorithm>
#include <string>

namespace CNN {

CNN::CNN()
{
	data_input_train = NULL;
	data_output_train = NULL;
	data_input_test = NULL;
	data_output_test = NULL;
	data_single_image = NULL;
	data_single_label = NULL;

	E_weight_C5 = NULL;
	E_bias_C5 = NULL;
	E_weight_output = NULL;
	E_bias_output = NULL;
}

CNN::~CNN()
{
	release();
}

void CNN::release()
{
	if (data_input_train)
	{
		delete[] data_input_train;
		data_input_train = NULL;
	}
	if (data_output_train)
	{
		delete[] data_output_train;
		data_output_train = NULL;
	}
	if (data_input_test)
	{
		delete[] data_input_test;
		data_input_test = NULL;
	}
	if (data_output_test)
	{
		delete[] data_output_test;
		data_output_test = NULL;
	}
	if (data_single_image)
	{
		delete[] data_single_image;
		data_single_image = NULL;
	}
	if (data_single_label)
	{
		delete[] data_single_label;
		data_single_label = NULL;
	}
}

// connection table [Y.Lecun, 1998 Table 1]
#define O true
#define X false
static const bool tb1[6][16] = {
	O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
	O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
	O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
	X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
	X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
	X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
};
#undef O
#undef X

void CNN::init_variable(double *val, double c, int len)
{
	for (int i = 0; i < len; i++)
		val[i] = c;
}

void CNN::init()
{
	int len_input_train = WIDTH_IMAGE_INPUT_CNN * HEIGHT_IMAGE_INPUT_CNN * NUM_PATTERNS_TRAIN_CNN;
	data_input_train = new double[len_input_train];
	init_variable(data_input_train, -1.0, len_input_train);

	int len_output_train = NUM_MAP_OUTPUT_CNN * NUM_PATTERNS_TRAIN_CNN;
	data_output_train = new double[len_output_train];
	init_variable(data_output_train, -0.8, len_output_train);

	int len_input_test = WIDTH_IMAGE_INPUT_CNN * HEIGHT_IMAGE_INPUT_CNN * NUM_PATTERNS_TEST_CNN;
	data_input_test = new double[len_input_test];
	init_variable(data_input_test, -1.0, len_input_test);

	int len_output_test = NUM_MAP_OUTPUT_CNN * NUM_PATTERNS_TEST_CNN;
	data_output_test = new double[len_output_test];
	init_variable(data_output_test, -0.8, len_output_test);

	std::fill(E_weight_C1, E_weight_C1 + LEN_WEIGHT_C1_CNN, 0.0);
	std::fill(E_bias_C1, E_bias_C1 + LEN_BIAS_C1_CNN, 0.0);
	std::fill(E_weight_S2, E_weight_S2 + LEN_WEIGHT_S2_CNN, 0.0);
	std::fill(E_bias_S2, E_bias_S2 + LEN_BIAS_S2_CNN, 0.0);
	std::fill(E_weight_C3, E_weight_C3 + LEN_WEIGHT_C3_CNN, 0.0);
	std::fill(E_bias_C3, E_bias_C3 + LEN_BIAS_C3_CNN, 0.0);
	std::fill(E_weight_S4, E_weight_S4 + LEN_WEIGHT_S4_CNN, 0.0);
	std::fill(E_bias_S4, E_bias_S4 + LEN_BIAS_S4_CNN, 0.0);
    
	E_weight_C5 = new double[LEN_WEIGHT_C5_CNN];
	std::fill(E_weight_C5, E_weight_C5 + LEN_WEIGHT_C5_CNN, 0.0);
	E_bias_C5 = new double[LEN_BIAS_C5_CNN];
	std::fill(E_bias_C5, E_bias_C5 + LEN_BIAS_C5_CNN, 0.0);
	E_weight_output = new double[LEN_WEIGHT_OUTPUT_CNN];
	std::fill(E_weight_output, E_weight_output + LEN_WEIGHT_OUTPUT_CNN, 0.0);
	E_bias_output = new double[LEN_BIAS_OUTPUT_CNN];
	std::fill(E_bias_output, E_bias_output + LEN_BIAS_OUTPUT_CNN, 0.0);

	initWeightThreshold();
	getSrcData();
}

double CNN::uniform_rand(double min, double max)
{
	// C++11 RANDOM GENERATION
	static std::mt19937 gen(1);
	std::uniform_real_distribution<double> dst(min, max);
	return dst(gen);
}

bool CNN::uniform_rand(double* src, int len, double min, double max)
{
	for (int i = 0; i < len; i++)
		src[i] = uniform_rand(min, max);

	return true;
}

/*
* (1)、权值使用函数uniform_real_distribution均匀分布初始化，tiny-cnn中每次初始化权值数值都相同，
*      这里作了调整，使每次初始化的权值均不同。每层权值初始化大小范围都不一样；
* (2)、所有层的偏置均初始化为0.
*/
bool CNN::initWeightThreshold()
{
	srand(time(0) + rand());
	const double scale = 6.0;

	double min_ = -std::sqrt(scale / (25.0 + 150.0));
	double max_ = std::sqrt(scale / (25.0 + 150.0));
	uniform_rand(weight_C1, LEN_WEIGHT_C1_CNN, min_, max_);
	for (int i = 0; i < LEN_BIAS_C1_CNN; i++)
		bias_C1[i] = 0.0;

	min_ = -std::sqrt(scale / (4.0 + 1.0));
	max_ = std::sqrt(scale / (4.0 + 1.0));
	uniform_rand(weight_S2, LEN_WEIGHT_S2_CNN, min_, max_);
	for (int i = 0; i < LEN_BIAS_S2_CNN; i++)
		bias_S2[i] = 0.0;

	min_ = -std::sqrt(scale / (150.0 + 400.0));
	max_ = std::sqrt(scale / (150.0 + 400.0));
	uniform_rand(weight_C3, LEN_WEIGHT_C3_CNN, min_, max_);
	for (int i = 0; i < LEN_BIAS_C3_CNN; i++)
		bias_C3[i] = 0.0;

	min_ = -std::sqrt(scale / (4.0 + 1.0));
	max_ = std::sqrt(scale / (4.0 + 1.0));
	uniform_rand(weight_S4, LEN_WEIGHT_S4_CNN, min_, max_);
	for (int i = 0; i < LEN_BIAS_S4_CNN; i++)
		bias_S4[i] = 0.0;

	min_ = -std::sqrt(scale / (400.0 + 3000.0));
	max_ = std::sqrt(scale / (400.0 + 3000.0));
	uniform_rand(weight_C5, LEN_WEIGHT_C5_CNN, min_, max_);
	for (int i = 0; i < LEN_BIAS_C5_CNN; i++)
		bias_C5[i] = 0.0;

	min_ = -std::sqrt(scale / (120.0 + 10.0));
	max_ = std::sqrt(scale / (120.0 + 10.0));
	uniform_rand(weight_output, LEN_WEIGHT_OUTPUT_CNN, min_, max_);
	for (int i = 0; i < LEN_BIAS_OUTPUT_CNN; i++)
		bias_output[i] = 0.0;

	return true;

}

}