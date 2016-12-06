//����CxImage��Դ���ͷ�ļ�
#include "ximage.h"
//����Dcmtk��Դ���ͷ�ļ�
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmdata/dcvrfd.h"
//�����Զ����ͷ�ļ�
#include "HDcmFileFormat.h"
//����ϵͳͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "Shlwapi.h"
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <cstring>

namespace THU_STD_NAMESPACE {

	TDcmFileFormat::TDcmFileFormat()
	{
		DcmFileFormat::DcmFileFormat();
		m_pImageBuffer = NULL;
		m_ImageBufferType = THU_GRAY_DIB;
	};
	TDcmFileFormat::TDcmFileFormat(DcmFileFormat DcmFile)
		:DcmFileFormat(DcmFile), m_pImageBuffer(NULL), m_ImageBufferType(THU_GRAY_DIB)
	{
		//FlushImageBuffer();
	};

	TDcmFileFormat::TDcmFileFormat(DcmFileFormat DcmFile, THUIMAGEBUFFERTYPE ImageBufferType)
		:DcmFileFormat(DcmFile)
	{
		m_pImageBuffer = NULL;
		this->m_ImageBufferType = ImageBufferType;
		//FlushImageBuffer();
	};
	TDcmFileFormat::TDcmFileFormat(const char* pDcmFileName, THUIMAGEBUFFERTYPE ImageBufferType)
	{
		if ((pDcmFileName != NULL) && (strlen(pDcmFileName) > 0))
		{
			m_pImageBuffer = NULL;
			this->m_ImageBufferType = ImageBufferType;
			DcmFileFormat::loadFile(pDcmFileName);
			//FlushImageBuffer();

		}
	};
	TDcmFileFormat::TDcmFileFormat(const TDcmFileFormat &OtherDcmFileFormat)
		:DcmFileFormat(OtherDcmFileFormat)
	{
		this->m_ImageBufferType = OtherDcmFileFormat.m_ImageBufferType;
		int Bits = m_ImageBufferType == THU_GRAY_DIB ? sizeof(BYTE) : 3 * sizeof(BYTE);
		int Size = (const_cast<TDcmFileFormat&>(OtherDcmFileFormat)).getWidth()*(const_cast<TDcmFileFormat&>(OtherDcmFileFormat)).getHeight()*Bits;
		m_pImageBuffer = new BYTE[Size];
		//deep copy of the image buffer;
		memcpy(m_pImageBuffer, OtherDcmFileFormat.m_pImageBuffer, Size);
	};
	TDcmFileFormat& TDcmFileFormat::operator =(const TDcmFileFormat& OtherDcmFileFormat)
	{
		TDcmFileFormat::TDcmFileFormat(OtherDcmFileFormat);
		return *this;
	};

	TDcmFileFormat::~TDcmFileFormat()
	{
		//DcmFileFormat::~DcmFileFormat();
		if (m_pImageBuffer)
			delete[]m_pImageBuffer;
	};



