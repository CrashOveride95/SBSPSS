/***********************/
/*** Map Editor Core ***/
/***********************/


#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

#include	"NewMapGUI.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerTile.h"
#include	"utils.h"

#include	"ExportAGB.h"
#include	"ExportPSX.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
		CurrentMousePos=CPoint(0,0);
}

/*****************************************************************************/
CCore::~CCore()
{
int	ListSize=Layer.size();
		for (int i=0; i<ListSize; i++) delete Layer[i];
}

/*****************************************************************************/
void	CCore::Init()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->RemoveAll();
// Add default param bar items			
			ParamBar->Add(Frm->GetLayerList(),IDD_LAYER_LIST_DIALOG,TRUE,TRUE);
			UpdateParamBar();
			UpdateAll(NULL);
}

/*****************************************************************************/
BOOL	CCore::New()
{
CNewMapGUI	Dlg;
int		Width,Height;
		Dlg.m_Width=TileLayerMinWidth;
		Dlg.m_Height=TileLayerMinHeight;

		Dlg.m_Back=TRUE;
		Dlg.m_Mid=TRUE;
		Dlg.m_Fore=FALSE;
		
		if (Dlg.DoModal()!=IDOK) return FALSE;
		Width=Dlg.m_Width;
		Height=Dlg.m_Height;

// Create Tile Layers
//															Type					Width					Height		Scale	3d?		Resizable?
		if (Dlg.m_Back)		Layer.push_back(new CLayerTile(	LAYERTILE_BACK,			32,						32,			4.0f,	FALSE,	FALSE));
		if (Dlg.m_Mid)		Layer.push_back(new CLayerTile(	LAYERTILE_MID,			Width,					Height,		2.0f,	FALSE,	TRUE));
							Layer.push_back(new CLayerTile(	LAYERTILE_ACTION,		Width,					Height,		1.0f,	TRUE,	TRUE));
		if (Dlg.m_Fore)		Layer.push_back(new CLayerTile(	LAYERTILE_FORE,			Width,					Height,		0.5f,	FALSE,	TRUE));

		ActiveLayer=FindActionLayer();
		MapCam.Zero();
		TileCam.Zero();
		TileViewFlag=FALSE;
		GridFlag=TRUE;
		Is3dFlag=TRUE;
		Init();
		return(TRUE);
}

/*****************************************************************************/

void	CCore::Load(CFile *File)
{
float	Version;

		File->Read(&Version,sizeof(float));
		TRACE1("Load Version %g\n",Version);

		if (Version>=1.0)
		{
			File->Read(&MapCam,sizeof(Vector3));
			File->Read(&MapCamOfs,sizeof(Vector3));
			File->Read(&TileCam,sizeof(Vector3));
			File->Read(&TileCamOfs,sizeof(Vector3));
	
			File->Read(&TileViewFlag,sizeof(BOOL));
			File->Read(&GridFlag,sizeof(BOOL));
			File->Read(&Is3dFlag,sizeof(BOOL));
		}
		if (Version>=1.1)
		{

		}


// Layers
int		LayerCount;
		File->Read(&LayerCount,sizeof(int));
		File->Read(&ActiveLayer,sizeof(int));

		for (int i=0;i<LayerCount;i++)
		{
			int	Type;

			File->Read(&Type,sizeof(int));
			switch (Type)
			{
			case LAYER_TYPE_TILE:
				Layer.push_back(new CLayerTile(File,Version));
				break;

			}
		}
		TileBank.Load(File,Version);


// Check Layers
int		MapWidth=Layer[FindActionLayer()]->GetWidth();
int		MapHeight=Layer[FindActionLayer()]->GetHeight();
		for (i=0;i<LayerCount;i++)
		{
			Layer[i]->CheckLayerSize(MapWidth,MapHeight);
		}

		Init();
//		MapCam.Zero();
}

