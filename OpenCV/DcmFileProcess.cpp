#include "DcmFileProcess.h"

void DcmFileProcess::readAllDcm(const char* FilePath, const std::vector<std::string>& AllDcmFile)
{
	//���������  
	CStatDir statdir;
	//����Ҫ������Ŀ¼  
	if (!statdir.SetInitDir(FilePath))
	{
		puts("Ŀ¼������");
	}
	//��ʼ����,��ȡ���ļ���������dcm��ʽͼ��һ��Ϊһ������
	std::vector<std::string> file_vec = statdir.BeginBrowseFilenames("*.dcm");
}