#pragma once

#include "afxtempl.h"


typedef long			int32;
typedef unsigned long	uint32;
typedef short			int16;
typedef unsigned short	uint16;
typedef char			int8;
typedef unsigned char	uint8;
typedef  enum {
	unknown,
	AE, AS, CS, DA, DS, DT, IS, LO, LT, PN, SH, ST,
	TM, UI,
	AT,//uint32
	FL,//int32
	FD,//int64
	OB,//
	OW,//
	SL,//int32
	SQ,//
	SS,//int16
	UL,//uint32
	US//uint16
} vType;
class CDCMImage
{
public:
	CDCMImage();
	virtual ~CDCMImage();
	CDC m_memdc;
	int    HandleItem(FILE*& fp, int group, int element);
	int    GetPixelData(CString FilePath);
	int     read16(FILE* fp);
	int     read32(FILE* fp);
	int little_endian_specified;
	int explicit_specified;
	BOOL   m_pixel_success;
	int    HandleDataSet(FILE*& fp, int group, int element);
	int slope_and_intercept_specified;
	int number_of_frames_specified;
	int frame_time_specified;
	int frame_time_vector_specified;
	int photometric_specified;
	int window_center_specified;           /*  1 windowCenter is given        */
										  /*  0 windowCenter not given       */
	int window_width_specified;            /*  1 windowsWidth  is given       */
										  /*  0 windowsWidth  not given      */

	void    flip_16bit_data(uint8* buff, int width, int height);
	int32* to_32bit_data(uint8* buff, int width, int height);
	int32* to_32bit_data(int8* buff, int width, int height);
	int32* to_32bit_data(uint16* buff, int width, int height);
	int32* to_32bit_data(int16* buff, int width, int height);
	void    handle_photometric_interpretation(int32* tmp, int width, int height);
	void   apply_slope_intercept(int32* buff, int width, int height, double slope, double intercept);
	//与象素显示有关的变量
	int		PixelRepresentation;      /*if 0 unsigned integer,1 2's complement*/
	int		PhotometricInterpretation;/*if 1 MONOCHROME1,2 MONOCHROME2,3 others*/
	char	TransferSyntax[100];      /*1.2.840.10008.1.2 Default Implicit VR little Endian*/
									  /*1.2.840.10008.1.2.1       Explicit VR little Endian*/
									  /*1.2.840.10008.1.2.2       Explicit VR Big    Endian*/
	int		BitsAllocated;
	int		BitsStored;
	int		HighBit;
	float  	WindowCenter;             /*窗位*/
	float	WindowWidth;			  /*窗宽*/
	int     SamplesPerPixel;         /*bytes  per Pixel*/
	int     MaxPixelValue;
	int     MinPixelValue;
	int     m_Slope, m_Intercept;

	//与一副图象的显示相关的变量
	int		Rows;
	int		Colums;
	int		ImageWidth;               /*图象的宽度=Colums*/
	int		ImageHeight;			  /*图象的高度=Rows*/
	int     BoxWidth, BoxHeight, BoxXpos, BoxYpos;
	int     WatchXpos, WatchYpos;//显示图象的左上角坐标
	int     WatchWidth, WatchHeight;//Watch窗口的宽和高
	int     SltXpos, SltYpos, SltWidth, SltHeight;
	int     SltImgCenX, SltImgCenY;//选中区域内图象的中心//
	//与多帧图象显示相关的变量
	int		NumberOfFrames;
	float	FrameTime;
	int     FrameTimeVector;
	int		RepetitionTime;
	int		EchoTime;
	float   FrameDelay;
	//==========================下面是显示信息====================//
	//Image信息
	char    ImageNumber[100];
	char	ImageDate[100];
	char	ImageTime[100];
	char	ImageType[100];
	//病人信息
	char	PatientName[100];
	char	PatientID[100];
	char	PatientAge[100];
	char	PatientSex[100];

	//Study信息
	char	StudyID[100];
	char    StudyTime[100];
	char	StudyDate[100];

	//Series
	char    SeriesNumber[100];
	char    SeriesDate[100];
	char    SeriesTime[100];

	char    AcquisitionDate[100];
	char    AcquisitionTime[100];

	//others
	char	Modality[100];
	char	Manufacturer[100];
	char    ManufacturerMode[100];
	char	InstitutionAddress[100];
	char	InstitutionName[100];

	char    DFOV[100];
	char    KVP[100];//电压
	char    TubeCurrent[100];//电流
	char    DetectorTilt[100];
	char    BodyPart[100];
	char    SliceLocation[100];
	char    SliceThickness[100];

	double    ImageOrientation[6];
	char      OrientMark[4];
	char      SaveOrientMark[4];
	//==========================显示信息末尾==========================//
	//Pixel DATA IS STORED HERE!
	int32* PixelValueData;//the true value,int 16 bits
	int32* SavePixelValueData;
	int32* WatchData;

	int            OW_specified;/*if 1 OW,o OB*/
	int            Selected;//这副图象是否被选中
	int          Deleted;
	int          StateChanged;//是否改变，是否要更新
	COLORREF     BoxColor;//边框的染色
	int          DisplayMethod;
	int          NoRegionX, NoRegionY, NoRegionWidth, NoRegionHeight;

	double       m_PixelSizeX, m_PixelSizeY;
	int          m_TopValue;
	BOOL         m_bShowInfo;
	BOOL         m_bContrast;
	//============下面的对恢复原状态有用=========================//
	float         SaveWinWidth, SaveWinCenter;
	double        m_SavePixelSizeX, m_SavePixelSizeY;
	int           m_SaveImageWidth, m_SaveImageHeight;
	int           m_RotateState;
	BOOL          m_bUpDown, m_bLeftRight;
	int           SaveSltXpos, SaveSltYpos, SaveSltWidth, SaveSltHeight;
	//=============计算定位线===================================//
	CArray<int, int&> m_ImageNum_Array;//定位线对应的图象号
	CArray<int, int&> m_SeriesNum_Array;//定位线对应的序列号
	int             m_Highlight_ImageNum;
	int             m_Highlight_SeriesNum;
	double          ImagePosition[3];
	BOOL            m_bLocalizer;
	BOOL            m_bLocalizerOK;
	BOOL            m_bShow_One_Localizer;
	int             m_NumOfLocalizers;
	int             m_bShowLocalizers;
	//=============标记==========================================//
	CObArray        NotationArray;
	int             m_NumberOfNo;//标记的数量//
	int             m_CurrentNo;
	BOOL            m_bShowNo;
};

