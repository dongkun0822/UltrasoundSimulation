
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

CUltrasoundSimulationDoc* CUltrasoundSimulationView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUltrasoundSimulationDoc)));
	return (CUltrasoundSimulationDoc*)m_pDocument;
}
#endif //_DEBUG


// CUltrasoundSimulationView 消息处理程序
