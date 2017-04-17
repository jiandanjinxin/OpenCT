#include "Util.h"

// ȱʡ������
CBrowseDir::CBrowseDir()
{
	// �õ�ǰĿ¼��ʼ��m_szInitDir 
	_getcwd(m_szInitDir, _MAX_PATH);

	// ���Ŀ¼�����һ����ĸ����'\',����������һ��'\'  
	int len = strlen(m_szInitDir);
	if (m_szInitDir[len - 1] != '\\')
		strcat_s(m_szInitDir, "\\");
}

// ���ó�ʼĿ¼Ϊdir���������false����ʾĿ¼������
bool CBrowseDir::SetInitDir(const char *dir)
{
	// �Ȱ�dirת��Ϊ����·��
	if (_fullpath(m_szInitDir, dir, _MAX_PATH) == NULL)
		return false;

	// �ж�Ŀ¼�Ƿ����
	if (_chdir(m_szInitDir) != 0)
		return false;

	// ���Ŀ¼�����һ����ĸ����'\',����������һ��'\'  
	int len = strlen(m_szInitDir);
	if (m_szInitDir[len - 1] != '\\')
		strcat_s(m_szInitDir, "\\");

	return true;
}

std::vector<std::string> CBrowseDir::BeginBrowseFilenames(const char *filespec)
{
	ProcessDir(m_szInitDir, NULL);
	return GetDirFilenames(m_szInitDir, filespec);
}

// ��ʼ������ʼĿ¼������Ŀ¼����filespecָ�����͵��ļ�  
// filespec����ʹ��ͨ��� * ?�����ܰ���·��  
// �������false����ʾ�������̱��û���ֹ
bool CBrowseDir::BeginBrowse(const char *filespec)
{
	ProcessDir(m_szInitDir, NULL);
	return BrowseDir(m_szInitDir, filespec);
}

// ����Ŀ¼dir����filespecָ�����ļ�  
// ������Ŀ¼,���õ����ķ���
// �������false,��ʾ��ֹ�����ļ�
bool CBrowseDir::BrowseDir(const char *dir, const char *filespec)
{
	_chdir(dir);

	// ���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(filespec, &fileinfo)) != -1)
	{
		do
		{
			// ����Ƿ�ΪĿ¼
			// ������ǣ�����д���
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy_s(filename, dir);
				strcat_s(filename, fileinfo.name);
				std::cout << filename << std::endl;
				if (!ProcessFile(filename))
					return false;
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	// ����dir�е���Ŀ¼  
	// ��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı���  
	// ��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir
	// ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼  
	// ��_findnextû��Ӱ�� 
	_chdir(dir);
	if ((hFile = _findfirst("*.*", &fileinfo)) != -1)
	{
		do
		{
			// ����ǲ���Ŀ¼  
			// �����,�ټ���ǲ��� . �� ..   
			// �������,���е���
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 &&
					strcmp(fileinfo.name, "..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy_s(subdir, dir);
					strcat_s(subdir, fileinfo.name);
					strcat_s(subdir, "\\");
					ProcessDir(subdir, dir);
					if (!BrowseDir(subdir, filespec))
						return false;
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	return true;
}

std::vector<std::string> CBrowseDir::GetDirFilenames(const char *dir, const char *filespec)
{
	_chdir(dir);
	std::vector<std::string> filename_vector;
	filename_vector.clear();

	// ���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(filespec, &fileinfo)) != -1)
	{
		do
		{
			// ����Ƿ�ΪĿ¼
			// ������ǣ�����д���
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy_s(filename, dir);
				strcat_s(filename, fileinfo.name);
				filename_vector.push_back(filename);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	// ����dir�е���Ŀ¼  
	// ��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı���  
	// ��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir��  
	// ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼  
	// ��_findnextû��Ӱ��
	_chdir(dir);
	if ((hFile = _findfirst("*.*", &fileinfo)) != -1)
	{
		do
		{
			// ����ǲ���Ŀ¼  
			// �����,�ټ���ǲ��� . �� ..   
			// �������,���е���
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 &&
					strcmp(fileinfo.name, "..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy_s(subdir, dir);
					strcat_s(subdir, fileinfo.name);
					strcat_s(subdir, "\\");
					ProcessDir(subdir, dir);
					std::vector<std::string> tmp = GetDirFilenames(subdir, filespec);
					for (std::vector<std::string>::iterator it = tmp.begin(); it < tmp.end(); it++)
						filename_vector.push_back(*it);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}

	return filename_vector;
}


bool CBrowseDir::ProcessFile(const char *filename)
{
	return true;
}

void CBrowseDir::ProcessDir(const char *currentdir, const char *parentdir)
{

}
