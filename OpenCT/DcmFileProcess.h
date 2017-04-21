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
	//���������ļ���
	static void createCache();
	//ɾ�������ļ���
	static void deleteCache();
	//��ȡ��ǰ�ļ���������dcmͼ�񣬷���vector
	static int readAllDcm(const char* FilePath, std::vector<float>& position);

	//��ȡ������ѡ�������������ļ�����Ӧ�ļ���������Ȧ��
	static void showPosPosition(const char* FilePath, std::vector<float>& position);

	static void split(const std::string& src, const std::string& separator, std::vector<std::string>& dest);

public:
	//��ȡ�����ļ�
	static int readAllFile(const char* FilePath, std::vector<std::string> result);

	std::string dirName;
};

#endif