/*****************************************************************************/
void	CCore::Save(CFile *File)
{
// Version 1
		File->Write(&FileVersion,sizeof(float));

		File->Write(&MapCam,sizeof(Vector3));
		File->Write(&MapCamOfs,sizeof(Vector3));
		File->Write(&TileCam,sizeof(Vector3));
		File->Write(&TileCamOfs,sizeof(Vector3));

		File->Write(&TileViewFlag,sizeof(BOOL));
		File->Write(&GridFlag,sizeof(BOOL));
		File->Write(&Is3dFlag,sizeof(BOOL));


// Layers
int		LayerCount=Layer.size();
		File->Write(&LayerCount,sizeof(int));
		File->Write(&ActiveLayer,sizeof(int));

		for (int i=0;i<LayerCount;i++)
		{
			int	Type=Layer[i]->GetType();
			File->Write(&Type,sizeof(int));
			Layer[i]->Save(File);
		}
	TileBank.Save(File);

}

/*****************************************************************************/
BOOL	CCore::Question(char *Txt)
{
	return(theApp.GetCurrent()->Question(Txt));
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Render(CMapEditView *View,BOOL ForceRender)
{
		if (TileBank.NeedLoad()) TileBank.LoadTileSets(this);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen
		if (TileViewFlag)
		{
			RenderTileView(View);
		}
		else
		{
			RenderLayers(View);
		}

}

/*****************************************************************************/
void	CCore::RenderLayers(CMapEditView *View)
{
Vector3	&ThisCam=GetCam();
int		ListSize=Layer.size();
	
		for (int i=0;i<ListSize;i++)
		{
			if (Layer[i]->IsVisible())
				{
				Layer[i]->Render(this,ThisCam,Is3dFlag);
				if (GridFlag) Layer[i]->RenderGrid(this,ThisCam,i==ActiveLayer);
				}
		}
		
		Layer[ActiveLayer]->RenderCursor(this,ThisCam,Is3dFlag);
		
// Get Cursor Pos
		LastCursorPos=CursorPos;
		Layer[ActiveLayer]->FindCursorPos(this,View,GetCam(),CurrentMousePos);
}

/////////////////////////////////////////////////////////////////////////////
void	CCore::RenderTileView(CMapEditView *View)
{
Vector3	&ThisCam=GetCam();

		TileBank.RenderSet(this,ThisCam,Is3dFlag);


// Get Cursor Pos
		TileBank.FindCursorPos(this,View,GetCam(),CurrentMousePos);
}

/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::SetMode(int NewMode)
{
BOOL	RedrawFlag=FALSE;
		
		RedrawFlag=Layer[ActiveLayer]->SetMode(NewMode);
		
}

/*****************************************************************************/
void	CCore::LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
BOOL	RedrawFlag=FALSE;

		if (TileViewFlag)
		{
			if (nFlags & MK_RBUTTON)
				RedrawFlag=TileBank.SelectCancel();
			else
				RedrawFlag=TileBank.SelectL(DownFlag);
		}
		else
		{
			if (Layer[ActiveLayer]->IsVisible())
			{
				RedrawFlag=Layer[ActiveLayer]->LButtonControl(this,View,nFlags,CursorPos,DownFlag);
			}
		}
		TileBank.SetActiveBrushL();

		if (RedrawFlag) View->Invalidate();
}

/*****************************************************************************/
void	CCore::MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
}

/*****************************************************************************/
void	CCore::RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
BOOL	RedrawFlag=FALSE;

		if (TileViewFlag)
		{
			if (nFlags & MK_LBUTTON)
				RedrawFlag=TileBank.SelectCancel();
			else
				RedrawFlag=TileBank.SelectR(DownFlag);
		}
		else
		{
			if (Layer[ActiveLayer]->IsVisible())
			{
				RedrawFlag=Layer[ActiveLayer]->RButtonControl(this,View,nFlags,CursorPos,DownFlag);
			}
		}
		TileBank.SetActiveBrushR();

		if (RedrawFlag) View->Invalidate();
}

