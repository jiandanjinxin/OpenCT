#ifndef __CANDIDATE_DATA_H  
#define __CANDIDATE_DATA_H

#include <iostream>
#include <fstream>

using namespace std;

class Candidate
{
public:
	/*
	* 读取Candidate文件的世界坐标数据,并转换为像素数据，写入pos及neg文件中
	*/
	void OutputCandidate();

	/*
	* 由于世界坐标起始位置每个病人都有所不同，所以对应首先需要生成世界坐标标注文件
	* 包括：所属人员，起始Axial世界坐标，移动步长，起始Sagittal世界坐标，移动步长，起始Coronal世界坐标，移动步长
	*/
	void WorldCoordinateConvert();

private:

};


#endif