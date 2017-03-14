#include "DcmFileProcess.h"

void DcmFileProcess::readAllDcm(const char* FilePath, const std::vector<std::string>& AllDcmFile)
{
	//构造类对象  
	CStatDir statdir;
	//设置要遍历的目录  
	if (!statdir.SetInitDir(FilePath))
	{
		puts("目录不存在");
	}
	//开始遍历,获取该文件夹下所有dcm格式图像，一般为一个病患
	std::vector<std::string> file_vec = statdir.BeginBrowseFilenames("*.dcm");
}