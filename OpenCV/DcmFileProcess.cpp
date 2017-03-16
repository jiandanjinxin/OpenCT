#include "DcmFileProcess.h"
#include "DcmFileFormat.h"

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
	//���������ļ���
	createCache();
	std::string dirName = GetExePath();
	dirName += "\\cache\\";
	for (auto iter = AllDcmFile.cbegin(); iter != AllDcmFile.cend(); iter++)
	{
		// ��ȡԭʼdcmͼ���ǰ׺Ŀ¼
		std::string subImageName = dirName;
		// ��ԭʼdcmͼ���stringת��Ϊchar*
		const char *chImageName = (*iter).c_str();
		// ��ȡԭʼdcmͼ���ǰ׺Ŀ¼��stringת��Ϊchar*
		const char *chSubImageName = subImageName.c_str();
		// ����dcmtkʵ����
		THU_STD_NAMESPACE::TDcmFileFormat dcm = THU_STD_NAMESPACE::TDcmFileFormat(chImageName);
		// ��ȡdcmͼ���InstanceNumber
		int InstancePosition = dcm.getPositionNumber();
		// ����bmpͼ��洢·��
		char BmpName[256];
		sprintf_s(BmpName, "%s%06d.bmp", chSubImageName, InstancePosition);
		// ����ͼ��ת��
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