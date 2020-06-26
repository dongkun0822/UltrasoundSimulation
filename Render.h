#pragma once
#include "CCTData.h"


#define PROJ_PERSPECT	3

class CRender
{
public:
	void InitViewer(CCTData* p_ct);
	//void ChangeRect(int width, int height);

public:
	int		Bottom, Left, Width, Height, xSize, ySize, zSize, viewNo;
	double  Vupx, Vupy, Vupz;
	double	ex, ey, ez, rx, ry, rz, upx, upy, upz, fovy, nearp, farp, xCenter, yCenter, zCenter, w_to_v;
	double	ex_bak, ey_bak, ez_bak, rx_bak, ry_bak, rz_bak, upx_bak, upy_bak, upz_bak;
	double	xRotate, yRotate, zRotate, scale, xTrans, yTrans, zTrans;
	bool OrthUp, OrthFoot, OrthLeft;
	double	ratio;
};

