/***********************/
/*** Map Editor Core ***/
/***********************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	"GLEnabledView.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerTile.h"

BOOL	Test3dFlag=TRUE;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
	for (int i=0; i<LAYER_TYPE_MAX; i++) Layers[i]=0;
}

/*****************************************************************************/
CCore::~CCore()
{
	for (int i=0; i<LAYER_TYPE_MAX; i++) if (Layers[i]) delete Layers[i];
}

/*****************************************************************************/
void	CCore::NewMap()
{

	RenderFlag=TRUE;

// To be loaded/created
	Layers[LAYER_TYPE_BACK]=	new CLayerTile("Back",	32,							32,								4.0f,	FALSE);
	Layers[LAYER_TYPE_MID]=		new CLayerTile("Mid",	TileLayerDefaultWidth/2.0f,	TileLayerDefaultHeight/2.0f,	2.0f,	FALSE);
	Layers[LAYER_TYPE_ACTION]=	new CLayerTile("Action",TileLayerDefaultWidth/1.0f,	TileLayerDefaultHeight/1.0f,	1.0f,	TRUE);
	Layers[LAYER_TYPE_FORE]=	new CLayerTile("Fore",	TileLayerDefaultWidth/0.5f,	TileLayerDefaultHeight/0.5f,	0.5f,	FALSE);

	TileViewFlag=0;
	ParamViewFlag=1;

	ActiveLayer=LAYER_TYPE_ACTION;
	MapCam=Vec(0,0,0);
	TileCam=Vec(0,0,0);
	TileBank.AddTileSet("c:/temp/3/test.gin");
}

/*****************************************************************************/
void	CCore::OpenMap()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Render(CMapEditView *View)
{

Vec		&ThisCam=GetCam();

		if (TileBank.NeedLoad()) TileBank.LoadTileSets(this);

		if (RenderFlag)
		{
			RenderFlag=FALSE;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen
			if (GetTileView())
			{
	
			}
			else
			{
				for (int i=0;i<LAYER_TYPE_MAX;i++)
				{
					Layers[i]->Render(this,ThisCam,Test3dFlag);
				}
			}
			Layers[ActiveLayer]->RenderGrid(this,ThisCam);
		}
// Calc CursorPos
		Layers[ActiveLayer]->FindCursorPos(this,View,ThisCam,CurrentMousePos);

}

/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
}

/*****************************************************************************/
void	CCore::MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
		LastMousePos=point;
}

/*****************************************************************************/
void	CCore::RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
}

/*****************************************************************************/
void	CCore::MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &pt) 
{
		if (zDelta>0)
			UpdateView(View,Vec(0,0,1.0f));
		else
			UpdateView(View,Vec(0,0,-1.0f));
}

/*****************************************************************************/
void	CCore::MouseMove(CMapEditView *View,UINT nFlags, CPoint &point) 
{
Vec		Ofs(0,0,0);

Vec		&ThisCam=GetCam();
// check if active doc
		if (theApp.GetCurrent()!=View->GetDocument()) return;

		CurrentMousePos=point;

// Handle Drag Movement
		if (nFlags & MK_MBUTTON)
			{
			float	XS,YS;
			RECT	ThisRect;

			View->GetWindowRect(&ThisRect);
			XS=ThisCam.z*4;//*Layers[ActiveLayer]->GetLayerZPos();
			YS=ThisCam.z*4;//*Layers[ActiveLayer]->GetLayerZPos();
			XS/=((ThisRect.right-ThisRect.left));
			YS/=((ThisRect.bottom-ThisRect.top));
	
			Ofs.x=LastMousePos.x-CurrentMousePos.x;
			Ofs.y=LastMousePos.y-CurrentMousePos.y;
			LastMousePos=CurrentMousePos;
	
			Ofs.x*=XS;
			Ofs.y*=YS;

			UpdateView(View,Ofs);
			}
		else
		{	// Mouse still moved, so need to redraw windows, to get CursorPos (And pos render)
			View->Invalidate();
		}

}

/*****************************************************************************/
/*** Layers ******************************************************************/
/*****************************************************************************/
void	CCore::UpdateParamBar(CMapEditView *View,BOOL ViewFlag)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();
CParamBar	*ParamBar=Frm->GetParamBar();
CCheckListBox	*Dlg=(CCheckListBox *)ParamBar->GetDlgItem(IDC_PARAMBAR_LAYER_LIST);
		
		Dlg->SetCheck(1,1);
		ParamViewFlag=ViewFlag;
		if (ParamViewFlag)
		{
			Dlg->ResetContent();

			for (int i=0;i<LAYER_TYPE_MAX;i++)
			{
				CLayer	*ThisLayer=GetLayer(i);
				Dlg->AddString(ThisLayer->GetName());
			}
			Dlg->SetCurSel(ActiveLayer);
		}

		ToolBar->GetToolBarCtrl().PressButton(ID_TOOLBAR_PARAMBAR,ParamViewFlag);
		Frm->ShowControlBar(ParamBar, ParamViewFlag, FALSE);	
		if (View) UpdateView(View);

}

/*****************************************************************************/
void	CCore::ToggleParamView(CMapEditView *View)
{
	UpdateParamBar(View,!ParamViewFlag);
}

/*****************************************************************************/
void	CCore::SetActiveLayer(int i)
{
	UpdateParamBar(NULL,ParamViewFlag);
}


/*****************************************************************************/
/*** TileBank ****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::UpdateTileView(CMapEditView *View,BOOL ViewFlag)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();

			TileViewFlag=ViewFlag;
			ToolBar->GetToolBarCtrl().PressButton(ID_TOOLBAR_TILEPALETTE,TileViewFlag);
			UpdateView(View);
}

/*****************************************************************************/
void	CCore::ToggleTileView(CMapEditView *View)
{
	UpdateTileView(View,!TileViewFlag);
}

/*****************************************************************************/
/*** Misc ********************************************************************/
/*****************************************************************************/
Vec		&CCore::GetCam()
{
	if (GetTileView())
		return(TileCam);
	else
		return(MapCam);

}

/*****************************************************************************/
void	CCore::UpdateAll(CMapEditView *View)
{
	UpdateView(View);
	UpdateParamBar(View,ParamViewFlag);
	UpdateTileView(View,TileViewFlag);
}

/*****************************************************************************/
void	CCore::UpdateView(CMapEditView *View,Vec Ofs)
{
Vec		&ThisCam=GetCam();

		Ofs.y=-Ofs.y;
		ThisCam+=Ofs;
		if (ThisCam.z>-1) ThisCam.z=-1;
		RenderFlag=TRUE;
		View->Invalidate();
}		
