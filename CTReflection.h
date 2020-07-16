#pragma once
#include <vector>

class CTReflection{
private:
	const double soundVelocity = 1.54;
	long* CTValueData;//存储CT值
	double* AcousticImpedanceValueData;//存储声阻抗系数

	
public:
	CTReflection();
	virtual ~CTReflection();
	bool pixelToCT(long* PixelValueData, int xSize, int ySize, int slope, int intercept);
	//函数接口，得到声阻抗矩阵
	bool getAcousticImpedance(CDCMImage* pImage);
	bool getAcousticImpedance(int* arrays, CDCMImage* pImage);//通过位置分布矩阵计算声阻抗
	//通过公式得声阻抗系数
	double computeAcousticImpedance(int dataCT);
	//获得法向量矩阵
	bool getNormalMatrix()

	void Reflection();
	double reflectivity(int x, int y, int z);
};