/*****************************************************************************/
void	CCore::Zoom(CMapEditView *View,float Dst) 
{
Vector3	Ofs;
		Ofs.Zero();
		Ofs.z=Dst;
		UpdateView(View,Ofs);
}


/*****************************************************************************/
void	CCore::MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &pt) 
{
		if (zDelta>0) 
			Zoom(View,+0.1f);
		else
			Zoom(View,-0.1f);
}

/*****************************************************************************/
void	CCore::MouseMove(CMapEditView *View,UINT nFlags, CPoint &point) 
{
Vector3	Ofs;
Vector3	&ThisCam=GetCam();
// check if active doc
		Ofs.Zero();
		if (theApp.GetCurrent()!=View->GetDocument()) return;

		
		CurrentMousePos=point;

// Handle Drag Movement
		if (nFlags & MK_MBUTTON || nFlags & MK_SHIFT)
			{
			float	XS,YS;
			RECT	ThisRect;

			View->GetWindowRect(&ThisRect);
			XS=ThisCam.z*16;
			YS=ThisCam.z*16;
			XS/=((ThisRect.right-ThisRect.left));
			YS/=((ThisRect.bottom-ThisRect.top));
	
			Ofs.x=LastMousePos.x-CurrentMousePos.x;
			Ofs.y=LastMousePos.y-CurrentMousePos.y;
	
			Ofs.x*=XS;
			Ofs.y*=YS;
			UpdateView(View,Ofs);
			}
		else
		{	
			if (TileViewFlag)
			{
			}
			else
			{
				if (Layer[ActiveLayer]->IsVisible())
				{
					Layer[ActiveLayer]->MouseMove(this,View,nFlags,CursorPos);
				}
			}
			View->Invalidate();	// Mouse still moved, so need to redraw windows, to get CursorPos (And pos render)
		}
		LastMousePos=CurrentMousePos;
}


/*****************************************************************************/
/*** Layers ******************************************************************/
/*****************************************************************************/
void	CCore::UpdateParamBar()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();
CMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->RemoveAll();
			Layer[ActiveLayer]->InitGUI(this);
			Layer[ActiveLayer]->UpdateGUI(this);

			ParamBar->Update();

}

/*****************************************************************************/
void		CCore::UpdateLayerGUI(CMapEditView *View)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();
CLayerList	*List=(CLayerList*)Frm->GetDialog(IDD_LAYER_LIST_DIALOG);
int			ListSize=Layer.size();

			List->ListBox.ResetContent();

			for (int i=0; i<ListSize; i++)
			{
				List->ListBox.AddString(Layer[i]->GetName());
			}
// Now sets checks (silly MSoft bug!!)
			for (i=0; i<ListSize; i++)
			{
				List->ListBox.SetCheck(i,Layer[i]->IsVisible());
			}
			List->ListBox.SetCurSel(ActiveLayer);
}

/*****************************************************************************/
void		CCore::SetLayer(int NewLayer)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();
CLayerList	*List=(CLayerList*)Frm->GetDialog(IDD_LAYER_LIST_DIALOG);

// If toggling layer, dont change the layer
		if ((int)List->ListBox.GetCheck(NewLayer)!=(int)Layer[NewLayer]->IsVisible())
		{
			Layer[NewLayer]->SetVisible(List->ListBox.GetCheck(NewLayer));
			List->ListBox.SetCurSel(ActiveLayer);
		}
		else
		{
			ActiveLayer=NewLayer;
		}

}

/*****************************************************************************/
/*** Grid ********************************************************************/
/*****************************************************************************/
void	CCore::UpdateGrid(CMapEditView *View,BOOL Toggle)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();

			if (Toggle) GridFlag=!GridFlag;

			ToolBar->GetToolBarCtrl().PressButton(ID_TOOLBAR_GRID,GridFlag);
			UpdateView(View);
}

