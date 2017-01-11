#ifndef __CANDIDATE_DATA_H  
#define __CANDIDATE_DATA_H

#include <iostream>
#include <fstream>

#include "HCandidate.h"
#include "HUtil.h"
#include "HDcmFileFormat.h"
#include "HBmpConvert.h"
#include "HLymphHOG.h"
#include "CNN.h"

#include "ximage.h"

#include "dcmtk/config/osconfig.h"  
#include "dcmtk/dcmdata/dctk.h"  
#include "dcmtk/dcmdata/dcpxitem.h"  
#include "dcmtk/dcmjpeg/djdecode.h"  
#include "dcmtk/dcmjpeg/djencode.h" 
#include "dcmtk/dcmjpeg/djcodece.h"  
#include "dcmtk/dcmjpeg/djrplol.h"  
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcvrfd.h"
#include "dcmtk/dcmdata/dcistrmf.h"

using namespace std;
using namespace THU_STD_NAMESPACE;

class Candidate
{
public:
	/*
	* 读取Candidate文件的世界坐标数据,并转换为像素数据，写入pos及neg文件中
	*/
	void OutputCandidate();

	/*
	* 由于世界坐标起始位置每个病人都有所不同，所以对应首先需要生成世界坐标标注文件
	* 包括：所属人员，起始Sagittal世界坐标，起始Coronal世界坐标，起始Axial世界坐标，移动步长
	*/
	void WorldCoordinateConvert();

	// 字符串split
	void split(const string& src, const string& separator, vector<string>& dest)
	{
		string str = src;
		string substring;
		string::size_type start = 0, index;

		do
		{
			index = str.find_first_of(separator, start);
			if (index != string::npos)
			{
				substring = str.substr(start, index - start);
				dest.push_back(substring);
				start = str.find_first_not_of(separator, index);
				if (start == string::npos) return;
			}
		} while (index != string::npos);

		//the last token
		substring = str.substr(start);
		dest.push_back(substring);
	}

private:

};


#endif