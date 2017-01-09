/*-------------------------------------------------

	�ļ�˵����
	�Զ����dcm�ļ��࣬������dcmtk��Դ��ĵײ�
	dcm�ࣺDcmFileFormat��
	��������������Adapter�����ģʽ����CxImage���
	����ͼ���ʽ���� jpg/bmp/png/tiff �ȣ��ı���ӿ�
	�ںϵ�DcmFileFormat���У��γ��µ��ࡣ

	Author��
	Wang Jianyu
	Date��
	2016-12-01
	-------------------------------------------------*/

#ifndef _TDCMFILEFORMAT_H
#define _TDCMFILEFORMAT_H

//����������DcmFileFormat��ͷ�ļ�����Ϊ�̳�ʱ��������Ҫ֪������Ĵ�С
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"//��Ҫ����ͼ�����ʾ�����������λ
//����ϵͳ�ļ�

namespace THU_STD_NAMESPACE{

	//define the image buffer type
	enum THUIMAGEBUFFERTYPE{
		THU_GRAY_DIB = 8, THU_RGB_DIB = 24
	};
	enum THUIMAGESTREAMTYPE{
		THU_JPEG, THU_BMP
	};


	class TDcmFileFormat :public DcmFileFormat
	{
	public:
		TDcmFileFormat();
		TDcmFileFormat(DcmFileFormat DcmFile);
		TDcmFileFormat(DcmFileFormat DcmFile, THUIMAGEBUFFERTYPE ImageBufferType);
		TDcmFileFormat(const char* pDcmFileName, THUIMAGEBUFFERTYPE ImageBufferType = THU_GRAY_DIB);
		TDcmFileFormat(const TDcmFileFormat& OtherDcmFileFormat);
		TDcmFileFormat& operator =(const TDcmFileFormat& OtherDcmFileFormat);
		virtual ~TDcmFileFormat();

		//����CxImage��Դ��Ľӿ�
	public:

		/*--------------------------------------
		  Func:saveToDcm
		  Purpose:
		  save the result of the operation on dcm file
		  Paras:
		  mDcmFileName,the dcm file name
		  --------------------------------------*/
		void saveToDcm(const char* pDcmFileName);
		/*--------------------------------------
		  Func:saveToBmp
		  Purpose:
		  change the dcm file to bmp format
		  Paras:
		  mBmpFileName:the name of the bmp file
		  bits:the bits of one pixel stored in memory
		  default value is 8 bits,i.e. the buffer is a pallete bmp image;
		  if bits=24,the buffer is a RGB bmp image.
		  --------------------------------------*/

		void saveToBmp(const char* pBmpFileName);
		/*--------------------------------------
		  Func:saveToJpg
		  Purpose:
		  change the dcm file to jpg format
		  Paras:
		  mJpgFileName:the name of the jpg file
		  bits:the bits of one pixel stored in memory
		  default value is 8 bits,i.e. the buffer is a pallete bmp image;
		  if bits=24,the buffer is a RGB bmp image.
		  --------------------------------------*/

		void saveToJpg(const char* pJpgFileName);
		/*--------------------------------------
		  Func:saveToJpgBySize
		  Purpose:
		  change the dcm file to jpg format
		  Paras:
		  mJpgFileName:the name of the jpg file

		  bits:the bits of one pixel stored in memory
		  default value is 8 bits,i.e. the buffer is a pallete bmp image;
		  if bits=24,the buffer is a RGB bmp image.

		  ImageWidth:	the width of the image
		  ImageHeight:	the height of the image

		  --------------------------------------*/
		void saveToJpgBySize(const char* pJpgFileName, long ImageWidth, long ImageHeight);
		/*--------------------------------------
		  Func:saveToBmpBySize
		  Purpose:
		  change the dcm file to jpg format
		  Paras:
		  mJpgFileName:the name of the jpg file

		  bits:the bits of one pixel stored in memory
		  default value is 8 bits,i.e. the buffer is a pallete bmp image;
		  if bits=24,the buffer is a RGB bmp image.

		  ImageWidth:		the width of the image
		  ImageHeight:	the height of the image
		  --------------------------------------*/
		void saveToBmpBySize(const char* pBmpFileName, long ImageWidth, long ImageHeight);



	public://dcmͼ�������Ϣ�ӿ�

		inline unsigned short int getWidth(){
			//�ֶΣ�0028,0010��Rows��VRΪUS��16λ�޷�������
			unsigned short DcmRows;
			this->getDataset()->findAndGetUint16(DCM_Columns, DcmRows);
			return DcmRows;
		};
		inline unsigned short int getHeight(){
			//�ֶΣ�0028.0011)Columns��VRΪUS��16λ�޷�������
			unsigned short DcmColumns;
			this->getDataset()->findAndGetUint16(DCM_Rows, DcmColumns);
			return DcmColumns;
		};
		inline unsigned short int getBitsAllocated(){
			//�ֶΣ�0028,0100��BitsAllocated��VRΪUS��16λ�޷�������
			unsigned short DcmBitsAllocated;
			this->getDataset()->findAndGetUint16(DCM_BitsAllocated, DcmBitsAllocated);
			return DcmBitsAllocated;

		};
		inline unsigned short int getBitsStored(){
			unsigned short DcmBitsStored;
			this->getDataset()->findAndGetUint16(DCM_BitsStored, DcmBitsStored);
			return DcmBitsStored;
		};
		inline unsigned short int getHighBit(){
			unsigned short DcmHighBit;
			this->getDataset()->findAndGetUint16(DCM_HighBit, DcmHighBit);
			return DcmHighBit;
		};
		inline double getWindowWidth(){
			double DcmWindowWidth;
			this->getDataset()->findAndGetFloat64(DCM_WindowWidth, DcmWindowWidth);
			return DcmWindowWidth;

		};
		inline double getWindowCenter(){
			double DcmWindowCenter;
			this->getDataset()->findAndGetFloat64(DCM_WindowCenter, DcmWindowCenter);
			return DcmWindowCenter;
		};
		inline double getRescaleSlope(){
			double DcmRescaleSlope;
			this->getDataset()->findAndGetFloat64(DCM_RescaleSlope, DcmRescaleSlope);
			return DcmRescaleSlope;

		};
		inline double getRescaleIntercept(){
			double DcmRescaleIntercept;
			this->getDataset()->findAndGetFloat64(DCM_RescaleIntercept, DcmRescaleIntercept);
			return DcmRescaleIntercept;

		};
		inline int getPositionNumber(){
			const char *tString;
			this->getDataset()->findAndGetString(DCM_InstanceNumber, tString);
			int position = atoi(tString);

			return position;
		};
		std::string getImagePositionPatient()
		{
			const char *tString;
			this->getDataset()->findAndGetString(DCM_ImagePositionPatient, tString);
			std::string result(tString);
			return result;
		}
		std::string getPixelSpacing()
		{
			const char *tString;
			this->getDataset()->findAndGetString(DCM_PixelSpacing, tString);
			std::string result(tString);
			return result;
		}
		void* getPixelData();//�����ʵ��������ָ��

