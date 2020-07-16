#include "pch.h"
#include "CCTData.h"
#include "MainFrm.h"
#include "DCMImage.h"
extern int sstart;
extern int slicenum;
extern int send1;

_int8* curveCutFlag;
_int8* LOWcurveCutFlag;
int CCTData::m_PminIso = 0;
int CCTData::m_PmaxIso = 0;
int CCTData::m_NavminIso = 0;
int CCTData::m_NavmaxIso = 0;
//CDialog* pk;

CCTData::CCTData()
{
	voxelValueData = NULL;
	m_pPixel = NULL;
	m_pPixel2 = NULL;
	m_xSize = 0;
	m_ySize = 0;
	m_zSize = 0;
	m_Scanorient = 0;
	m_xPixelSize = 1;
	m_yPixelSize = 1;
	m_zPixelSize = 1;
	m_tmpPixel = NULL;
	m_tmpxSize = 0;
	m_tmpySize = 0;
	m_tmpzSize = 0;
	m_tmpScanorient = 0;
	m_tmpxPixelSize = 1;
	m_tmpyPixelSize = 1;
	m_tmpzPixelSize = 1;
	m_RoiBoxEnabled = false;
	m_Finish = false;
	m_CompressRatio = 1;
}
CCTData::~CCTData()
{
	if (voxelValueData != NULL) {
		for (int i = 0; i < m_xSize; i++) {
			for (int j = 0; j < m_ySize; j++) {
				delete voxelValueData[i][j];
			}
		}
	}
	delete m_pPixel;
	if (curveCutFlag != (_int8*)0xcdcdcdcd)
		delete curveCutFlag;
	if (LOWcurveCutFlag != (_int8*)0xcdcdcdcd)
		delete LOWcurveCutFlag;
}
bool CCTData::ReadCTFile(CString filen)
{
	CFile	file;
	PWORD	pPixel;
	PWORD   pBuf;
	DWORD   d;
	long VoxelValueData;//体素值：即三维的Pixel value

	CString* dateName;
	CString file_path;
	CString file_name;
	CFileException e;
	CString dest_Root = filen;

	int xSize, ySize;
	int zSize = 0;

	CFileFind finder;
	CString file_Root;
	file_Root = filen;
	int k0, k1;//起止层
	k0 = sstart - 1;
	k1 = send1 - 1;
	zSize = k1 - k0 + 1;
	dateName = NULL;
	pPixel = NULL;

	BOOL bWorking = finder.FindFile(file_Root + "\\*.*");//02112135
	bWorking = finder.FindNextFile();
	bWorking = finder.FindNextFile();

	int zfullnum = 0;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		zfullnum++;
	}

	dateName = new CString[zfullnum];
	CDCMImage* pImage;
	pImage = NULL;

	bWorking = finder.FindFile(file_Root + "\\*.*");//02112135
	bWorking = finder.FindNextFile();
	bWorking = finder.FindNextFile();
	bWorking = finder.FindNextFile();

	int getPixel;
	char temp[100];
	int tempnum;
	for (int i = 0; i < zfullnum; i++)
	{
		file_name = "";
		file_path = "";

		file_name = finder.GetFileName();
		file_path = file_Root + "\\" + file_name;

		pImage = new CDCMImage();
		getPixel = pImage->GetPixelData(file_path);

		strcpy(temp, pImage->ImageNumber);
		tempnum = atof(temp);
		tempnum = tempnum - 425;
		//tempnum = zfullnum;

		dateName[tempnum] = file_path;

		bWorking = finder.FindNextFile();

		delete pImage;
	}
	//pk->SetText("读入切片数据...");
	//SetDlgItemText(IDC_STATIC_STR, "读入切片数据...");
	
	int k;
	for (k = 0; k < zSize; k++)
	{
		/*if (k == zSize / 10.)
		{
			pk->SetText("切片数据读入20%");
			pk->m_Fileprogress.SetPos((int)(200 * 0.20));
		}
		if (k == zSize / 5.)
		{
			pk->SetText("切片数据读入40%");
			pk->m_Fileprogress.SetPos((int)(200 * 0.40));
		}
		if (k == zSize / 2.)
		{
			pk->SetText("切片数据读入60%");
			pk->m_Fileprogress.SetPos((int)(200 * 0.60));
		}
		if (k == zSize * 4. / 5.)
		{
			pk->SetText("切片数据读入80%");
			pk->m_Fileprogress.SetPos((int)(200 * 0.80));
		}*/

		file_path = dateName[k + k0];

		if (file_path.IsEmpty())
			AfxMessageBox("error");

		pImage = new CDCMImage();

		int getPixel = pImage->GetPixelData(file_path);  //读取每层CT信息 

		if (getPixel)
		{
			if (k == 0)
			{
				xSize = pImage->ImageWidth;
				ySize = pImage->ImageHeight;

				/*int syz_zSize = xSize, syz_zfullnum = ySize;
				CString strsyz1, strsyz2;
				strsyz1.Format(_T("%d"), syz_zSize);
				strsyz2.Format(_T("%d"), syz_zfullnum);
				MessageBox(NULL, strsyz1 + " " + strsyz2, "提示", MB_OK);*/

				m_xPixelSize = 1;
				m_yPixelSize = 1;
				m_Scanorient = 0;
				m_zPixelSize = atof(pImage->SliceThickness);
				strcpy(m_bodyname, pImage->BodyPart);
				strcpy(m_paname, pImage->PatientName);
				strcpy(m_pasex, pImage->PatientSex);
				strcpy(m_paage, pImage->PatientAge);

				/*CTWCCenter[0] = pImage->WindowCenter;
				CTWCWidth[0] = pImage->WindowWidth;
				CTWCCenter[1] = pImage->WindowCenter;
				CTWCWidth[1] = pImage->WindowWidth;
				CTWCCenter[2] = pImage->WindowCenter;
				CTWCWidth[2] = pImage->WindowWidth;*/
				m_pPixel = new WORD[xSize * ySize * zSize];
				//初始化voxelValueData
				voxelValueData = new long**[xSize];
				for (int i = 0; i < xSize; i++) {
					voxelValueData[i] = new long*[ySize];
					for (int j = 0; j < ySize; j++) {
						voxelValueData[i][j] = new long[zSize];
					}
				}
			}

		//	CDC             dcMemory;
			
			pBuf = m_pPixel + k * xSize * ySize;
			for (int j = 0;j < ySize;j++)
				for (int i = 0;i < xSize;i++)
				{
					d = pImage->PixelValueData[j * xSize + i];
					*(pBuf + j * ySize + i) = (WORD)(short)d; //pBuf存储像素信息
					//将切片信息写入voxelValueData中
					voxelValueData[i][j][k] = d;
				}
			//dcMemory.CreateCompatibleDC(pImage->m_memdc);
		//	pImage->m_memdc.BitBlt(0, 0, xSize, ySize, &dcMemory, 0, 0, SRCCOPY);
			/*pk->m_edit1.Format("%d", xSize);
			pk->m_edit2.Format("%d", ySize);
			pk->m_edit3.Format("%d", zSize);
			pk->m_edit4.Format("%f", m_zPixelSize);
			pk->m_edit5.Format("%s", m_paname);
			pk->m_edit6.Format("%s", m_pasex);
			pk->m_edit7.Format("%s", m_paage);
			pk->m_edit8.Format("%s", m_bodyname);*/
			//	pk->UpdateF();
		}

		m_xSize = xSize;
		m_ySize = ySize;
		m_zSize = zSize;

		/*if (m_bodyname == "ABDOMEN")
		{
			VRTWindow_Width = 1600;
			VRTWindow_Center = 800;
		}
		else if (m_bodyname == "HEAD")
		{
			VRTWindow_Width = 1600;
			VRTWindow_Center = 800;
		}
		else if (m_bodyname == "HEART")
		{
			VRTWindow_Width = 500;
			VRTWindow_Center = 200;
		}
		else
		{
			VRTWindow_Width = 500;
			VRTWindow_Center = 200;
		}*/
		m_PminIso = 600;
		m_PmaxIso = 1100;
		m_NavminIso = 200;
		m_NavmaxIso = 1001;
		delete pImage;
	}

	finder.Close();
	xs = 0;
	ys = 0;
	zs = 0;
	xe = m_xSize;
	ye = m_ySize;
	ze = m_zSize;

	if (curveCutFlag == (_int8*)0xcdcdcdcd || curveCutFlag == NULL)
		curveCutFlag = new _int8[m_zSize * m_ySize * m_xSize];
	memset(curveCutFlag, 1, m_zSize * m_ySize * m_xSize);

	m_CompressRatio = 1;

	delete[]dateName;
	return true;
}
bool CCTData::ReadCTFileLow(PWORD m_HPixel, int m_xsize, int m_ysize, int m_zsize, int m_xpixelsize, int m_ypixelsize, int m_zpixelsize, CString m_paname, CString m_pasex, CString m_paage, CString m_bodyname)
{
	m_tmpScanorient = m_Scanorient;
	m_xSize = m_xsize;
	m_ySize = m_ysize;
	m_zSize = m_zsize;
	m_xPixelSize = m_xpixelsize;
	m_yPixelSize = m_ypixelsize;
	m_zPixelSize = m_zpixelsize;
	scale1_4(m_HPixel);
	if (LOWcurveCutFlag == (_int8*)0xcdcdcdcd || LOWcurveCutFlag == NULL)
		LOWcurveCutFlag = new _int8[m_zSize * m_ySize * m_xSize];
	memset(LOWcurveCutFlag, 1, m_zSize * m_ySize * m_xSize);

	//delete m_tmpPixel;
	m_CompressRatio = 4;
	xs = 0;
	ys = 0;
	zs = 0;
	xe = m_xSize;
	ye = m_ySize;
	ze = m_zSize;

	return TRUE;

}
void CCTData::scale1_4(PWORD m_tmpPixel)
{
	int	i, j, k;
	double w1, w2, w3, w4, w5, w6, w7, w8;
	m_tmpxSize = m_xSize;
	m_tmpySize = m_ySize;
	m_tmpzSize = m_zSize;

	m_xSize = m_xSize / 4;
	m_ySize = m_ySize / 4;
	m_zSize = int(floor(double(m_zSize) / 4));
	m_pPixel = new WORD[m_xSize * m_ySize * m_zSize];
	for (k = 0; k < m_zSize - 1; k++)
		for (j = 0; j < m_ySize - 1; j++)
			for (i = 0; i < m_xSize - 1; i++) {
				w1 = (double)(short)(*(m_tmpPixel + 4 * k * m_tmpxSize * m_tmpySize + 4 * j * m_tmpxSize + 4 * i));
				w2 = (double)(short)(*(m_tmpPixel + 4 * k * m_tmpxSize * m_tmpySize + 4 * j * m_tmpxSize + 4 * i + 1));
				w3 = (double)(short)(*(m_tmpPixel + 4 * k * m_tmpxSize * m_tmpySize + (4 * j + 1) * m_tmpxSize + 4 * i + 1));
				w4 = (double)(short)(*(m_tmpPixel + 4 * k * m_tmpxSize * m_tmpySize + (4 * j + 1) * m_tmpxSize + 4 * i));
				w5 = (double)(short)(*(m_tmpPixel + (4 * k + 1) * m_tmpxSize * m_tmpySize + 4 * j * m_tmpxSize + 4 * i));
				w6 = (double)(short)(*(m_tmpPixel + (4 * k + 1) * m_tmpxSize * m_tmpySize + 4 * j * m_tmpxSize + 4 * i + 1));
				w7 = (double)(short)(*(m_tmpPixel + (4 * k + 1) * m_tmpxSize * m_tmpySize + (4 * j + 1) * m_tmpxSize + 4 * i + 1));
				w8 = (double)(short)(*(m_tmpPixel + (4 * k + 1) * m_tmpxSize * m_tmpySize + (4 * j + 1) * m_tmpxSize + 4 * i));
				*(m_pPixel + k * m_tmpxSize / 4 * m_tmpySize / 4 + j * m_tmpxSize / 4 + i) = WORD((w1 + w2 + w3 + w4 + w5 + w6 + w7 + w8) / 8);
			}
	m_Scanorient = m_tmpScanorient;
}