// MapEditDoc.cpp : implementation of the CMapEditDoc class
//

#include "stdafx.h"
#include "MapEdit.h"

#include "MapEditDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc

IMPLEMENT_DYNCREATE(CMapEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapEditDoc, CDocument)
	//{{AFX_MSG_MAP(CMapEditDoc)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURSORXY, OnStatusCursorXY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc construction/destruction

CMapEditDoc::CMapEditDoc() 
{
}

CMapEditDoc::~CMapEditDoc()
{
}

BOOL CMapEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;
	TRACE0("New Doc\n");
	Core.NewMap();

	return TRUE;
}

BOOL CMapEditDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	TRACE1("Load Doc %s\n",lpszPathName);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc serialization
void CMapEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
   
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc diagnostics

#ifdef _DEBUG
void CMapEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CMapEditDoc::UpdateView(CMapEditView *View)
{
		Core.UpdateView(View);
}

/*********************************************************************************/
void	CMapEditDoc::Render(CMapEditView *View)	
{
		Core.Render(View);
}

/*********************************************************************************/
void	CMapEditDoc::UpdateAll(CMapEditView *View)
{
		Core.UpdateAll(View);
}

/*********************************************************************************/
void	CMapEditDoc::OnStatusCursorXY(CCmdUI *pCmdUI)
{

CPoint	&XY=Core.GetCursorPos();
CString XYStr;
		pCmdUI->Enable();
		if (XY.x!=-1 && XY.y!=-1)
			XYStr.Format( "%d\t%d", XY.x,XY.y);
		pCmdUI->SetText(XYStr); 
}

/*********************************************************************************/
/*********************************************************************************/
/*** Windows Message Handlers ****************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CMapEditDoc::LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
	Core.LButtonControl(View,nFlags,point,DownFlag);
}

/*********************************************************************************/
void	CMapEditDoc::MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
	Core.MButtonControl(View,nFlags,point,DownFlag);
}

/*********************************************************************************/
void	CMapEditDoc::RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
	Core.RButtonControl(View,nFlags,point,DownFlag);
}

/*********************************************************************************/
void	CMapEditDoc::MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &point)
{
	Core.MouseWheel(View,nFlags,zDelta,point);
}

/*********************************************************************************/
void	CMapEditDoc::MouseMove(CMapEditView *View,UINT nFlags, CPoint &point)
{
	Core.MouseMove(View,nFlags,point);
}

/*********************************************************************************/
void	CMapEditDoc::ToggleTileView(CMapEditView *View)
{
		Core.UpdateTileView(View,TRUE);
		Core.UpdateAll(View);
}

/*********************************************************************************/
void	CMapEditDoc::ToggleGrid(CMapEditView *View)
{
		Core.UpdateGrid(View,TRUE);
}

/*********************************************************************************/
void	CMapEditDoc::ReloadTileBank()
{
		Core.ReloadTileBank();
		UpdateAllViews(NULL);

}

/*********************************************************************************/
void	CMapEditDoc::TileBankChange()
{
		Core.ChangeTileBank();
		UpdateAllViews(NULL);
}
