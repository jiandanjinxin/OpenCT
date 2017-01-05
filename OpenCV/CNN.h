#ifndef _CNN_HPP_
#define _CNN_HPP_

#include <vector>
#include <unordered_map>

namespace CNN {

#define WIDTH_IMAGE_INPUT_CNN     32 // 归一化图像宽
#define HEIGHT_IMAGE_INPUT_CNN    32 // 归一化图像高
#define WIDTH_IMAGE_C1_CNN        28
#define HEIGHT_IMAGE_C1_CNN       28
#define WIDTH_IMAGE_S2_CNN        14
#define HEIGHT_IMAGE_S2_CNN       14
#define WIDTH_IMAGE_C3_CNN        10
#define HEIGHT_IMAGE_C3_CNN       10
#define WIDTH_IMAGE_S4_CNN        5
#define HEIGHT_IMAGE_S4_CNN       5
#define WIDTH_IMAGE_C5_CNN        1
#define HEIGHT_IMAGE_C5_CNN       1
#define WIDTH_IMAGE_OUTPUT_CNN    1
#define HEIGHT_IMAGE_OUTPUT_CNN   1

#define WIDTH_KERNEL_CONV_CNN     5  // 卷积核
#define HEIGHT_KERNEL_CONV_CNN    5
#define WIDTH_KERNEL_POOLING_CNN  2
#define HEIGHT_KERNEL_POOLING_CNN 2
#define SIZE_POOLING_CNN          2

#define NUM_MAP_INPUT_CNN         1  // 输入层MAP个数
#define NUM_MAP_C1_CNN            6  // C1层MAP个数
#define NUM_MAP_S2_CNN            6  // S2层MAP个数
#define NUM_MAP_C3_CNN            16 // C3层MAP个数
#define NUM_MAP_S4_CNN            16 // S4层MAP个数
#define NUM_MAP_C5_CNN            120// C5层MAP个数
#define NUM_MAP_OUTPUT_CNN        10 // 输出层MAP个数

#define NUM_PATTERNS_TRAIN_CNN    60000
#define NUM_PATTERNS_TEST_CNN     10000
#define NUM_EPOCHS_CNN            100
#define ACCURACY_RATE_CNN         0.985
#define LEARNING_RATE_CNN         0.01
#define EPS_CNN                   1e-8

#define LEN_WEIGHT_C1_CNN         150   // C1层权值数量，5*5*6*1=150
#define LEN_BIAS_C1_CNN           6
#define LEN_WEIGHT_S2_CNN         6
#define LEN_BIAS_S2_CNN           6
#define LEN_WEIGHT_C3_CNN         2400  // C3层权值数量，5*5*16*6=2400
#define LEN_BIAS_C3_CNN           16
#define LEN_WEIGHT_S4_CNN         16
#define LEN_BIAS_S4_CNN           16
#define LEN_WEIGHT_C5_CNN         48000 // C5层权值数量，5*5*16*120=48000
#define LEN_BIAS_C5_CNN           120
#define LEN_WEIGHT_OUTPUT_CNN     1200  // 120*10=1200
#define LEN_BIAS_OUTPUT_CNN       10    //

#define NUM_NEURON_INPUT_CNN      1024  // 32*32=1024
#define NUM_NEURON_C1_CNN         4704  // C1: 28*28*6=4704
#define NUM_NEURON_S2_CNN         1176  // S2: 14*14*6=1176
#define NUM_NEURON_C3_CNN         1600  // C3: 10*10*16=1600
#define NUM_NEURON_S4_CNN         400   // S4: 5*5*16=400
#define NUM_NEURON_C5_CNN         120   // C5: 1*120=120
#define NUM_NEURON_OUTPUT_CNN     10    // 1*10=10


class CNN
{
public:
	CNN();     // 构造函数
	~CNN();    // 析构函数

	void init();                                                       // 初始化分配空间
	bool train();                                                      // 基于该神经网络进行训练
	int predict(const unsigned char *data, int width, int height);     // 进行数据预测
	bool readModelFile(const char *filename);                          // 读取已经训练好的反向传播模型

protected:
	typedef std::vector<std::pair<int, int> > wi_connections;
	typedef std::vector<std::pair<int, int> > wo_connections;
	typedef std::vector<std::pair<int, int> > io_connections;

	void release();                                                    // 释放申请的空间
	bool saveModelFile(const char *name);                              // 将训练好的MODEL保存起来，包括各层的节点数，权值和阈值
	bool initWeightThreshold();                                        // 初始化，产生[-1, 1]之间的随机小数
	bool getSrcData();                                                 // 读取MNIST数据集

	double test();                                                     // 训练完1次计算1次准确率
	double activation_function_tanh(double x);                         // 激活函数tanh
	double activation_function_tanh_derivative(double x);              // 激活函数tanh的导数
	double activation_function_identity(double x);
	double activation_function_identity_derivative(double x);

	double loss_function_mse(double y, double t);                      // 损失函数：mean squared error
	double loss_function_mse_derivative(double y, double t);
	void loss_function_gradient(const double *y, const double *t, double *dst, int len);
	double dot_product(const double *s1, const double *s2, int len);   // 点乘运算
	bool muladd(const double *src, double c, int len, double *dst);    // dst[i] += c * src[i]
	void init_variable(double *val, double c, int len);
	bool uniform_rand(double *src, int len, double min, double max);
	double uniform_rand(double min, double max);
	int get_index(int x, int y, int channel, int width, int height, int depth);

