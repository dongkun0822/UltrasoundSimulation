#include "pch.h"
#include "CTReflection.h"
#include "DCMImage.h"
#include "MainFrm.h"
CTReflection::CTReflection() {

	CTValueData = NULL;
	AcousticImpedanceValueData = NULL;
}
CTReflection::~CTReflection() {
	if (CTValueData != NULL)
	{
		delete[]CTValueData;
		CTValueData = NULL;
	}
	if (AcousticImpedanceValueData != NULL)
	{
		delete[]AcousticImpedanceValueData;
		AcousticImpedanceValueData = NULL;
	}
}
//将像素值改为CT值
bool CTReflection::pixelToCT(long* PixelValueData, int xSize, int ySize, int slope, int intercept) {
	/*int xSize = pImage->ImageWidth;
	int ySize = pImage->ImageHeight;*/
	if (xSize <= 0 || ySize <= 0) return false;
	/*int slope = pImage->m_Slope;
	int intercept = pImage->m_Intercept;*/
	long tempPixelValue = 0;
	CTValueData = new long[xSize*ySize];
	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++)
		{
			tempPixelValue = PixelValueData[j * xSize + i];
			CTValueData[j * xSize + i] = tempPixelValue * slope + intercept;//pixel * slope + intercept
		}
	}
	return true;
}
bool CTReflection::getAcousticImpedance(CDCMImage* pImage) {
	int xSize = pImage->ImageWidth;
	int ySize = pImage->ImageHeight;
	if (xSize <= 0 || ySize <= 0) return false;
	int slope = pImage->m_Slope;
	int intercept = pImage->m_Intercept;
	long tempCTvalue = 0;
	pixelToCT(pImage->PixelValueData, xSize, ySize, slope, intercept);
	AcousticImpedanceValueData = new double[xSize*ySize];
	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++)
		{
			tempCTvalue = CTValueData[j * xSize + i];
			AcousticImpedanceValueData[j * xSize + i] = computeAcousticImpedance(tempCTvalue);
		}
	}
	return true;
}
bool CTReflection::getAcousticImpedance(int* arrays, CDCMImage* pImage) {////通过位置分布矩阵计算声阻抗
	int xSize = pImage->ImageWidth;
	int ySize = pImage->ImageHeight;
	if (xSize <= 0 || ySize <= 0) return false;
	int slope = pImage->m_Slope;
	int intercept = pImage->m_Intercept;
	long tempCTvalue = 0;
	//通过位置分布矩阵，找到像素矩阵
	long *tempPixelValueData = new long[xSize*ySize];
	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++)
		{
			tempPixelValueData[j * xSize + i] = pImage->PixelValueData[arrays[j * xSize + i]];
		}
	}
	pixelToCT(tempPixelValueData, xSize, ySize, slope, intercept);
	AcousticImpedanceValueData = new double[xSize*ySize];
	for (int j = 0; j < ySize; j++) {
		for (int i = 0; i < xSize; i++)
		{
			tempCTvalue = CTValueData[j * xSize + i];
			AcousticImpedanceValueData[j * xSize + i] = computeAcousticImpedance(tempCTvalue);
		}
	}
	delete[] tempPixelValueData;
	return true;
}
//通过公式得声阻抗系数
double CTReflection::computeAcousticImpedance(int dataCT) {//判断所在区间
	double res = 0;
	if (dataCT >= -1000 && dataCT < -950) {
		res = (0.00086*(dataCT + 1000) + 0.001)*soundVelocity;
	}
	else if (dataCT >= -950 && dataCT < -700) {
		res = (0.001032*(dataCT + 950) + 0.044)*soundVelocity;
	}
	else if (dataCT >= -700 && dataCT < -98) {
		res = (0.00104*(dataCT + 700) + 0.302)*soundVelocity;
	}
	else if (dataCT >= -98 && dataCT < 100) {
		res = (0.00096*(dataCT + 98) + 0.93)*soundVelocity;
	}
	else if (dataCT >= 100 && dataCT <= 1000) {
		res = (0.00089*(dataCT - 100) + 1.12)*soundVelocity;
	}


	return res;
}


void CTReflection::Reflection()
{

}
double CTReflection::reflectivity(int x, int y, int z)
{
	return;
}


