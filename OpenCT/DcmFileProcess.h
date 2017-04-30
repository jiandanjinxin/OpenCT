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
	创建文件夹缓冲区
	Paras:
	--------------------------------------*/
	static void createCache();

	/*--------------------------------------
	Func:deleteCache
	Purpose:
	删除文件夹缓冲区
	Paras:
	--------------------------------------*/
	static void deleteCache();

	/*--------------------------------------
	Func:readAllDcm
	Purpose:
	读取当前文件夹下所有dcm图像，返回vector
	Paras:
	FilePath : 文件夹路径
	position : 返回一些dcm属性，放入position
	--------------------------------------*/
	static int readAllDcm(const char* FilePath, std::vector<float>& position);

	/*--------------------------------------
	Func:showPosPosition
	Purpose:
	读取正例候选集的世界坐标文件，对应文件进行正例圈定
	Paras:
	FilePath : 文件夹路径
	position : 基于返回的一些dcm属性position，进行区域确定
	--------------------------------------*/
	static void showPosPosition(const char* FilePath, std::vector<float>& position);

	/*--------------------------------------
	Func:split
	Purpose:
	字符串分割函数
	Paras:
	src : 原始字符串
	separator : 分割关键字
	dest : 返回分割结果vector
	--------------------------------------*/
	static void split(const std::string& src, const std::string& separator, std::vector<std::string>& dest);

public:
	//读取所有文件
	static int readAllFile(const char* FilePath, std::vector<std::string> result);

	std::string dirName;
};

#endif