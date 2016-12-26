#ifndef _CNN_HPP_
#define _CNN_HPP_

#include <vector>
#include <unordered_map>

namespace CNN {

#define WIDTH_IMAGE_INPUT_CNN     32 // ��һ��ͼ���
#define HEIGHT_IMAGE_INPUT_CNN    32 // ��һ��ͼ���
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

#define WIDTH_KERNEL_CONV_CNN     5  // �����
#define HEIGHT_KERNEL_CONV_CNN    5
#define WIDTH_KERNEL_POOLING_CNN  2
#define HEIGHT_KERNEL_POOLING_CNN 2
#define SIZE_POOLING_CNN          2

#define NUM_MAP_INPUT_CNN         1  // �����MAP����
#define NUM_MAP_C1_CNN            6  // C1��MAP����
#define NUM_MAP_S2_CNN            6  // S2��MAP����
#define NUM_MAP_C3_CNN            16 // C3��MAP����
#define NUM_MAP_S4_CNN            16 // S4��MAP����
#define NUM_MAP_C5_CNN            120// C5��MAP����
#define NUM_MAP_OUTPUT_CNN        10 // �����MAP����

#define NUM_PATTERNS_TRAIN_CNN    60000
#define NUM_PATTERNS_TEST_CNN     10000
#define NUM_EPOCHS_CNN            100
#define ACCURACY_RATE_CNN         0.985
#define LEARNING_RATE_CNN         0.01
#define EPS_CNN                   1e-8

#define LEN_WEIGHT_C1_CNN         150   // C1��Ȩֵ������5*5*6*1=150
#define LEN_BIAS_C1_CNN           6
#define LEN_WEIGHT_S2_CNN         6
#define LEN_BIAS_S2_CNN           6
#define LEN_WEIGHT_C3_CNN         2400  // C3��Ȩֵ������5*5*16*6=2400
#define LEN_BIAS_C3_CNN           16
#define LEN_WEIGHT_S4_CNN         16
#define LEN_BIAS_S4_CNN           16
#define LEN_WEIGHT_C5_CNN         48000 // C5��Ȩֵ������5*5*16*120=48000
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
	CNN();     // ���캯��
	~CNN();    // ��������

	void init();           // ��ʼ������ռ�
	bool train();          // ���ڸ����������ѵ��



};

}

#endif //_CNN_HPP_