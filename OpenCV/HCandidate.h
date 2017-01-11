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
	* ��ȡCandidate�ļ���������������,��ת��Ϊ�������ݣ�д��pos��neg�ļ���
	*/
	void OutputCandidate();

	/*
	* ��������������ʼλ��ÿ�����˶�������ͬ�����Զ�Ӧ������Ҫ�������������ע�ļ�
	* ������������Ա����ʼSagittal�������꣬��ʼCoronal�������꣬��ʼAxial�������꣬�ƶ�����
	*/
	void WorldCoordinateConvert();

	// �ַ���split
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