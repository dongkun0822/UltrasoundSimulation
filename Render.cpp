#include "pch.h"
#include "Render.h"


void CRender::InitViewer(CCTData* p_ct)
{
	if (p_ct->m_xSize == 0)
	{
		upx = Vupx = 0.0;
		upy = Vupy = -1.0;
		upz = Vupz = 0.0;
		xRotate = yRotate = zRotate = 0;
		xSize = 512;
		ySize = 512;
		zSize = 256;	//???不知为什么zSize==512时，坐标轴不显示
		ratio = 1;

		OrthUp = OrthFoot = OrthLeft = true;

		ex = xSize / 2;
		ey = ySize / 2;
		//		ez = -zSize*ratio + 0.1*(xSize+ySize);
		if (zSize * ratio > xSize)
			ez = -zSize * ratio / 2.0;
		else
			ez = -xSize / 2.0;

		xCenter = rx = xSize / 2;
		yCenter = ry = ySize / 2;
		zCenter = rz = 0;
		viewNo = PROJ_PERSPECT;
		fovy = 100.0;
		nearp = 0.1;
		farp = 5000.0;
		scale = 1;
		w_to_v = ((double)xSize) / 340.0;
		return;
	}
	upx = Vupx = 0.0;
	upy = Vupy = -1.0;
	upz = Vupz = 0.0;
	xRotate = yRotate = zRotate = 0;
	xSize = p_ct->m_xSize;
	ySize = p_ct->m_ySize;
	zSize = p_ct->m_zSize;
	ratio = p_ct->m_zPixelSize / p_ct->m_xPixelSize;

	OrthUp = OrthFoot = OrthLeft = true;

	ex = xSize / 2;
	ey = ySize / 2;
	//	ez = -zSize*ratio + 0.1*(xSize+ySize);
	if (zSize * ratio > xSize)
		ez = -zSize * ratio / 2.0;
	else
		ez = -xSize / 2.0;
	xCenter = rx = xSize / 2;
	yCenter = ry = ySize / 2;
	zCenter = rz = zSize * ratio / 2;
	fovy = 100.0;
	nearp = 0.1;
	farp = 5000.0;
	scale = 1;
	//lgt_pos[0][2] = float(rz) + 100;
}
//void CRender::ChangeRect(int width, int height)
//{
//	m_roiBox.width = width;
//	m_roiBox.height = height;
//	m_roiBox.Init();
//}