		/*--------------------------------------
		  Func:getImageStream
		  Purpose:
		  convert the pixel data in the dcm file into JPEG or BMP type,
		  and save as a BYTE stream
		  Paras:
		  imageStream:the BYTE stream of the pixel data in the dcm file,
		  it must be initialized to NULL
		  size:the size of image stream,the starting value is zero
		  type:the type of the image stream
		  return:
		  void
		  --------------------------------------*/

		void getImageStream(BYTE** pImageStream, long& StreamSize, THUIMAGESTREAMTYPE Type = THU_JPEG);
		/*--------------------------------------
		  Func:getImageStreamBySize
		  Purpose:
		  convert the pixel data in the dcm file into JPEG or BMP type, then resample the image
		  in order to change its size to ImageWidth*ImageHeight,and save as a BYTE stream
		  Paras:
		  imageStream:the BYTE stream of the pixel data in the dcm file,
		  it must be initialized to NULL
		  size:the size of image stream,the starting value is zero
		  type:the type of the image stream
		  return:
		  void
		  --------------------------------------*/
		void getImageStreamBySize(BYTE** pImageStream, long& StreamSize, long ImageWidth, long ImageHeight, THUIMAGESTREAMTYPE Type = THU_JPEG);

	public://ͼ������
		
				
		void setWindow();//ʹ��Ĭ�ϵĴ�����λ
		void setWindow(const double WindowCenter, const double WindowWidth);//�Զ��崰����λ

	protected:
		/*--------------------------------------
		  Func:checkCompressed
		  Purpose:
		  check the transfer syntax of the dcm file compressed or not
		  return:
		  0,the dcm file's transfersyntax is uncompressed
		  1,the dcm file's transfersyntax is compressed
		  --------------------------------------*/

		bool checkCompressed();//��⴫�����壬��ͼ���Ƿ���ѹ��ͼ��


		//�ڶ�dcmԭʼ���ݽ����޸ĺ���Ҫ����FlushImagebuffer()����dcm�е�ͼ����Ϣ���ص�ͼ�񻺳����У�Ϊ֮���ͼ���ʽת����׼����
		/*--------------------------------------
		  Func:FlushImageBuffer
		  Purpose:
		  refresh the image buffer,mImageBuffer, store the result of the last operation
		  Paras:
		  bits:the bits of one pixel stored in memory
		  default value is 8 bits,i.e. the buffer is a pallete bmp image;
		  if bits=24,the buffer is a RGB bmp image.
		  Tips:
		  All of constructors of the TDcmFileFormat don't call the FlushImageBuffer() function expilictly,so
		  the image buffer,mImageBuffer,is null.
		  Whenever you want to save your operation on the dcm file to JPG/BMP file, you should call the
		  FlushImageBuffer() function to refresh the image buffer.
		  --------------------------------------*/
		void FlushImageBuffer();//ˢ��ͼ�񻺳���������ͼ����д������Ҫˢ��ͼ�񻺳���,
		/*--------------------------------------
		  Func:createBmpFileHeader
		  Purpose:
		  create the file header of teh image buffer,mImageBuffer
		  Paras:
		  bits:the bits of one pixel stored in memory
		  default value is 8 bits,i.e. the buffer is a pallete bmp image;
		  if bits=24,the buffer is a RGB bmp image.
		  Return:
		  the file header of the image buffer,mImageBuffer
		  --------------------------------------*/

		BITMAPFILEHEADER* createBmpFileHeader();
		/*--------------------------------------
		  Func:createBmpInfoHeader
		  Purpose:
		  create the information header of the image buffer,mImageBuffer
		  Paras:
		  bits:the bits of one pixel stored in memory
		  default value is 8 bits,i.e. the buffer is a pallete bmp image;
		  if bits=24,the buffer is a RGB bmp image.
		  Return:
		  the information header of the image buffer,mImageBuffer
		  --------------------------------------*/
		BITMAPINFOHEADER* createBmpInfoHeader();
		/*--------------------------------------
		  Func:createBmpPalette
		  Purpose:
		  create the palette of the image buffer,mImageBuffer
		  Return:
		  the image buffer's palette
		  --------------------------------------*/
		RGBQUAD* createBmpPalette();


	private:

		void* m_pImageBuffer;//the image buffer,storing the pixel data of dcm file
		THUIMAGEBUFFERTYPE m_ImageBufferType;

	};
};

#endif
