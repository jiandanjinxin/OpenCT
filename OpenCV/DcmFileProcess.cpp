#include "DcmFileProcess.h"
#include "DcmFileFormat.h"

#include <direct.h>

std::string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\') + 1)[0] = 0; // 删除文件名，只获得路径字串
	std::string path = szFilePath;

	return path;
}

void DcmFileProcess::readAllDcm(const char* FilePath, std::vector<std::string>& AllDcmFile)
{
	//构造类对象  
	CStatDir statdir;
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
	}

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
	rmdir(dirName.c_str());
}