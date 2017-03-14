#include "DcmFileProcess.h"

#include <direct.h>

std::string GetExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\') + 1)[0] = 0; // ɾ���ļ�����ֻ���·���ִ�
	std::string path = szFilePath;

	return path;
}

void DcmFileProcess::readAllDcm(const char* FilePath, std::vector<std::string>& AllDcmFile)
{
	//���������  
	CStatDir statdir;
	//����Ҫ������Ŀ¼  
	if (!statdir.SetInitDir(FilePath))
	{
		puts("Ŀ¼������");
	}
	//��ʼ����,��ȡ���ļ���������dcm��ʽͼ��һ��Ϊһ������
	AllDcmFile.clear();
	AllDcmFile = statdir.BeginBrowseFilenames("*.dcm");
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