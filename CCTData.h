#pragma once
class CCTData
{
public:
	CCTData();
	virtual ~CCTData();
	long*** voxelValueData;//存储全部切片
	int         m_xSize;
	int         m_ySize;
	int         m_zSize;
	int         m_Scanorient;
	double      m_xPixelSize;
	double      m_yPixelSize;
	double      m_zPixelSize;
	PWORD       m_pPixel;
	PWORD       m_pPixel2; 
	char        m_paname[100];
	char        m_pasex[100];
	char        m_paage[100];
	char        m_bodyname[100];
	int         m_tmpxSize;
	int         m_tmpySize;
	int         m_tmpzSize;
	PWORD       m_tmpPixel;
	bool		m_RoiBoxEnabled, m_Finish;
	double      m_tmpxPixelSize;
	double      m_tmpyPixelSize;
	double      m_tmpzPixelSize;
	static int			m_PminIso, m_PmaxIso;
	static int m_NavminIso, m_NavmaxIso;

	int			xs, ys, xe, ye, zs, ze;	// 当前重构区域
	int         m_CompressRatio;

	int         m_tmpScanorient;
	
public:
	bool ReadCTFile(CString filen);
	bool ReadCTFileLow(PWORD m_HPixel, int m_xsize, int m_ysize, int m_zsize, int m_xpixelsize, int m_ypixelsize, int m_zpixelsize, CString m_paname, CString m_pasex, CString m_paage, CString m_bodyname);
	void scale1_4(PWORD m_tmpPixel);
};

