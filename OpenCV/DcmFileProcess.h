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
	//读取当前文件夹下所有dcm图像，返回vector
	static void readAllDcm(const char* FilePath, const std::vector<std::string>& AllDcmFile);
};

#endif