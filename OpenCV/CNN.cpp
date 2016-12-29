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

}