/*******************/
/*** Layer Shade ***/
/*******************/


#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerShade.h"
#include	"Utils.h"
#include	"Select.h"
#include	"Export.h"
#include	"GUILayerShade.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerShade::CLayerShade(sLayerDef &Def)
{
		InitLayer(Def);
}


/*****************************************************************************/
CLayerShade::~CLayerShade()
{
}

/*****************************************************************************/
void	CLayerShade::InitLayer(sLayerDef &Def)
{
		CLayer::InitLayer(Def);
		LayerDef.Width=TileLayerMinWidth+(Def.Width-TileLayerMinWidth)/GetScaleFactor();
		LayerDef.Height=TileLayerMinHeight+(Def.Height-TileLayerMinHeight)/GetScaleFactor();

		RGB[0].rgbRed=255;	RGB[0].rgbGreen=255;	RGB[0].rgbBlue=255;
		RGB[1].rgbRed=255;	RGB[1].rgbGreen=0;		RGB[1].rgbBlue=0;
		RGB[2].rgbRed=0;	RGB[2].rgbGreen=255;	RGB[2].rgbBlue=0;
		RGB[3].rgbRed=0;	RGB[3].rgbGreen=0;		RGB[3].rgbBlue=255;
		Count=2;

}
/*****************************************************************************/
void	CLayerShade::Load(CFile *File,int Version)
{
		if (Version<=5)
		{
			BOOL	DB;
			float	DF;
			LayerDef.Type=LAYER_TYPE_SHADE;
			File->Read(&DB,sizeof(BOOL));
			File->Read(&DF,sizeof(float));
			File->Read(&DB,sizeof(BOOL));
			File->Read(&LayerDef.VisibleFlag,sizeof(BOOL));
			File->Read(&LayerDef.SubType,sizeof(int));
			File->Read(&LayerDef.Width,sizeof(int));
			File->Read(&LayerDef.Height,sizeof(int));
		}
		InitLayer(LayerDef);
		File->Read(&Count,sizeof(int));
		if (Count<2) Count=2;
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			File->Read(&Pos[i],sizeof(int));
			File->Read(&RGB[i],sizeof(RGBQUAD));
		}

}

/*****************************************************************************/
void	CLayerShade::Save(CFile *File)
{
// Always Save current version
		File->Write(&Count,sizeof(int));
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			File->Write(&Pos[i],sizeof(int));
			File->Write(&RGB[i],sizeof(RGBQUAD));
		}

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerShade::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
int			ThisCount=Count-1;
float		X0=0;
float		X1=LayerDef.Width;
float		Y=(0+1);

float		YInc=(float)LayerDef.Height/(float)ThisCount;

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);		// Set scroll offset
			glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

			glBegin (GL_QUADS);
			for (int i=0; i<ThisCount; i++)
			{
					glColor3ub(RGB[i+0].rgbRed,RGB[i+0].rgbGreen,RGB[i+0].rgbBlue);
					glVertex3f( X0,Y,0.0f);
					glVertex3f( X1,Y,0.0f);
					Y-=YInc;
					glColor3ub(RGB[i+1].rgbRed,RGB[i+1].rgbGreen,RGB[i+1].rgbBlue);
					glVertex3f( X1,Y,0.0f);
					glVertex3f( X0,Y,0.0f);
			}
			glEnd();
		glPopMatrix();
}


/*****************************************************************************/
bool	CLayerShade::Resize(int Width,int Height)
{
		LayerDef.Width=TileLayerMinWidth+(Width-TileLayerMinWidth)/GetScaleFactor();
		LayerDef.Height=TileLayerMinHeight+(Height-TileLayerMinHeight)/GetScaleFactor();

		return(true);
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerShade::GUIInit(CCore *Core)
{
		Core->GUIAdd(GUI,IDD_LAYER_SHADE);
		for (int i=0; i<LAYER_SHADE_MAX; i++) 
		{
			GUI.SetRGB(RGB[i],i);
		}
		GUI.SetCount(Count);
}

/*****************************************************************************/
void	CLayerShade::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUI,IDD_LAYER_SHADE);
}

/*****************************************************************************/
void	CLayerShade::GUIUpdate(CCore *Core)
{
		GUI.CheckData();
}

/*****************************************************************************/
void	CLayerShade::GUIChanged(CCore *Core)
{
		for (int i=0; i<LAYER_SHADE_MAX; i++) 
		{
			GUI.GetRGB(RGB[i],i);
		}
		GUI.GetCount(Count);
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
void	CLayerShade::Export(CCore *Core,CExport &Exp)
{
		Exp.ExportLayerHeader(LayerDef);//LAYER_TYPE_SHADE,LayerDef.SubType,LayerDef.Width,LayerDef.Height);
		Exp.Write(&Count,sizeof(int));
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			Exp.Write(&Pos[i],sizeof(int));
			Exp.Write(&RGB[i].rgbRed,sizeof(u8));
			Exp.Write(&RGB[i].rgbGreen,sizeof(u8));
			Exp.Write(&RGB[i].rgbBlue,sizeof(u8));
			Exp.Write(&RGB[i].rgbRed,sizeof(u8));	// Pad
		}
}
