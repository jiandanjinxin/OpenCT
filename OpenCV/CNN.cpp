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



}