/*****************************************************************************/
/*** TileBank ****************************************************************/
/*****************************************************************************/
void	CCore::UpdateTileView(CMapEditView *View,BOOL Toggle)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();
CMultiBar	*ParamBar=Frm->GetParamBar();

			if (Toggle) TileViewFlag=!TileViewFlag;
			ParamBar->RemoveAll();
			ToolBar->GetToolBarCtrl().PressButton(ID_TOOLBAR_TILEPALETTE,TileViewFlag);
			UpdateParamBar();
			UpdateView(View);
}

/*****************************************************************************/
void	CCore::TileBankLoad(char *Filename)
{
		TileBank.AddTileSet(Filename);
		TileBank.UpdateGUI(this,TileViewFlag);
		UpdateView(NULL);
}

/*****************************************************************************/
void	CCore::TileBankDelete()
{
		if (Question("Delete Current Tile Bank\n\nAll used tiles in current set will be set to blank\nAre you sure?"))
			{
			int		SetCount=TileBank.GetSetCount();
			int		Current=TileBank.GetCurrent();
			int		i,ListSize=Layer.size();

				for (i=0;i<ListSize;i++)
				{
					Layer[i]->DeleteSet(Current);
				}
				TileBank.Delete();

				for (int Set=Current+1; Set<SetCount; Set++)
				{
		
					for (i=0;i<ListSize;i++)
					{
						Layer[i]->RemapSet(Set,Set-1);
					}
				}
				UpdateView(NULL);
			}
}

/*****************************************************************************/
void	CCore::TileBankReload()
{
		TileBank.Reload();
		TexCache.Purge();
		UpdateView(NULL);
}

/*****************************************************************************/
void	CCore::TileBankSet()
{
CMainFrame		*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CLayerTileGUI	*Dlg=(CLayerTileGUI*)Frm->GetDialog(IDD_LAYERTILE_GUI);

		TileBank.SetCurrent(Dlg->m_List.GetCurSel());
}

/*****************************************************************************/
void	CCore::MirrorX(CMapEditView *View)
{
		if (!TileViewFlag) 
		{
			Layer[ActiveLayer]->MirrorX(this);
			UpdateView(View);
		}
}

/*****************************************************************************/
void	CCore::MirrorY(CMapEditView *View)
{
		if (!TileViewFlag) 
		{
			Layer[ActiveLayer]->MirrorY(this);
			UpdateView(View);
		}
}

/*****************************************************************************/
void	CCore::ActiveBrushLeft(CMapEditView *View)
{
		TileBank.SetActiveBrushL();
		UpdateView(View);
}

/*****************************************************************************/
void	CCore::ActiveBrushRight(CMapEditView *View)
{
		TileBank.SetActiveBrushR();
		UpdateView(View);
}

/*****************************************************************************/
BOOL	CCore::IsTileValid(int Set,int Tile)
{
		return(TileBank.IsTileValid(Set,Tile));
}

/*****************************************************************************/
void	CCore::CopySelection(CMapEditView *View)
{
		Layer[ActiveLayer]->CopySelection(this);
		UpdateView(View);
}

/*****************************************************************************/
void	CCore::PasteSelection(CMapEditView *View)
{
		Layer[ActiveLayer]->PasteSelection(this);
		UpdateView(View);
}

/*****************************************************************************/
/*** Misc ********************************************************************/
/*****************************************************************************/
Vector3	&CCore::GetCam()
{
		if (TileViewFlag)
			return(TileCam);
		else
			return(MapCam);

}

/*****************************************************************************/
void	CCore::SetScale()
{
float	XS=GetZoomW();
float	YS=GetZoomH();
float	ZS=XS/YS;

		ScaleVector.x=1.0f/XS;
		ScaleVector.y=1.0f/XS;
		ScaleVector.z=1.0f/XS;//4.0f;
}

