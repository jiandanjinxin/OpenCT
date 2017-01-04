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
static const bool tbl[6][16] = {
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
* (1)��Ȩֵʹ�ú���uniform_real_distribution���ȷֲ���ʼ����tiny-cnn��ÿ�γ�ʼ��Ȩֵ��ֵ����ͬ��
*      �������˵�����ʹÿ�γ�ʼ����Ȩֵ����ͬ��ÿ��Ȩֵ��ʼ����С��Χ����һ����
* (2)�����в��ƫ�þ���ʼ��Ϊ0.
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

static int reverseInt(int i)
{
	unsigned char ch1, ch2, ch3, ch4;
	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;
	return ((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

static void readMnistImages(std::string filename, double *data_dst, int num_image)
{
	const int width_src_image = 28;
	const int height_src_image = 28;
	const int x_padding = 2;
	const int y_padding = 2;
	const double scale_min = -1;
	const double scale_max = 1;

	std::ifstream file(filename, std::ios::binary);
	assert(file.is_open());

	int magic_number = 0;
	int number_of_images = 0;
	int n_rows = 0;
	int n_cols = 0;

	file.read((char*)&magic_number, sizeof(magic_number));
	magic_number = reverseInt(magic_number);
	file.read((char*)&number_of_images, sizeof(number_of_images));
	number_of_images = reverseInt(number_of_images);
	assert(number_of_images == num_image);

	file.read((char*)&n_rows, sizeof(n_rows));
	n_rows = reverseInt(n_rows);
	file.read((char*)&n_cols, sizeof(n_cols));
	n_cols = reverseInt(n_cols);
	assert(n_rows == height_src_image && n_cols == width_src_image);

	int size_single_image = WIDTH_IMAGE_INPUT_CNN * HEIGHT_IMAGE_INPUT_CNN;

	for (int i = 0; i < number_of_images; i++)
	{
		int addr = size_single_image * i;

		for (int r = 0; r < n_rows; r++)
		{
			for (int c = 0; c < n_cols; c++)
			{
				unsigned char temp = 0;
				file.read((char*)&temp, sizeof(temp));
				data_dst[addr + WIDTH_IMAGE_INPUT_CNN * (r + y_padding) + c + x_padding] = (temp / 255.0) * (scale_max - scale_min) + scale_min;
			}
		}
	}

}

static void readMnistLabels(std::string filename, double *data_dst, int num_image)
{
	const double scale_max = 0.8;

	std::ifstream file(filename, std::ios::binary);
	assert(file.is_open());

	int magic_number = 0;
	int number_of_images = 0;
	file.read((char*)&magic_number, sizeof(magic_number));
	magic_number = reverseInt(magic_number);
	file.read((char*)&number_of_images, sizeof(number_of_images));
	number_of_images = reverseInt(number_of_images);
	assert(number_of_images == num_image);

	for (int i = 0; i < number_of_images; i++)
	{
		unsigned char temp = 0;
		file.read((char*)&temp, sizeof(temp));
		data_dst[i * NUM_MAP_OUTPUT_CNN + temp] = scale_max;
	}
}

bool CNN::getSrcData()
{
	assert(data_input_train && data_output_train && data_input_test && data_output_test);

	std::string filename_train_images = "data/train-images.idx3-ubyte";
	std::string filename_train_labels = "data/train-labels.idx1-ubyte";
	readMnistImages(filename_train_images, data_input_train, NUM_PATTERNS_TRAIN_CNN);
	readMnistLabels(filename_train_labels, data_output_train, NUM_PATTERNS_TRAIN_CNN);

	std::string filename_test_images = "data/t10k-images.idx3-ubyte";
	std::string filename_test_labels = "data/t10k-labels.idx1-ubyte";
	readMnistImages(filename_test_images, data_input_test, NUM_PATTERNS_TEST_CNN);
	readMnistLabels(filename_test_labels, data_output_test, NUM_PATTERNS_TEST_CNN);

	return true;
}

bool CNN::train()
{
	out2wi_S2.clear();
	out2bias_S2.clear();

	out2wi_S4.clear();
	out2bias_S4.clear();

	in2wo_C3.clear();
	weight2io_C3.clear();
	bias2out_C3.clear();

	in2wo_C1.clear();
	weight2io_C1.clear();
	bias2out_C1.clear();

	calc_out2wi(WIDTH_IMAGE_C1_CNN, HEIGHT_IMAGE_C1_CNN, WIDTH_IMAGE_S2_CNN, HEIGHT_IMAGE_S2_CNN, NUM_MAP_S2_CNN, out2wi_S2);
	calc_out2bias(WIDTH_IMAGE_S2_CNN, HEIGHT_IMAGE_S2_CNN, NUM_MAP_S2_CNN, out2bias_S2);
	calc_out2wi(WIDTH_IMAGE_C3_CNN, HEIGHT_IMAGE_C3_CNN, WIDTH_IMAGE_S4_CNN, HEIGHT_IMAGE_S4_CNN, NUM_MAP_S4_CNN, out2wi_S4);
	calc_out2bias(WIDTH_IMAGE_S4_CNN, HEIGHT_IMAGE_S4_CNN, NUM_MAP_S4_CNN, out2bias_S4);
	calc_in2wo(WIDTH_IMAGE_C3_CNN, HEIGHT_IMAGE_C3_CNN, WIDTH_IMAGE_S4_CNN, HEIGHT_IMAGE_S4_CNN, NUM_MAP_C3_CNN, NUM_MAP_S4_CNN, in2wo_C3);
	calc_weight2io(WIDTH_IMAGE_C3_CNN, HEIGHT_IMAGE_C3_CNN, WIDTH_IMAGE_S4_CNN, HEIGHT_IMAGE_S4_CNN, NUM_MAP_C3_CNN, NUM_MAP_S4_CNN, weight2io_C3);
	calc_bias2out(WIDTH_IMAGE_C3_CNN, HEIGHT_IMAGE_C3_CNN, WIDTH_IMAGE_S4_CNN, HEIGHT_IMAGE_S4_CNN, NUM_MAP_C3_CNN, NUM_MAP_S4_CNN, bias2out_C3);
	calc_in2wo(WIDTH_IMAGE_C1_CNN, HEIGHT_IMAGE_C1_CNN, WIDTH_IMAGE_S2_CNN, HEIGHT_IMAGE_S2_CNN, NUM_MAP_C1_CNN, NUM_MAP_C3_CNN, in2wo_C1);
	calc_weight2io(WIDTH_IMAGE_C1_CNN, HEIGHT_IMAGE_C1_CNN, WIDTH_IMAGE_S2_CNN, HEIGHT_IMAGE_S2_CNN, NUM_MAP_C1_CNN, NUM_MAP_C3_CNN, weight2io_C1);
	calc_bias2out(WIDTH_IMAGE_C1_CNN, HEIGHT_IMAGE_C1_CNN, WIDTH_IMAGE_S2_CNN, HEIGHT_IMAGE_S2_CNN, NUM_MAP_C1_CNN, NUM_MAP_C3_CNN, bias2out_C1);

	int iter = 0;
	for (iter = 0; iter < NUM_EPOCHS_CNN; iter++)
	{
		std::cout << "epoch: " << iter + 1;

		for (int i = 0; i < NUM_PATTERNS_TRAIN_CNN; i++)
		{
			// ����ͼƬ��С1024�ƶ�ָ���ȡÿ��ͼƬ
			data_single_image = data_input_train + i * NUM_NEURON_INPUT_CNN;
			data_single_label = data_output_train + i * NUM_NEURON_OUTPUT_CNN;
			// ǰ�򴫲�
			Forward_C1();
			Forward_S2();
			Forward_C3();
			Forward_S4();
			Forward_C5();
			Forward_output();
			// ���򴫲�
			Backward_output();
			Backward_C5();
			Backward_S4();
			Backward_C3();
			Backward_S2();
			Backward_C1();
			Backward_input();
			// ����Ȩֵ
			UpdateWeights();
		}

		double accuracyRate = test();
		std::cout << ", accuracy rate: " << accuracyRate << std::endl;

		if (accuracyRate > ACCURACY_RATE_CNN)
		{
			saveModelFile("data/cnn.model");
			std::cout << "generate cnn model." << std::endl;
			break;
		}
	}

	if (iter == NUM_EPOCHS_CNN)
	{
		saveModelFile("data/cnn.model");
		std::cout << "generate cnn model." << std::endl;
	}

	return true;
}

double CNN::activation_function_tanh(double x)
{
	double ep = std::exp(x);
	double em = std::exp(-x);

	return (ep - em) / (ep + em);
}

double CNN::activation_function_tanh_derivative(double x)
{
	return (1.0 - x * x);
}

double CNN::activation_function_identity(double x)
{
	return x;
}

double CNN::activation_function_identity_derivative(double x)
{
	return 1;
}

double CNN::loss_function_mse(double y, double t)
{
	return (y - t) * (y - t) / 2;
}

double CNN::loss_function_mse_derivative(double y, double t)
{
	return (y - t);
}

void CNN::loss_function_gradient(const double* y, const double* t, double* dst, int len)
{
	for (int i = 0; i < len; i++) 
	{
		dst[i] = loss_function_mse_derivative(y[i], t[i]);
	}
}

double CNN::dot_product(const double* s1, const double* s2, int len)
{
	double result = 0.0;

	for (int i = 0; i < len; i++) 
	{
		result += s1[i] * s2[i];
	}

	return result;
}

bool CNN::muladd(const double* src, double c, int len, double* dst)
{
	for (int i = 0; i < len; i++) 
	{
		dst[i] += (src[i] * c);
	}

	return true;
}

int CNN::get_index(int x, int y, int channel, int width, int height, int depth)
{
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	assert(channel >= 0 && channel < depth);
	return (height * channel + y) * width + x;
}

void CNN::calc_out2wi(int width_in, int height_in, int width_out, int height_out, int depth_out, std::vector<wi_connections> &out2wi)
{
	for (int i = 0; i < depth_out; i++)
	{
		int block = width_in * height_in * i;

		for (int y = 0; y < height_out; y++)
		{
			for (int x = 0; x < width_out; x++)
			{
				int rows = y * WIDTH_KERNEL_POOLING_CNN;
				int cols = x * HEIGHT_KERNEL_POOLING_CNN;

				wi_connections wi_connections_;
				std::pair<int, int> pair_;

				for (int m = 0; m < WIDTH_KERNEL_POOLING_CNN; m++)
				{
					for (int n = 0; n < HEIGHT_KERNEL_POOLING_CNN; n++)
					{
						pair_.first = i;
						pair_.second = (rows + m) * width_in + cols + n + block;
						wi_connections_.push_back(pair_);
					}
				}
				out2wi.push_back(wi_connections_);
			}
		}
	}
}

void CNN::calc_out2bias(int width, int height, int depth, std::vector<int> &out2bias)
{
	for (int i = 0; i < depth; i++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				out2bias.push_back(i);
			}
		}
	}
}

void CNN::calc_in2wo(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<wo_connections> &in2wo)
{
	int len = width_in * height_in * depth_in;
	in2wo.resize(len);

	for (int c = 0; c < depth_in; c++)
	{
		for (int y = 0; y < height_in; y += HEIGHT_KERNEL_POOLING_CNN)
		{
			for (int x = 0; x < width_in; x += WIDTH_KERNEL_POOLING_CNN)
			{
				int dymax = min(SIZE_POOLING_CNN, height_in - y);
				int dxmax = min(SIZE_POOLING_CNN, width_in - x);
				int dstx = x / WIDTH_KERNEL_POOLING_CNN;
				int dsty = y / HEIGHT_KERNEL_POOLING_CNN;

				for (int dy = 0; dy < dymax; dy++)
				{
					for (int dx = 0; dx < dxmax; dx++)
					{
						int index_in = get_index(x + dx, y + dy, c, width_in, height_in, depth_in);
						int index_out = get_index(dstx, dsty, c, width_out, height_out, depth_out);

						wo_connections wo_connections_;
						std::pair<int, int> pair_;
						pair_.first = c;
						pair_.second = index_out;
						wo_connections_.push_back(pair_);

						in2wo[index_in] = wo_connections_;
					}
				}
			}
		}
	}
}

void CNN::calc_weight2io(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<io_connections> &weight2io)
{
	int len = depth_in;
	weight2io.resize(len);

	for (int c = 0; c < depth_in; c++)
	{
		for (int y = 0; y < height_in; y += HEIGHT_KERNEL_POOLING_CNN)
		{
			for (int x = 0; x < width_in; x += WIDTH_KERNEL_POOLING_CNN)
			{
				int dymax = min(SIZE_POOLING_CNN, height_in - y);
				int dxmax = min(SIZE_POOLING_CNN, width_in - x);
				int dstx = x / WIDTH_KERNEL_POOLING_CNN;
				int dsty = y / HEIGHT_KERNEL_POOLING_CNN;

				for (int dy = 0; dy < dymax; dy++)
				{
					for (int dx = 0; dx < dxmax; dx++)
					{
						int index_in = get_index(x + dx, y + dy, c, width_in, height_in, depth_in);
						int index_out = get_index(dstx, dsty, c, width_out, height_out, depth_out);

						std::pair<int, int> pair_;
						pair_.first = index_in;
						pair_.second = index_out;

						weight2io[c].push_back(pair_);
					}
				}
			}
		}
	}
}

void CNN::calc_bias2out(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<std::vector<int> > &bias2out)
{
	int len = depth_in;
	bias2out.resize(len);

	for (int c = 0; c < depth_in; c++)
	{
		for (int y = 0; y < height_out; y++)
		{
			for (int x = 0; x < width_out; x++)
			{
				int index_out = get_index(x, y, c, width_out, height_out, depth_out);
				bias2out[c].push_back(index_out);
			}
		}
	}
}

bool CNN::Forward_C1()
{
	init_variable(neuron_C1, 0.0, NUM_NEURON_C1_CNN);

	for (int o = 0; o < NUM_MAP_C1_CNN; o++)
	{
		for (int inc = 0; inc < NUM_MAP_INPUT_CNN; inc++)
		{
			int addr1 = get_index(0, 0, NUM_MAP_INPUT_CNN * o + inc, WIDTH_KERNEL_CONV_CNN, HEIGHT_KERNEL_CONV_CNN, NUM_MAP_C1_CNN * NUM_MAP_INPUT_CNN);
			int addr2 = get_index(0, 0, inc, WIDTH_IMAGE_INPUT_CNN, HEIGHT_IMAGE_INPUT_CNN, NUM_MAP_INPUT_CNN);
			int addr3 = get_index(0, 0, o, WIDTH_IMAGE_C1_CNN, HEIGHT_IMAGE_C1_CNN, NUM_MAP_C1_CNN);

			const double *pw = &weight_C1[0] + addr1;
			const double *pi = data_single_image + addr2;
			double *pa = &neuron_C1[0] + addr3;

			for (int y = 0; y < HEIGHT_IMAGE_C1_CNN; y++)
			{
				for (int x = 0; x < WIDTH_IMAGE_C1_CNN; x++)
				{
					const double *ppw = pw;
					const double *ppi = pi + y * WIDTH_IMAGE_INPUT_CNN + x;
					double sum = 0.0;

					for (int wy = 0; wy < HEIGHT_KERNEL_CONV_CNN; wy++)
					{
						for (int wx = 0; wx < WIDTH_KERNEL_CONV_CNN; wx++)
						{
							sum += *ppw++ * ppi[wy * WIDTH_IMAGE_INPUT_CNN + wx];
						}
					}

					pa[y * WIDTH_IMAGE_C1_CNN + x] += sum;
				}
			}
		}

		int addr3 = get_index(0, 0, o, WIDTH_IMAGE_C1_CNN, HEIGHT_IMAGE_C1_CNN, NUM_MAP_C1_CNN);
		double *pa = &neuron_C1[0] + addr3;
		double b = bias_C1[o];

		for (int y = 0; y < HEIGHT_IMAGE_C1_CNN; y++)
		{
			for (int x = 0; x < WIDTH_IMAGE_C1_CNN; x++)
			{
				pa[y * WIDTH_IMAGE_C1_CNN + x] += b;
			}
		}
	}

	for (int i = 0; i < NUM_NEURON_C1_CNN; i++)
	{
		neuron_C1[i] = activation_function_tanh(neuron_C1[i]);
	}

	return true;
}

bool CNN::Forward_S2()
{
	init_variable(neuron_S2, 0.0, NUM_NEURON_S2_CNN);
	double scale_factor = 1.0 / (WIDTH_KERNEL_POOLING_CNN * HEIGHT_KERNEL_POOLING_CNN);

	assert(out2wi_S2.size() == NUM_NEURON_S2_CNN);
	assert(out2bias_S2.size() == NUM_NEURON_S2_CNN);

	for (int i = 0; i < NUM_NEURON_S2_CNN; i++)
	{
		const wi_connections &connections = out2wi_S2[i];
		neuron_S2[i] = 0;

		for (int index = 0; index < connections.size(); index++)
		{
			neuron_S2[i] += weight_S2[connections[index].first] * neuron_C1[connections[index].second];
		}

		neuron_S2[i] += scale_factor;
		neuron_S2[i] += bias_S2[out2bias_S2[i]];
	}

	for (int i = 0; i < NUM_NEURON_S2_CNN; i++)
	{
		neuron_S2[i] = activation_function_tanh(neuron_S2[i]);
	}

	return true;
}

bool CNN::Forward_C3()
{
	init_variable(neuron_C3, 0.0, NUM_NEURON_C3_CNN);

	for (int o = 0; o < NUM_MAP_C3_CNN; o++) {
		for (int inc = 0; inc < NUM_MAP_S2_CNN; inc++) {
			if (!tbl[inc][o]) continue;

			int addr1 = get_index(0, 0, NUM_MAP_S2_CNN * o + inc, WIDTH_KERNEL_CONV_CNN, HEIGHT_KERNEL_CONV_CNN, NUM_MAP_C3_CNN * NUM_MAP_S2_CNN);
			int addr2 = get_index(0, 0, inc, WIDTH_IMAGE_S2_CNN, HEIGHT_IMAGE_S2_CNN, NUM_MAP_S2_CNN);
			int addr3 = get_index(0, 0, o, WIDTH_IMAGE_C3_CNN, HEIGHT_IMAGE_C3_CNN, NUM_MAP_C3_CNN);

			const double *pw = &weight_C3[0] + addr1;
			const double *pi = &neuron_S2[0] + addr2;
			double *pa = &neuron_C3[0] + addr3;

			for (int y = 0; y < HEIGHT_IMAGE_C3_CNN; y++) {
				for (int x = 0; x < WIDTH_IMAGE_C3_CNN; x++) {
					const double *ppw = pw;
					const double *ppi = pi + y * WIDTH_IMAGE_S2_CNN + x;
					double sum = 0.0;

					for (int wy = 0; wy < HEIGHT_KERNEL_CONV_CNN; wy++) {
						for (int wx = 0; wx < WIDTH_KERNEL_CONV_CNN; wx++) {
							sum += *ppw++ * ppi[wy * WIDTH_IMAGE_S2_CNN + wx];
						}
					}

					pa[y * WIDTH_IMAGE_C3_CNN + x] += sum;
				}
			}
		}

		int addr3 = get_index(0, 0, o, WIDTH_IMAGE_C3_CNN, HEIGHT_IMAGE_C3_CNN, NUM_MAP_C3_CNN);
		double* pa = &neuron_C3[0] + addr3;
		double b = bias_C3[o];
		for (int y = 0; y < HEIGHT_IMAGE_C3_CNN; y++) {
			for (int x = 0; x < WIDTH_IMAGE_C3_CNN; x++) {
				pa[y * WIDTH_IMAGE_C3_CNN + x] += b;
			}
		}
	}

	for (int i = 0; i < NUM_NEURON_C3_CNN; i++) {
		neuron_C3[i] = activation_function_tanh(neuron_C3[i]);
	}

	return true;
}

bool CNN::Forward_S4()
{
	double scale_factor = 1.0 / (WIDTH_KERNEL_POOLING_CNN * HEIGHT_KERNEL_POOLING_CNN);
	init_variable(neuron_S4, 0.0, NUM_NEURON_S4_CNN);

	assert(out2wi_S4.size() == NUM_NEURON_S4_CNN);
	assert(out2bias_S4.size() == NUM_NEURON_S4_CNN);

	for (int i = 0; i < NUM_NEURON_S4_CNN; i++) {
		const wi_connections& connections = out2wi_S4[i];
		neuron_S4[i] = 0.0;

		for (int index = 0; index < connections.size(); index++) {
			neuron_S4[i] += weight_S4[connections[index].first] * neuron_C3[connections[index].second];
		}

		neuron_S4[i] *= scale_factor;
		neuron_S4[i] += bias_S4[out2bias_S4[i]];
	}

	for (int i = 0; i < NUM_NEURON_S4_CNN; i++) {
		neuron_S4[i] = activation_function_tanh(neuron_S4[i]);
	}

	return true;
}

bool CNN::Forward_C5()
{
	init_variable(neuron_C5, 0.0, NUM_NEURON_C5_CNN);

	for (int o = 0; o < NUM_MAP_C5_CNN; o++) {
		for (int inc = 0; inc < NUM_MAP_S4_CNN; inc++) {
			int addr1 = get_index(0, 0, NUM_MAP_S4_CNN * o + inc, WIDTH_KERNEL_CONV_CNN, HEIGHT_KERNEL_CONV_CNN, NUM_MAP_C5_CNN * NUM_MAP_S4_CNN);
			int addr2 = get_index(0, 0, inc, WIDTH_IMAGE_S4_CNN, HEIGHT_IMAGE_S4_CNN, NUM_MAP_S4_CNN);
			int addr3 = get_index(0, 0, o, WIDTH_IMAGE_C5_CNN, HEIGHT_IMAGE_C5_CNN, NUM_MAP_C5_CNN);

			const double *pw = &weight_C5[0] + addr1;
			const double *pi = &neuron_S4[0] + addr2;
			double *pa = &neuron_C5[0] + addr3;

			for (int y = 0; y < HEIGHT_IMAGE_C5_CNN; y++) {
				for (int x = 0; x < WIDTH_IMAGE_C5_CNN; x++) {
					const double *ppw = pw;
					const double *ppi = pi + y * WIDTH_IMAGE_S4_CNN + x;
					double sum = 0.0;

					for (int wy = 0; wy < HEIGHT_KERNEL_CONV_CNN; wy++) {
						for (int wx = 0; wx < WIDTH_KERNEL_CONV_CNN; wx++) {
							sum += *ppw++ * ppi[wy * WIDTH_IMAGE_S4_CNN + wx];
						}
					}

					pa[y * WIDTH_IMAGE_C5_CNN + x] += sum;
				}
			}
		}

		int addr3 = get_index(0, 0, o, WIDTH_IMAGE_C5_CNN, HEIGHT_IMAGE_C5_CNN, NUM_MAP_C5_CNN);
		double *pa = &neuron_C5[0] + addr3;
		double b = bias_C5[o];
		for (int y = 0; y < HEIGHT_IMAGE_C5_CNN; y++) {
			for (int x = 0; x < WIDTH_IMAGE_C5_CNN; x++) {
				pa[y * WIDTH_IMAGE_C5_CNN + x] += b;
			}
		}
	}

	for (int i = 0; i < NUM_NEURON_C5_CNN; i++) {
		neuron_C5[i] = activation_function_tanh(neuron_C5[i]);
	}

	return true;
}

bool CNN::Forward_output()
{
	init_variable(neuron_output, 0.0, NUM_NEURON_OUTPUT_CNN);

	for (int i = 0; i < NUM_NEURON_OUTPUT_CNN; i++) {
		neuron_output[i] = 0.0;

		for (int c = 0; c < NUM_NEURON_C5_CNN; c++) {
			neuron_output[i] += weight_output[c * NUM_NEURON_OUTPUT_CNN + i] * neuron_C5[c];
		}

		neuron_output[i] += bias_output[i];
	}

	for (int i = 0; i < NUM_NEURON_OUTPUT_CNN; i++) {
		neuron_output[i] = activation_function_tanh(neuron_output[i]);
	}

	return true;
}

bool CNN::Backward_output()
{
	init_variable(delta_neuron_output, 0.0, NUM_NEURON_OUTPUT_CNN);

	double dE_dy[NUM_NEURON_OUTPUT_CNN];
	init_variable(dE_dy, 0.0, NUM_NEURON_OUTPUT_CNN);
	loss_function_gradient(neuron_output, data_single_label, dE_dy, NUM_NEURON_OUTPUT_CNN); // ��ʧ����: mean squared error(������)  

	// delta = dE/da = (dE/dy) * (dy/da)  
	for (int i = 0; i < NUM_NEURON_OUTPUT_CNN; i++) {
		double dy_da[NUM_NEURON_OUTPUT_CNN];
		init_variable(dy_da, 0.0, NUM_NEURON_OUTPUT_CNN);

		dy_da[i] = activation_function_tanh_derivative(neuron_output[i]);
		delta_neuron_output[i] = dot_product(dE_dy, dy_da, NUM_NEURON_OUTPUT_CNN);
	}

	return true;
}

}