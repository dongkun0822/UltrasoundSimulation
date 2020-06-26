
// UltrasoundSimulationView.cpp: CUltrasoundSimulationView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "UltrasoundSimulation.h"
#endif

#include "UltrasoundSimulationDoc.h"
#include "UltrasoundSimulationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//CDialog* pk;
// CUltrasoundSimulationView

IMPLEMENT_DYNCREATE(CUltrasoundSimulationView, CView)

BEGIN_MESSAGE_MAP(CUltrasoundSimulationView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CUltrasoundSimulationView 构造/析构

CUltrasoundSimulationView::CUltrasoundSimulationView() noexcept
{
	// TODO: 在此处添加构造代码
	m_CTDataHEI = new CCTData();
	m_CTDataLOW = new CCTData();
	m_CTData = new CCTData();

}

CUltrasoundSimulationView::~CUltrasoundSimulationView()
{
}

BOOL CUltrasoundSimulationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CUltrasoundSimulationView 绘图

void CUltrasoundSimulationView::OnDraw(CDC* /*pDC*/)
{
	CUltrasoundSimulationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CUltrasoundSimulationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUltrasoundSimulationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CUltrasoundSimulationView 诊断

#ifdef _DEBUG
void CUltrasoundSimulationView::AssertValid() const
{
	CView::AssertValid();
}

void CUltrasoundSimulationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
void CUltrasoundSimulationView::OpenCTFile()
{
	AfxBeginThread(ReadCTFile, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}
CUltrasoundSimulationDoc* CUltrasoundSimulationView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUltrasoundSimulationDoc)));
	return (CUltrasoundSimulationDoc*)m_pDocument;
}
void CUltrasoundSimulationView::OpenFile(CString fileindex)
{
	BOOL suc;
	if (suc = m_CTDataHEI->ReadCTFile(fileindex)) {
		if (m_CTDataLOW->ReadCTFileLow(m_CTDataHEI->m_pPixel, m_CTDataHEI->m_xSize, m_CTDataHEI->m_ySize, m_CTDataHEI->m_zSize, m_CTDataHEI->m_xPixelSize, m_CTDataHEI->m_yPixelSize, m_CTDataHEI->m_zPixelSize, m_CTDataHEI->m_paname, m_CTDataHEI->m_pasex, m_CTDataHEI->m_paage, m_CTDataHEI->m_bodyname))
		{
			//////////////////////////////////////////
				//////////////////////////////////显示参数
			/*pk->m_edit1.Format("%d", m_CTDataHEI->m_xSize);
			pk->m_edit2.Format("%d", m_CTDataHEI->m_ySize);
			pk->m_edit3.Format("%d", m_CTDataHEI->m_zSize);
			pk->m_edit4.Format("%f", m_CTDataHEI->m_zPixelSize);
			pk->m_edit5.Format("%s", m_CTDataHEI->m_paname);
			pk->m_edit6.Format("%s", m_CTDataHEI->m_pasex);
			pk->m_edit7.Format("%s", m_CTDataHEI->m_paage);
			pk->m_edit8.Format("%s", m_CTDataHEI->m_bodyname);*/
		//	pk->SendMessage(WM_UPDATESTATUSBAR);
			//	pk->UpdateF();
		//	pk->m_Fileprogress.SetStep(1);
			///////////////////////////////////////

			m_CTData = m_CTDataHEI;
			/*m_pCTSlice = new CCTSlice();
			m_pZoomSlice = new CZoomSlice();   */ //  by gss image zooming

			/*m_pCTSlice->m_pData = m_CTData->m_pPixel;
			m_pCTSlice->m_CTSliceData = m_CTData;
			m_pCTSlice->InitializeMatrix();*/
			//m_vol = new VOLUME;
			//m_volBIG = new VOLUME;

			//m_CurrentCT = fileindex;

			//m_volBIG->length[0] = m_CTDataHEI->m_xSize;
			//m_volBIG->length[1] = m_CTDataHEI->m_ySize;
			//m_volBIG->length[2] = m_CTDataHEI->m_zSize;
			//m_volBIG->pData = m_CTDataHEI->m_pPixel;

			//m_vol->length[0] = m_CTDataLOW->m_xSize;
			//m_vol->length[1] = m_CTDataLOW->m_ySize;
			//m_vol->length[2] = m_CTDataLOW->m_zSize;
			//m_vol->pData = m_CTDataLOW->m_pPixel;
			//InitVRT_Scale = 0.8; //double(m_ViewWnd[0]->r1.Height())/double(m_volBIG->length[0]);
			//VRT_Scale = 0.8; //double(m_ViewWnd[0]->r1.Height())/double(m_volBIG->length[0]);
			//m_newvrtBIG.SelectVolume(m_volBIG);//m_newvrtBIG是VrtVolumeRenderingSystem类的对象
			//pk->SetText("开始进行三维重建");
			//m_newvrtBIG.EncodeVolume();
			//m_newvrt.SelectVolume(m_vol);//m_newvrt是VrtVolumeRenderingSystem类的对象
			//m_newvrt.EncodeVolume();//reconstruct
			//m_newvrt.InitializeMatrix();
			//m_newvrtBIG.InitializeMatrix();
			//m_newvrt.m_LOW = TRUE;
			//m_newvrtBIG.m_LOW = FALSE;

			//m_File_Finish = TRUE;

			m_Render.InitViewer(m_CTData);
			m_WndWidth = r.Width() / 2;//compute the window's weight and height
			m_WndHeight = r.Height() / 2;
			//m_Render.ChangeRect(m_WndWidth, m_WndHeight);
			m_MenuReconstruct = true;
			pre_z[0] = int(m_CTData->m_zSize * m_CTData->m_zPixelSize / 2);
			pre_z[1] = (int)(-m_CTData->m_xSize / 2);
			pre_z[2] = (int)(m_CTData->m_ySize / 2);
			pre_z[3] = 0;
			/////////////////////////////////////

		}
	}

	else
	{
		m_Data_Error = TRUE;
	}
}
#endif //_DEBUG


// CUltrasoundSimulationView 消息处理程序