/*****************************************************************************/
float	CCore::GetZoomW()
{
Vector3	&ThisCam=GetCam();

	return((float)SCREEN_MAP_WIDTH/ThisCam.z);
}

/*****************************************************************************/
float	CCore::GetZoomH()
{
Vector3	&ThisCam=GetCam();

	return((float)SCREEN_MAP_HEIGHT/ThisCam.z);
}

/*****************************************************************************/
void	CCore::UpdateGUI(CMapEditView *View)
{
		UpdateLayerGUI(View);
		UpdateGrid(View);

		Layer[ActiveLayer]->UpdateGUI(this);

}

/*****************************************************************************/
void	CCore::UpdateAll(CMapEditView *View)
{
		UpdateGUI(View);
		UpdateView(View);
}

/*****************************************************************************/
void	CCore::UpdateView(CMapEditView *View)
{
		SetScale();
		if (View) View->Invalidate();
}		

/*****************************************************************************/
void	CCore::UpdateView(CMapEditView *View,Vector3 &Ofs)
{
Vector3	&ThisCam=GetCam();

		ThisCam.x+=Ofs.x;
		ThisCam.y+=Ofs.y;
		ThisCam.z-=Ofs.z;
		if (ThisCam.x<0) ThisCam.x=0;
		if (ThisCam.y<0) ThisCam.y=0;
		if (ThisCam.z<0.1) ThisCam.z=0.1f;
		TRACE1("ZoomVal %f\n",ThisCam.z);

		UpdateView(View);
}		

/*****************************************************************************/
void	CCore::SetMapSize(CMapEditView *View,int Width,int Height)
{
	if (Width==GetMapWidth() && Height==GetMapHeight()) return;

int		ListSize=Layer.size();

	for (int i=0; i<ListSize; i++) 
	{
		Layer[i]->Resize(Width,Height);
	}

	UpdateView(View);
}

/*****************************************************************************/
void	CCore::Toggle2d3d(CMapEditView *View)
{
		Is3dFlag=!Is3dFlag;
		UpdateView(View);
}

/*****************************************************************************/
int		CCore::FindLayer(int Type,int SubType)
{
int	ListSize=Layer.size();

		for (int i=0; i<ListSize; i++)
		{
			if (Layer[i]->GetType()==Type)
				if (SubType==-1 || Layer[i]->GetSubType()==SubType)
					return(i);
		}
	return(-1);
}

/*****************************************************************************/
int		CCore::FindActionLayer()
{
int	Idx=FindLayer(LAYER_TYPE_TILE,LAYERTILE_ACTION);

	return(Idx);
}

/*****************************************************************************/

Vector3	CCore::OffsetCam(Vector3 &Cam,float DivVal)
{
Vector3	ThisCam=Cam;

	ThisCam=Cam/DivVal;
	ThisCam.z=Cam.z;

	return(ThisCam);
}

/*****************************************************************************/
void	CCore::ExportAGB(char *Filename)
{
int		LayerCount=Layer.size();
char	ExportName[256];
		
		SetFileExt(Filename,ExportName,"c");

CExportAGB	Exp(ExportName);

		for (int i=0;i<LayerCount;i++)
		{
			Layer[i]->Export(this,Exp);
		}
		Exp.ExportTiles(this);
		Exp.ExportPalette();
}

/*****************************************************************************/
void	CCore::ExportPSX(char *Filename)
{

int		LayerCount=Layer.size();
char	ExportName[256];
		
		SetFileExt(Filename,ExportName,"PME");

CExportPSX	Exp(ExportName,LayerCount);

		for (int i=0;i<LayerCount;i++)
		{
			Layer[i]->Export(this,Exp);
		}

		Exp.ExportTiles(this);
		Exp.ExportTexList(this);

}

/*****************************************************************************/
GString	CCore::GetCurrentPath()
{
GFName	FullPath=theApp.GetCurrent()->GetPathName();
GString	Path=FullPath.Dir();
	
	return(Path);

}


