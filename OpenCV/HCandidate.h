#ifndef __CANDIDATE_DATA_H  
#define __CANDIDATE_DATA_H

#include <iostream>
#include <fstream>

using namespace std;

class Candidate
{
public:
	/*
	* ��ȡCandidate�ļ���������������,��ת��Ϊ�������ݣ�д��pos��neg�ļ���
	*/
	void OutputCandidate();

	/*
	* ��������������ʼλ��ÿ�����˶�������ͬ�����Զ�Ӧ������Ҫ�������������ע�ļ�
	* ������������Ա����ʼAxial�������꣬�ƶ���������ʼSagittal�������꣬�ƶ���������ʼCoronal�������꣬�ƶ�����
	*/
	void WorldCoordinateConvert();

private:

};


#endif