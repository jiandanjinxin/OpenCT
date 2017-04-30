#ifndef __DCMFILEPROCESS_H  
#define __DCMFILEPROCESS_H 


#include "Util.h"
#include "DcmFileFormat.h" 

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

class DcmFileProcess
{
public:
	/*--------------------------------------
	Func:createCache
	Purpose:
	�����ļ��л�����
	Paras:
	--------------------------------------*/
	static void createCache();

	/*--------------------------------------
	Func:deleteCache
	Purpose:
	ɾ���ļ��л�����
	Paras:
	--------------------------------------*/
	static void deleteCache();

	/*--------------------------------------
	Func:readAllDcm
	Purpose:
	��ȡ��ǰ�ļ���������dcmͼ�񣬷���vector
	Paras:
	FilePath : �ļ���·��
	position : ����һЩdcm���ԣ�����position
	--------------------------------------*/
	static int readAllDcm(const char* FilePath, std::vector<float>& position);

	/*--------------------------------------
	Func:showPosPosition
	Purpose:
	��ȡ������ѡ�������������ļ�����Ӧ�ļ���������Ȧ��
	Paras:
	FilePath : �ļ���·��
	position : ���ڷ��ص�һЩdcm����position����������ȷ��
	--------------------------------------*/
	static void showPosPosition(const char* FilePath, std::vector<float>& position);

	/*--------------------------------------
	Func:split
	Purpose:
	�ַ����ָ��
	Paras:
	src : ԭʼ�ַ���
	separator : �ָ�ؼ���
	dest : ���طָ���vector
	--------------------------------------*/
	static void split(const std::string& src, const std::string& separator, std::vector<std::string>& dest);

public:
	//��ȡ�����ļ�
	static int readAllFile(const char* FilePath, std::vector<std::string> result);

	std::string dirName;
};

#endif