﻿#include "DcmFileProcess.h"
#include "DcmFileFormat.h"

#include <opencv2\opencv.hpp>  

#include <direct.h>

std::string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\') + 1)[0] = 0; // 删除文件名，只获得路径字串
	std::string path = szFilePath;

	return path;
}

int DcmFileProcess::readAllDcm(const char* FilePath)
{
	//构造类对象  
	CStatDir statdir;
	std::vector<std::string> AllDcmFile;
	int count = 0;
	//设置要遍历的目录  
	if (!statdir.SetInitDir(FilePath))
	{
		puts("目录不存在");
	}
	//开始遍历,获取该文件夹下所有dcm格式图像，一般为一个病患
	AllDcmFile.clear();
	AllDcmFile = statdir.BeginBrowseFilenames("*.dcm");
	//创建缓冲文件夹
	createCache();
	std::string dirName = GetExePath();
	dirName += "\\cache\\";

	cv::Mat mat[700];

	for (auto iter = AllDcmFile.cbegin(); iter != AllDcmFile.cend(); iter++)
	{
		// 获取原始dcm图像的前缀目录
		std::string subImageName = dirName;
		// 将原始dcm图像的string转换为char*
		const char *chImageName = (*iter).c_str();
		// 获取原始dcm图像的前缀目录的string转换为char*
		const char *chSubImageName = subImageName.c_str();
		// 基于dcmtk实现类
		THU_STD_NAMESPACE::TDcmFileFormat dcm = THU_STD_NAMESPACE::TDcmFileFormat(chImageName);
		// 获取dcm图像的InstanceNumber
		int InstancePosition = dcm.getPositionNumber();
		// 生成bmp图像存储路径
		char BmpName[256];
		sprintf_s(BmpName, "%s%06d.bmp", chSubImageName, InstancePosition);
		// 进行图像转换
		dcm.setWindow(715, 3478);
		dcm.saveToBmp(BmpName);
		//读取所有dcm图像构造三维数组
		cv::Mat temp = cv::imread(BmpName, cv::IMREAD_GRAYSCALE);
		mat[InstancePosition - 1] = temp;

		count++;
	}
	
	for (int total = 0; total < mat[0].rows; total++)
	{
		// 获取原始dcm图像的前缀目录
		std::string subImageName = dirName;
		// 获取原始dcm图像的前缀目录的string转换为char*
		const char *chSubImageName = subImageName.c_str();

		//构造冠状位图像数据
		uchar* data = new uchar[count * mat[0].cols];
		int countdata = 0;
		for (int num = 0; num < count; num++)
		{
			for (int j = 0; j < mat[0].cols; j++)
			{
				//std::cout << (int)mat[num].at<uchar>(300, j) << " ";
				data[countdata] = mat[num].at<uchar>(total, j);
				countdata++;
			}
			//std::cout << std::endl;
		}

		cv::Mat mat_temp(count, mat[0].cols, CV_8UC1);
		for (int i = 0; i < mat_temp.rows; ++i)
		{
			uchar *p = mat_temp.ptr<uchar>(mat_temp.rows - 1 - i);
			for (int j = 0; j < mat_temp.cols; ++j) //M.ptr<uchar>(i)返回的是第 i 行像素点的首地址
				p[j] = data[i * mat[0].cols + j];
		}

		char BmpName[256];
		sprintf_s(BmpName, "%s%s%06d.bmp", chSubImageName, "C", total + 1);
		cv::imwrite(BmpName, mat_temp);

		delete data;
	}

	//构造矢状位图像数据
	uchar* data = new uchar[count * mat[0].cols];
	int countdata = 0;
	for (int num = 0; num < count; num++)
	{
		for (int j = 0; j < mat[0].cols; j++)
		{
			//std::cout << (int)mat[num].at<uchar>(300, j) << " ";
			data[countdata] = mat[num].at<uchar>(j, 300);
			countdata++;
		}
		//std::cout << std::endl;
	}

	cv::Mat mat_temp(count, mat[0].cols, CV_8UC1);
	for (int i = 0; i < mat_temp.rows; ++i)
	{
		uchar *p = mat_temp.ptr<uchar>(mat_temp.rows - 1 - i);
		for (int j = 0; j < mat_temp.cols; ++j) //M.ptr<uchar>(i)返回的是第 i 行像素点的首地址
			p[j] = data[i * mat[0].cols + j];
	}

	cv::imwrite("C:\\Users\\Administrator\\Desktop\\test.bmp", mat_temp);

	delete data;
	

	return count;
}

void DcmFileProcess::createCache()
{
	std::string dirName = GetExePath();
	dirName += "\\cache";
	_mkdir(dirName.c_str());
}

void DcmFileProcess::deleteCache()
{
	std::string dirName = GetExePath();
	dirName += "\\cache";
	//构造类对象  
	CStatDir statdir;
	std::vector<std::string> AllDcmFile;
	if (!statdir.SetInitDir(dirName.c_str()))
	{
		puts("目录不存在");
	}
	//开始遍历,获取该文件夹下所有dcm格式图像，一般为一个病患
	AllDcmFile.clear();
	AllDcmFile = statdir.BeginBrowseFilenames("*.bmp");
	for (auto iter = AllDcmFile.cbegin(); iter != AllDcmFile.cend(); iter++)
	{
		remove((*iter).c_str());
	}
	rmdir(dirName.c_str());
}


int DcmFileProcess::readAllFile(const char* FilePath, std::vector<std::string> result)
{
	//构造类对象  
	CStatDir statdir;

	//设置要遍历的目录  
	if (!statdir.SetInitDir(FilePath))
	{
		puts("目录不存在");
		return 0;
	}
	result.clear();
	//开始遍历  
	result = statdir.BeginBrowseFilenames("*.*");
	for (std::vector<std::string>::const_iterator it = result.begin(); it < result.end(); ++it)
		std::cout << *it << std::endl;

	return result.size();
	
}