	void calc_out2wi(int width_in, int height_in, int width_out, int height_out, int depth_out, std::vector<wi_connections> &out2wi);
	void calc_out2bias(int width, int height, int depth, std::vector<int> &out2bias);
	void calc_in2wo(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<wo_connections> &in2wo);
	void calc_weight2io(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<io_connections> &weight2io);
	void calc_bias2out(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<std::vector<int> > &bias2out);

	// 前向传播
	bool Forward_C1();
	bool Forward_S2();
	bool Forward_C3();
	bool Forward_S4();
	bool Forward_C5();
	bool Forward_output();

	// 反向传播
	bool Backward_output();
	bool Backward_C5();
	bool Backward_S4();
	bool Backward_C3();
	bool Backward_S2();
	bool Backward_C1();
	bool Backward_input();

	bool UpdateWeights();
	void update_weights_bias(const double *delta, double *e_weight, double *weight, int len);

private:
	double *data_input_train;          // 原始标准输入数据，训练，范围[-1, 1]
	double *data_output_train;         // 原始标准期望结果，训练，取值-0.8/0.8
	double *data_input_test;           // 原始标准输入数据，测试，范围[-1, 1]
	double *data_output_test;          // 原始标准期望结果, 测试，取值-0.8/0.8
	double *data_single_image;
	double *data_single_label;

	double weight_C1[LEN_WEIGHT_C1_CNN];
	double bias_C1[LEN_BIAS_C1_CNN];
	double weight_S2[LEN_WEIGHT_S2_CNN];
	double bias_S2[LEN_BIAS_S2_CNN];
	double weight_C3[LEN_WEIGHT_C3_CNN];
	double bias_C3[LEN_BIAS_C3_CNN];
	double weight_S4[LEN_WEIGHT_S4_CNN];
	double bias_S4[LEN_BIAS_S4_CNN];
	double weight_C5[LEN_WEIGHT_C5_CNN];
	double bias_C5[LEN_BIAS_C5_CNN];
	double weight_output[LEN_WEIGHT_OUTPUT_CNN];
	double bias_output[LEN_BIAS_OUTPUT_CNN];

	double E_weight_C1[LEN_WEIGHT_C1_CNN];
	double E_bias_C1[LEN_BIAS_C1_CNN];
	double E_weight_S2[LEN_WEIGHT_S2_CNN];
	double E_bias_S2[LEN_BIAS_S2_CNN];
	double E_weight_C3[LEN_WEIGHT_C3_CNN];
	double E_bias_C3[LEN_BIAS_C3_CNN];
	double E_weight_S4[LEN_WEIGHT_S4_CNN];
	double E_bias_S4[LEN_BIAS_S4_CNN];
	double *E_weight_C5;
	double *E_bias_C5;
	double *E_weight_output;
	double *E_bias_output;

	// data_single_image
	double neuron_input[NUM_NEURON_INPUT_CNN];
	double neuron_C1[NUM_NEURON_C1_CNN];
	double neuron_S2[NUM_NEURON_S2_CNN];
	double neuron_C3[NUM_NEURON_C3_CNN];
	double neuron_S4[NUM_NEURON_S4_CNN];
	double neuron_C5[NUM_NEURON_C5_CNN];
	double neuron_output[NUM_NEURON_OUTPUT_CNN];

	// 神经元误差
	double delta_neuron_output[NUM_NEURON_OUTPUT_CNN];
	double delta_neuron_C5[NUM_NEURON_C5_CNN];
	double delta_neuron_S4[NUM_NEURON_S4_CNN];
	double delta_neuron_C3[NUM_NEURON_C3_CNN];
	double delta_neuron_S2[NUM_NEURON_S2_CNN];
	double delta_neuron_C1[NUM_NEURON_C1_CNN];
	double delta_neuron_input[NUM_NEURON_INPUT_CNN];

	// 权值误差，阈值误差
	double delta_weight_C1[LEN_WEIGHT_C1_CNN];
	double delta_bias_C1[LEN_BIAS_C1_CNN];
	double delta_weight_S2[LEN_WEIGHT_S2_CNN];
	double delta_bias_S2[LEN_BIAS_S2_CNN];
	double delta_weight_C3[LEN_WEIGHT_C3_CNN];
	double delta_bias_C3[LEN_BIAS_C3_CNN];
	double delta_weight_S4[LEN_WEIGHT_S4_CNN];
	double delta_bias_S4[LEN_BIAS_S4_CNN];
	double delta_weight_C5[LEN_WEIGHT_C5_CNN];
	double delta_bias_C5[LEN_BIAS_C5_CNN];
	double delta_weight_output[LEN_WEIGHT_OUTPUT_CNN];
	double delta_bias_output[LEN_BIAS_OUTPUT_CNN];

	std::vector<wi_connections> out2wi_S2;             // out_id -> [(weight_id, in_id)]
	std::vector<int> out2bias_S2;
	std::vector<wi_connections> out2wi_S4;
	std::vector<int> out2bias_S4;
	std::vector<wo_connections> in2wo_C3;              // in_id -> [(weight_id, out_id)]
	std::vector<io_connections> weight2io_C3;          // weight_id -> [(in_id, out_id)]
	std::vector<std::vector<int> > bias2out_C3;
	std::vector<wo_connections> in2wo_C1;              
	std::vector<io_connections> weight2io_C1;          
	std::vector<std::vector<int> > bias2out_C1;
};

}

#endif //_CNN_HPP_