	// the save operation 
	void TDcmFileFormat::saveToDcm(const char* pDcmFileName)
	{
		if ((pDcmFileName != NULL) && (strlen(pDcmFileName) > 0))
		{
			if (checkCompressed())
			{
				DJDecoderRegistration::registerCodecs(); // register JPEG codecs

				DcmDataset *dataset = this->getDataset();
				dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
				if (dataset->canWriteXfer(EXS_LittleEndianExplicit))
				{
					this->saveFile(pDcmFileName, EXS_LittleEndianExplicit);
				}
				DJDecoderRegistration::cleanup(); // deregister JPEG codecs


			}
			else
			{
				this->saveFile(pDcmFileName, EXS_LittleEndianExplicit);
			}
		}

	};
	void TDcmFileFormat::saveToBmp(const char* pBmpFileName)
	{
		if ((pBmpFileName != NULL) && (strlen(pBmpFileName) > 0))
		{
			if (!m_pImageBuffer)
				FlushImageBuffer();
			int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
			//attach the dib file header and dib information header to the image's buffer 
			int ImgSize = (this->getWidth())*(this->getHeight())*sizeof(BYTE)*(Bits / 8);
			int HeadSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+((Bits == 24) ? 0 : sizeof(RGBQUAD)* 256);
			int Size = HeadSize + ImgSize;
			BYTE* pDibImage = new BYTE[Size];
			memset(pDibImage, 0, Size);
			BITMAPFILEHEADER* pBmpFileHeader = this->createBmpFileHeader();
			memcpy(pDibImage, pBmpFileHeader, sizeof(BITMAPFILEHEADER));
			delete pBmpFileHeader;
			BITMAPINFOHEADER* pBmpInfoHeader = this->createBmpInfoHeader();
			memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)], pBmpInfoHeader, sizeof(BITMAPINFOHEADER));
			delete pBmpInfoHeader;
			if (Bits == 8)
			{
				RGBQUAD* pPalette = this->createBmpPalette();
				memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], pPalette, sizeof(RGBQUAD)* 256);
				delete pPalette;

			}
			memcpy(&pDibImage[HeadSize], this->m_pImageBuffer, ImgSize);

			//����CxImage��Դ�⣬�����bmp�ļ���
			CxImage* pImage = new CxImage();
			pImage->Decode(pDibImage, Size, CXIMAGE_FORMAT_BMP);

			int iLength;
			TCHAR tchar[1024] = { 0 };
			iLength = MultiByteToWideChar(CP_ACP, 0, pBmpFileName, strlen(pBmpFileName) + 1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, pBmpFileName, strlen(pBmpFileName) + 1, tchar, iLength);
			pImage->Save(tchar, CXIMAGE_FORMAT_BMP);
			delete[]pDibImage;
			delete pImage;

		}

	};

	void TDcmFileFormat::saveToBmpBySize(const char* pBmpFileName, long ImageWidth, long ImageHeight)
	{
		if ((pBmpFileName != NULL) && (strlen(pBmpFileName) > 0))
		{
			if (!m_pImageBuffer)
				FlushImageBuffer();
			int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
			//attach the dib file header and dib information header to the image's buffer 
			int ImgSize = (this->getWidth())*(this->getHeight())*sizeof(BYTE)*(Bits / 8);
			int HeadSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+((Bits == 24) ? 0 : sizeof(RGBQUAD)* 256);
			int Size = HeadSize + ImgSize;
			BYTE* pDibImage = new BYTE[Size];
			memset(pDibImage, 0, Size);
			BITMAPFILEHEADER* pBmpFileHeader = this->createBmpFileHeader();
			memcpy(pDibImage, pBmpFileHeader, sizeof(BITMAPFILEHEADER));
			delete pBmpFileHeader;
			BITMAPINFOHEADER* pBmpInfoHeader = this->createBmpInfoHeader();
			memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)], pBmpInfoHeader, sizeof(BITMAPINFOHEADER));
			delete pBmpInfoHeader;
			if (Bits == 8)
			{
				RGBQUAD* pPalette = this->createBmpPalette();
				memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], pPalette, sizeof(RGBQUAD)* 256);
				delete pPalette;

			}
			memcpy(&pDibImage[HeadSize], this->m_pImageBuffer, ImgSize);

			//����CxImage��Դ�⣬�����bmp�ļ���
			CxImage* pImage = new CxImage();
			pImage->Decode(pDibImage, Size, CXIMAGE_FORMAT_BMP);

			//�ı�ͼ��Ĵ�С
			pImage->Resample2(ImageWidth, ImageHeight, CxImage::IM_BILINEAR, CxImage::OM_REPEAT);


			int iLength;
			TCHAR tchar[1024] = { 0 };
			iLength = MultiByteToWideChar(CP_ACP, 0, pBmpFileName, strlen(pBmpFileName) + 1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, pBmpFileName, strlen(pBmpFileName) + 1, tchar, iLength);

			pImage->Save(tchar, CXIMAGE_FORMAT_BMP);
			delete[]pDibImage;
			delete pImage;

		}
	};

	void TDcmFileFormat::saveToJpg(const char* pJpgFileName)
	{
		if ((pJpgFileName != NULL) && (strlen(pJpgFileName) > 0))
		{
			if (!m_pImageBuffer)
				FlushImageBuffer();
			int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
			//attach the dib file header and dib information header to the image's buffer 
			int ImgSize = (this->getWidth())*(this->getHeight())*sizeof(BYTE)*(Bits / 8);
			int HeadSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+((Bits == 24) ? 0 : sizeof(RGBQUAD)* 256);
			int Size = HeadSize + ImgSize;
			BYTE* pDibImage = new BYTE[Size];
			memset(pDibImage, 0, Size);

			BITMAPFILEHEADER* pBmpFileHeader = this->createBmpFileHeader();
			memcpy(pDibImage, pBmpFileHeader, sizeof(BITMAPFILEHEADER));
			delete pBmpFileHeader;
			BITMAPINFOHEADER* pBmpInfoHeader = this->createBmpInfoHeader();
			memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)], pBmpInfoHeader, sizeof(BITMAPINFOHEADER));
			delete pBmpInfoHeader;
			if (Bits == 8)
			{
				RGBQUAD* pPalette = this->createBmpPalette();
				memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], pPalette, sizeof(RGBQUAD)* 256);
				delete pPalette;

			}
			memcpy(&pDibImage[HeadSize], this->m_pImageBuffer, ImgSize);

			//����CxImage��Դ�Ᵽ���jpg��ʽ
			CxImage Img(pDibImage, Size, CXIMAGE_FORMAT_BMP);
			CxImage *pImage = new CxImage(pDibImage, Size, CXIMAGE_FORMAT_BMP);

			int iLength;
			TCHAR tchar[1024] = { 0 };
			iLength = MultiByteToWideChar(CP_ACP, 0, pJpgFileName, strlen(pJpgFileName) + 1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, pJpgFileName, strlen(pJpgFileName) + 1, tchar, iLength);

			pImage->Save(tchar, CXIMAGE_FORMAT_JPG);
			delete pImage;
			//mImage->~CxImage();
			delete[]pDibImage;

		}

	};

	void TDcmFileFormat::saveToJpgBySize(const char* pJpgFileName, long ImageWidth, long ImageHeight)
	{
		if ((pJpgFileName != NULL) && (strlen(pJpgFileName) > 0))
		{
			if (!m_pImageBuffer)
				FlushImageBuffer();
			int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
			//attach the dib file header and dib information header to the image's buffer 
			int ImgSize = (this->getWidth())*(this->getHeight())*sizeof(BYTE)*(Bits / 8);
			int HeadSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+((Bits == 24) ? 0 : sizeof(RGBQUAD)* 256);
			int Size = HeadSize + ImgSize;
			BYTE* pDibImage = new BYTE[Size];
			memset(pDibImage, 0, Size);

			BITMAPFILEHEADER* pBmpFileHeader = this->createBmpFileHeader();
			memcpy(pDibImage, pBmpFileHeader, sizeof(BITMAPFILEHEADER));
			delete pBmpFileHeader;
			BITMAPINFOHEADER* pBmpInfoHeader = this->createBmpInfoHeader();
			memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)], pBmpInfoHeader, sizeof(BITMAPINFOHEADER));
			delete pBmpInfoHeader;
			if (Bits == 8)
			{
				RGBQUAD* pPalette = this->createBmpPalette();
				memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], pPalette, sizeof(RGBQUAD)* 256);
				delete pPalette;

			}
			memcpy(&pDibImage[HeadSize], this->m_pImageBuffer, ImgSize);

			//����CxImage��Դ�Ᵽ���jpg��ʽ
			CxImage Img(pDibImage, Size, CXIMAGE_FORMAT_BMP);
			CxImage *pImage = new CxImage(pDibImage, Size, CXIMAGE_FORMAT_BMP);

			//2013-05-21
			//�ı�ͼ��Ĵ�С
			pImage->Resample2(ImageWidth, ImageHeight, CxImage::IM_BILINEAR, CxImage::OM_REPEAT);

			int iLength;
			TCHAR tchar[1024] = { 0 };
			iLength = MultiByteToWideChar(CP_ACP, 0, pJpgFileName, strlen(pJpgFileName) + 1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, pJpgFileName, strlen(pJpgFileName) + 1, tchar, iLength);

			pImage->Save(tchar, CXIMAGE_FORMAT_JPG);
			delete pImage;
			//mImage->~CxImage();
			delete[]pDibImage;

		}

	}



	void TDcmFileFormat::FlushImageBuffer()//ˢ��ͼ�񻺳���������ͼ����д������Ҫˢ��ͼ�񻺳���
	{
		//int Bits=m_ImageBufferType==THU_GRAY_DIB?8:24;

		OFString DcmFileType;
		this->getDataset()->findAndGetOFString(DCM_PhotometricInterpretation, DcmFileType);
		int width = this->getWidth();
		int height = this->getHeight();

		if (!m_pImageBuffer)//���ͼ�񻺳���Ϊ�գ���ô����ǰdcm�ļ���ͼ�����ݴ洢��ͼ�񻺳����������ѹ����ʽ����Ҫ���н�ѹ����
		{
			if (checkCompressed())
			{
				DJDecoderRegistration::registerCodecs(); // register JPEG codecs


				//2013-06-20
				//�޸ģ����ͼ��Photometric Interpretation����Ԫ���б�����ͼ������

				//OFString DcmFileType;
				//this->getDataset()->findAndGetOFString(DCM_PhotometricInterpretation,DcmFileType);
				if (strcmp(DcmFileType.c_str(), ("RGB")))
				{
					int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
					//�����MONOCHROME1����MONOCHROME2����ͼ��
					DcmDataset *dataset = this->getDataset();
					dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
					DicomImage *di = new DicomImage((DcmFileFormat*)this, EXS_LittleEndianExplicit/*xfer*/, CIF_AcrNemaCompatibility, 0, 1);
					if (di->getStatus() != EIS_Normal)
						exit(0);
					di->setWindow(this->getWindowCenter(), this->getWindowWidth());
					di->createWindowsDIB(m_pImageBuffer, 0, 0, Bits, 1, 1);//����bmp��ʽ��ͼ�񻺳���

					delete di;
					//di->~DicomImage();
				}
				else
				{
					//�����RGBͼ��
					this->m_ImageBufferType = THU_RGB_DIB;
					int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;

					int ImgSize = width*height*sizeof(BYTE)*(Bits / 8);
					BYTE* pDibImage = new BYTE[ImgSize];
					memset(pDibImage, 0, ImgSize);

					//ֱ�ӽ���DCM�ļ���BMP�ļ���ת��
					//ת�������
					//			1)RGB��ɫ˳����ȷ��
					//			2)ͼ�����µߵ���
					//memcpy(&pDibImage[HeadSize],pPixel,ImgSize);



					//�޸ģ�2013-06-18
					//��DCM��BMP��ת��ʱ�����ֶ�����
					//�����������������棺
					//					1)RGB����ΪBGR
					//					2)����λ�õߵ�
					UINT8* pPixel = (UINT8*)getPixelData();

					int realWidth = (width % 4 == 0) ? width : ((int)(width / 4) + 1) * 4;
					for (int j = 0; j < height; ++j)
					{
						UINT8* pSource = pPixel + (height - 1 - j)*width * 3;
						UINT8* pDest = pDibImage + j*width * 3;
						for (int i = 0; i < realWidth; ++i)
						{
							if (i < width)
							{
								//RGB�ߵ�ΪBGR
								pDest[i * 3] = pSource[i * 3 + 2];
								pDest[i * 3 + 1] = pSource[i * 3 + 1];
								pDest[i * 3 + 2] = pSource[i * 3];
							}
							else
							{
								//����ֽڣ�0
								pDest[i * 3] = 0;
								pDest[i * 3 + 1] = 0;
								pDest[i * 3 + 2] = 0;

							}

						}

					}
					m_pImageBuffer = (void*)pDibImage;

				}
				DJDecoderRegistration::cleanup(); // deregister JPEG codecs

			}
			else
			{
				if (strcmp(DcmFileType.c_str(), ("RGB")))
				{
					int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;

					//�����MONOCHROME1����MONOCHROME2����ͼ��
					DicomImage *di = new DicomImage((DcmFileFormat*)this, EXS_LittleEndianExplicit/*xfer*/, CIF_AcrNemaCompatibility, 0, 1);
					if (di->getStatus() != EIS_Normal)
						exit(0);
					di->setWindow(this->getWindowCenter(), this->getWindowWidth());
					di->createWindowsDIB(m_pImageBuffer, 0, 0, Bits, 1, 1);//����bmp��ʽ��ͼ�񻺳���
					delete di;
					//di->~DicomImage();
				}
				else
				{
					//�����RGBͼ��
					this->m_ImageBufferType = THU_RGB_DIB;
					int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;

					int ImgSize = width*height*sizeof(BYTE)*(Bits / 8);
					BYTE* pDibImage = new BYTE[ImgSize];
					memset(pDibImage, 0, ImgSize);
					//ֱ�ӽ���DCM�ļ���BMP�ļ���ת��
					//ת�������
					//			1)RGB��ɫ˳����ȷ��
					//			2)ͼ�����µߵ���
					//memcpy(&pDibImage[HeadSize],pPixel,ImgSize);



					//�޸ģ�2013-06-18
					//��DCM��BMP��ת��ʱ�����ֶ�����
					//�����������������棺
					//					1)RGB����ΪBGR
					//					2)����λ�õߵ�
					UINT8* pPixel = (UINT8*)getPixelData();

					int realWidth = (width % 4 == 0) ? width : ((int)(width / 4) + 1) * 4;
					for (int j = 0; j < height; ++j)
					{
						UINT8* pSource = pPixel + (height - 1 - j)*width * 3;
						UINT8* pDest = pDibImage + j*width * 3;
						for (int i = 0; i < realWidth; ++i)
						{
							if (i < width)
							{
								//RGB�ߵ�ΪBGR
								pDest[i * 3] = pSource[i * 3 + 2];
								pDest[i * 3 + 1] = pSource[i * 3 + 1];
								pDest[i * 3 + 2] = pSource[i * 3];
							}
							else
							{
								//����ֽڣ�0
								pDest[i * 3] = 0;
								pDest[i * 3 + 1] = 0;
								pDest[i * 3 + 2] = 0;

							}

						}

					}
					m_pImageBuffer = (void*)pDibImage;

				}
			}
		}
		else//����ˢ��ͼ�񻺳���
		{
			if (checkCompressed())
			{
				DJDecoderRegistration::registerCodecs(); // register JPEG codecs

				//int Size=this->getWidth()*this->getHeight()*((Bits/8)*sizeof(BYTE));
				//memset(m_pImageBuffer,0,Size);
				//DcmDataset *dataset=this->getDataset();
				//dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
				//DicomImage *di=new DicomImage((DcmFileFormat*)this,EXS_LittleEndianExplicit/*xfer*/,CIF_AcrNemaCompatibility,0,1);
				//if(di->getStatus()!=EIS_Normal)
				//	exit(0);
				//di->setWindow(this->getWindowCenter(),this->getWindowWidth());
				//di->createWindowsDIB(m_pImageBuffer,Size,0,Bits,1,1);//����bmp��ʽ��ͼ�񻺳���
				//delete di;
				////di->~DicomImage();

				if (strcmp(DcmFileType.c_str(), ("RGB")))
				{
					int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
					//�����MONOCHROME1����MONOCHROME2����ͼ��
					DcmDataset *dataset = this->getDataset();
					dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
					DicomImage *di = new DicomImage((DcmFileFormat*)this, EXS_LittleEndianExplicit/*xfer*/, CIF_AcrNemaCompatibility, 0, 1);
					if (di->getStatus() != EIS_Normal)
						exit(0);
					di->setWindow(this->getWindowCenter(), this->getWindowWidth());
					di->createWindowsDIB(m_pImageBuffer, 0, 0, Bits, 1, 1);//����bmp��ʽ��ͼ�񻺳���

					delete di;
					//di->~DicomImage();
				}
				else
				{
					//�����RGBͼ��
					this->m_ImageBufferType = THU_RGB_DIB;
					int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;

					int ImgSize = width*height*sizeof(BYTE)*(Bits / 8);
					BYTE* pDibImage = new BYTE[ImgSize];
					memset(pDibImage, 0, ImgSize);

					//ֱ�ӽ���DCM�ļ���BMP�ļ���ת��
					//ת�������
					//			1)RGB��ɫ˳����ȷ��
					//			2)ͼ�����µߵ���
					//memcpy(&pDibImage[HeadSize],pPixel,ImgSize);



					//�޸ģ�2013-06-18
					//��DCM��BMP��ת��ʱ�����ֶ�����
					//�����������������棺
					//					1)RGB����ΪBGR
					//					2)����λ�õߵ�
					UINT8* pPixel = (UINT8*)getPixelData();

					int realWidth = (width % 4 == 0) ? width : ((int)(width / 4) + 1) * 4;
					for (int j = 0; j < height; ++j)
					{
						UINT8* pSource = pPixel + (height - 1 - j)*width * 3;
						UINT8* pDest = pDibImage + j*width * 3;
						for (int i = 0; i < realWidth; ++i)
						{
							if (i < width)
							{
								//RGB�ߵ�ΪBGR
								pDest[i * 3] = pSource[i * 3 + 2];
								pDest[i * 3 + 1] = pSource[i * 3 + 1];
								pDest[i * 3 + 2] = pSource[i * 3];
							}
							else
							{
								//����ֽڣ�0
								pDest[i * 3] = 0;
								pDest[i * 3 + 1] = 0;
								pDest[i * 3 + 2] = 0;

							}

						}

					}
					m_pImageBuffer = (void*)pDibImage;

				}


				DJDecoderRegistration::cleanup(); // deregister JPEG codecs

			}
			else
			{
				//int Size=this->getWidth()*this->getHeight()*((Bits/8)*sizeof(BYTE));
				//memset(m_pImageBuffer,0,Size);
				//DicomImage *di=new DicomImage((DcmFileFormat*)this,EXS_LittleEndianExplicit/*xfer*/,CIF_AcrNemaCompatibility,0,1);
				//if(di->getStatus()!=EIS_Normal)
				//	exit(0);
				//di->setWindow(this->getWindowCenter(),this->getWindowWidth());
				//di->createWindowsDIB(m_pImageBuffer,Size,0,Bits,1,1);//����bmp��ʽ��ͼ�񻺳���
				//delete di;
				////di->~DicomImage();
				if (strcmp(DcmFileType.c_str(), ("RGB")))
				{
					int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
					//�����MONOCHROME1����MONOCHROME2����ͼ��
					DcmDataset *dataset = this->getDataset();
					dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
					DicomImage *di = new DicomImage((DcmFileFormat*)this, EXS_LittleEndianExplicit/*xfer*/, CIF_AcrNemaCompatibility, 0, 1);
					if (di->getStatus() != EIS_Normal)
						exit(0);
					di->setWindow(this->getWindowCenter(), this->getWindowWidth());
					di->createWindowsDIB(m_pImageBuffer, 0, 0, Bits, 1, 1);//����bmp��ʽ��ͼ�񻺳���

					delete di;
					//di->~DicomImage();
				}
				else
				{
					//�����RGBͼ��
					this->m_ImageBufferType = THU_RGB_DIB;
					int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;

					int ImgSize = width*height*sizeof(BYTE)*(Bits / 8);
					BYTE* pDibImage = new BYTE[ImgSize];
					memset(pDibImage, 0, ImgSize);

					//ֱ�ӽ���DCM�ļ���BMP�ļ���ת��
					//ת�������
					//			1)RGB��ɫ˳����ȷ��
					//			2)ͼ�����µߵ���
					//memcpy(&pDibImage[HeadSize],pPixel,ImgSize);



					//�޸ģ�2013-06-18
					//��DCM��BMP��ת��ʱ�����ֶ�����
					//�����������������棺
					//					1)RGB����ΪBGR
					//					2)����λ�õߵ�
					UINT8* pPixel = (UINT8*)getPixelData();

					int realWidth = (width % 4 == 0) ? width : ((int)(width / 4) + 1) * 4;
					for (int j = 0; j < height; ++j)
					{
						UINT8* pSource = pPixel + (height - 1 - j)*width * 3;
						UINT8* pDest = pDibImage + j*width * 3;
						for (int i = 0; i < realWidth; ++i)
						{
							if (i < width)
							{
								//RGB�ߵ�ΪBGR
								pDest[i * 3] = pSource[i * 3 + 2];
								pDest[i * 3 + 1] = pSource[i * 3 + 1];
								pDest[i * 3 + 2] = pSource[i * 3];
							}
							else
							{
								//����ֽڣ�0
								pDest[i * 3] = 0;
								pDest[i * 3 + 1] = 0;
								pDest[i * 3 + 2] = 0;

							}

						}

					}
					m_pImageBuffer = (void*)pDibImage;

				}

			}

		}

	}


	void TDcmFileFormat::setWindow()
	{
		int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
		if (checkCompressed())
		{
			DJDecoderRegistration::registerCodecs(); // register JPEG codecs
			this->getDataset()->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
			DicomImage *pdcmImage = new DicomImage((DcmFileFormat*)this, EXS_LittleEndianExplicit, CIF_AcrNemaCompatibility, 0, 1);
			if (pdcmImage->getStatus() != EIS_Normal)
				exit(0);
			pdcmImage->setWindow(this->getWindowCenter(), this->getWindowWidth());
			std::cout << this->getWindowCenter() << " " << this->getWindowWidth() << std::endl;
			//ˢ��ͼ�񻺳���
			if (!m_pImageBuffer)
			{
				pdcmImage->createWindowsDIB(m_pImageBuffer, 0, 0, Bits, 1, 1);//����bmp��ʽ��ͼ�񻺳���

			}
			else
			{
				int Size = this->getWidth()*this->getHeight()*((Bits / 8)*sizeof(BYTE));
				pdcmImage->createWindowsDIB(m_pImageBuffer, Size, 0, Bits, 1, 1);
			}
			delete pdcmImage;
			//pdcmImage->~DicomImage();
			DJDecoderRegistration::cleanup(); // deregister JPEG codecs

		}
		else
		{
			DicomImage *pdcmImage = new DicomImage((DcmFileFormat*)this, EXS_LittleEndianExplicit, CIF_AcrNemaCompatibility, 0, 1);
			if (pdcmImage->getStatus() != EIS_Normal)
				exit(0);
			pdcmImage->setWindow(this->getWindowCenter(), this->getWindowWidth());
			//ˢ��ͼ�񻺳���
			if (!m_pImageBuffer)
			{
				pdcmImage->createWindowsDIB(m_pImageBuffer, 0, 0, Bits, 1, 1);//����bmp��ʽ��ͼ�񻺳���

			}
			else
			{
				int Size = this->getWidth()*this->getHeight()*((Bits / 8)*sizeof(BYTE));
				pdcmImage->createWindowsDIB(m_pImageBuffer, Size, 0, Bits, 1, 1);
			}
			delete pdcmImage;
			//pdcmImage->~DicomImage();

		}

	}
	void TDcmFileFormat::setWindow(const double WindowCenter, const double WindowWidth)
	{

		int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
		if (checkCompressed())
		{
			DJDecoderRegistration::registerCodecs(); // register JPEG codecs

			this->getDataset()->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
			DicomImage *pdcmImage = new DicomImage((DcmFileFormat*)this, EXS_LittleEndianExplicit, CIF_AcrNemaCompatibility, 0, 1);
			if (pdcmImage->getStatus() != EIS_Normal)
				exit(0);
			pdcmImage->setWindow(WindowCenter, WindowWidth);
			//ˢ��ͼ�񻺳���
			if (!m_pImageBuffer)
			{
				pdcmImage->createWindowsDIB(m_pImageBuffer, 0, 0, Bits, 1, 1);//����bmp��ʽ��ͼ�񻺳���

			}
			else
			{
				int Size = this->getWidth()*this->getHeight()*((Bits / 8)*sizeof(BYTE));
				pdcmImage->createWindowsDIB(m_pImageBuffer, Size, 0, Bits, 1, 1);
			}
			delete pdcmImage;
			//pdcmImage->~DicomImage();

			DJDecoderRegistration::cleanup(); // deregister JPEG codecs

		}
		else
		{
			DicomImage *pdcmImage = new DicomImage((DcmFileFormat*)this, EXS_LittleEndianExplicit, CIF_AcrNemaCompatibility, 0, 1);
			if (pdcmImage->getStatus() != EIS_Normal)
				exit(0);
			pdcmImage->setWindow(WindowCenter, WindowWidth);
			//ˢ��ͼ�񻺳���
			if (!m_pImageBuffer)
			{
				pdcmImage->createWindowsDIB(m_pImageBuffer, 0, 0, Bits, 1, 1);//����bmp��ʽ��ͼ�񻺳���

			}
			else
			{
				int Size = this->getWidth()*this->getHeight()*((Bits / 8)*sizeof(BYTE));
				pdcmImage->createWindowsDIB(m_pImageBuffer, Size, 0, Bits, 1, 1);
			}
			delete pdcmImage;
			//pdcmImage->~DicomImage();

		}
	}
	bool TDcmFileFormat::checkCompressed()
	{
		DcmDataset *dataset = this->getDataset();
		E_TransferSyntax xfer = dataset->getOriginalXfer();
		if (xfer == EXS_LittleEndianExplicit)
			return 0;
		else
			return 1;

	}

	BITMAPFILEHEADER* TDcmFileFormat::createBmpFileHeader()
	{
		BITMAPFILEHEADER* pFileHeader = new BITMAPFILEHEADER;

		int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
		pFileHeader->bfType = 0x4d42;
		pFileHeader->bfReserved1 = 0;
		pFileHeader->bfReserved2 = 0;
		pFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+((Bits == 24) ? 0 : sizeof(RGBQUAD)* 256);
		pFileHeader->bfSize = pFileHeader->bfOffBits + (Bits / 8)*this->getWidth()*this->getHeight();


		return pFileHeader;
	};
	BITMAPINFOHEADER* TDcmFileFormat::createBmpInfoHeader()
	{
		BITMAPINFOHEADER* pInfoHeader = new BITMAPINFOHEADER;
		int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
		pInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
		pInfoHeader->biWidth = this->getWidth();
		pInfoHeader->biHeight = this->getHeight();
		pInfoHeader->biPlanes = 1;
		pInfoHeader->biBitCount = Bits;
		pInfoHeader->biCompression = 0;
		pInfoHeader->biSizeImage = (Bits / 8)*sizeof(BYTE)*this->getWidth()*this->getHeight();
		pInfoHeader->biXPelsPerMeter = 0;
		pInfoHeader->biYPelsPerMeter = 0;
		pInfoHeader->biClrUsed = 0;
		pInfoHeader->biClrImportant = 0;

		return pInfoHeader;
	};
	RGBQUAD* TDcmFileFormat::createBmpPalette()
	{
		RGBQUAD* pPalette = new RGBQUAD[256];
		memset(pPalette, 0, sizeof(RGBQUAD)* 256);
		for (int i = 0; i < 256; ++i)
		{
			pPalette[i].rgbBlue = i;
			pPalette[i].rgbGreen = i;
			pPalette[i].rgbRed = i;
		}

		return pPalette;

	}


	void* TDcmFileFormat::getPixelData()	//�����ʵ��������ָ��
	{
		if (checkCompressed())
		{

			DJDecoderRegistration::registerCodecs(); // register JPEG codecs
			DcmDataset *dataset = this->getDataset();
			// decompress data set if compressed
			dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
			DcmElement* element = NULL;
			dataset->findAndGetElement(DCM_PixelData, element);
			unsigned char* pImage = NULL;
			element->getUint8Array(pImage);
			DJDecoderRegistration::cleanup(); // deregister JPEG codecs
			return pImage;

		}
		else
		{
			DcmDataset *dataset = this->getDataset();
			// decompress data set if compressed
			dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
			DcmElement* element = NULL;
			dataset->findAndGetElement(DCM_PixelData, element);
			unsigned char* pImage = NULL;
			element->getUint8Array(pImage);
			return pImage;

		}
	}


	void TDcmFileFormat::getImageStream(BYTE** pImageStream, long& StreamSize, THUIMAGESTREAMTYPE Type/*=THU_JPEG*/)
	{

		if (!m_pImageBuffer)
			FlushImageBuffer();
		int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
		//attach the dib file header and dib information header to the image's buffer 
		int ImgSize = (this->getWidth())*(this->getHeight())*sizeof(BYTE)*(Bits / 8);
		int HeadSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+((Bits == 24) ? 0 : sizeof(RGBQUAD)* 256);
		int Size = HeadSize + ImgSize;
		BYTE* pDibImage = new BYTE[Size];
		memset(pDibImage, 0, Size);

		BITMAPFILEHEADER* pBmpFileHeader = this->createBmpFileHeader();
		memcpy(pDibImage, pBmpFileHeader, sizeof(BITMAPFILEHEADER));
		delete pBmpFileHeader;
		BITMAPINFOHEADER* pBmpInfoHeader = this->createBmpInfoHeader();
		memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)], pBmpInfoHeader, sizeof(BITMAPINFOHEADER));
		delete pBmpInfoHeader;
		if (Bits == 8)
		{
			RGBQUAD* pPalette = this->createBmpPalette();
			memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], pPalette, sizeof(RGBQUAD)* 256);
			delete pPalette;

		}
		memcpy(&pDibImage[HeadSize], this->m_pImageBuffer, ImgSize);

		//����CxImage��Դ�Ᵽ���jpg��ʽ
		CxImage *pImage = new CxImage(pDibImage, Size, CXIMAGE_FORMAT_BMP);
		//make the temp space for the image stream
		BYTE* buffer = 0;
		int32_t msize = 0;
		switch (Type)
		{
		case THU_JPEG:
			pImage->Encode(buffer, msize, CXIMAGE_FORMAT_JPG);
			(*pImageStream) = new BYTE[msize + 1];
			memcpy((*pImageStream), buffer, msize);
			(*pImageStream)[msize + 1] = 0;
			StreamSize = msize;
			delete buffer;
			break;
		case THU_BMP:
			pImage->Encode(buffer, msize, CXIMAGE_FORMAT_BMP);
			(*pImageStream) = new BYTE[msize + 1];
			memcpy((*pImageStream), buffer, msize);
			(*pImageStream)[msize + 1] = 0;
			StreamSize = msize;
			delete buffer;
			break;
		}
		delete pImage;
		delete[]pDibImage;

	}

	//void TDcmFileFormat::getImageStreamBySize(BYTE** pImageStream,long& StreamSize,long ImageWidth,long ImageHeight,THUIMAGESTREAMTYPE Type/*=THU_JPEG*/)
	void TDcmFileFormat::getImageStreamBySize(BYTE** pImageStream, long& StreamSize, long ImageWidth, long ImageHeight, THUIMAGESTREAMTYPE Type/*=THU_JPEG*/)
	{

		if (!m_pImageBuffer)
			FlushImageBuffer();
		int Bits = m_ImageBufferType == THU_GRAY_DIB ? 8 : 24;
		//attach the dib file header and dib information header to the image's buffer 
		int ImgSize = (this->getWidth())*(this->getHeight())*sizeof(BYTE)*(Bits / 8);
		int HeadSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+((Bits == 24) ? 0 : sizeof(RGBQUAD)* 256);
		int Size = HeadSize + ImgSize;
		BYTE* pDibImage = new BYTE[Size];
		memset(pDibImage, 0, Size);

		BITMAPFILEHEADER* pBmpFileHeader = this->createBmpFileHeader();
		memcpy(pDibImage, pBmpFileHeader, sizeof(BITMAPFILEHEADER));
		delete pBmpFileHeader;
		BITMAPINFOHEADER* pBmpInfoHeader = this->createBmpInfoHeader();
		memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)], pBmpInfoHeader, sizeof(BITMAPINFOHEADER));
		delete pBmpInfoHeader;
		if (Bits == 8)
		{
			RGBQUAD* pPalette = this->createBmpPalette();
			memcpy(&pDibImage[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], pPalette, sizeof(RGBQUAD)* 256);
			delete pPalette;

		}
		memcpy(&pDibImage[HeadSize], this->m_pImageBuffer, ImgSize);

		//����CxImage��Դ�Ᵽ���jpg��ʽ
		CxImage *pImage = new CxImage(pDibImage, Size, CXIMAGE_FORMAT_BMP);

		//�ı�ͼ��Ĵ�С
		pImage->Resample2(ImageWidth, ImageHeight, CxImage::IM_BILINEAR, CxImage::OM_REPEAT);
		//make the temp space for the image stream
		BYTE* buffer = 0;
		int32_t msize = 0;
		switch (Type)
		{
		case THU_JPEG:
			pImage->Encode(buffer, msize, CXIMAGE_FORMAT_JPG);

			(*pImageStream) = new BYTE[msize + 1];
			memcpy((*pImageStream), buffer, msize);
			(*pImageStream)[msize + 1] = 0;
			StreamSize = msize;
			delete buffer;
			break;
		case THU_BMP:
			pImage->Encode(buffer, msize, CXIMAGE_FORMAT_BMP);
			(*pImageStream) = new BYTE[msize + 1];
			memcpy((*pImageStream), buffer, msize);
			(*pImageStream)[msize + 1] = 0;
			StreamSize = msize;
			delete buffer;
			break;
		}
		delete pImage;
		delete[]pDibImage;

	}

};