/******************/
/*** Layer Core ***/
/******************/

#ifndef	__LAYER_HEADER__
#define	__LAYER_HEADER__

#include	<Vector>
#include	<Vector3.h>
#include	"Map.h"
#include	"Select.h"
#include	"LayerDef.h"

/*****************************************************************************/
struct	sLayerInfoTable
{
	LAYER_TYPE		Type;
	LAYER_SUBTYPE	SubType;
	char			*Name;

	bool			DeleteFlag;
	float			ScaleFactor;
	bool			Render3dFlag;
	bool			ResizeFlag;
	bool			ExportFlag;
	int				SubView;

};

/*****************************************************************************/
enum	LAYER_ENUMS
{
SELECT_BUFFER_SIZE=16,
TileLayerMinWidth=32,
TileLayerMinHeight=22,
};

/*****************************************************************************/
class	CCore;
class	CExport;
class	CLayer
{
public:
		CLayer();
virtual	~CLayer(){};

static	sLayerInfoTable	InfoTable[];
static	int				InfoTableSize;

static	int				GetLayerIdx(int Type,int SubType);

		void			SetDefaultParams();		

		char			*GetName()						{return(InfoTable[GetLayerIdx(GetType(),GetSubType())].Name);}
		bool			CanDelete()						{return(InfoTable[GetLayerIdx(GetType(),GetSubType())].DeleteFlag);}
		bool			CanExport()						{return(InfoTable[GetLayerIdx(GetType(),GetSubType())].ExportFlag);}
		bool			IsUnique()						{return(!(InfoTable[GetLayerIdx(GetType(),GetSubType())].ExportFlag));}
		int				GetSubViewType()				{return((InfoTable[GetLayerIdx(GetType(),GetSubType())].SubView));}

virtual	void			InitSubView(CCore *Core){};
virtual	CLayer			*GetSubView()					{return(SubView);}


virtual	void			SetVisible(bool f)				{VisibleFlag=f;}
virtual	bool			IsVisible()						{return(VisibleFlag);}
virtual	int				GetType()=0;
virtual	int				GetSubType()					{return(LAYER_SUBTYPE_NONE);}
		float			GetScaleFactor()				{return(ScaleFactor);}

virtual	void			Render(CCore *Core,Vector3 &CamPos,bool Is3d){};
virtual	void			RenderGrid(CCore *Core,Vector3 &CamPos,bool Active);
virtual void			RenderSelection(CCore *Core,Vector3 &ThisCam){};
virtual	void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d){};
virtual	void			FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);

virtual	void			GUIInit(CCore *Core){};
virtual	void			GUIKill(CCore *Core){};
virtual	void			GUIUpdate(CCore *Core){};
virtual	void			GUIChanged(CCore *Core){};

virtual	int				GetWidth()						{return(-1);}
virtual	int				GetHeight()						{return(-1);}
virtual	void			CheckLayerSize(int Width,int Height){};
virtual	bool			Resize(int Width,int Height)	{return(false);}

virtual	void			Load(CFile *File,int Version)=0;
virtual	void			Save(CFile *File)=0;

virtual	void			Export(CCore *Core,CExport &Exp)=0;

virtual	Vector3			&GetCam()						{return(LayerCam);}

// Functions
virtual	bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag){return(false);}
virtual	bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag){return(false);}
virtual	bool			MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos){return(false);}
virtual	bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0){return(false);};

protected:
		float			ScaleFactor;
		bool			Render3dFlag;
		bool			ResizeFlag;

		bool			VisibleFlag;
		CSelect			Selection;
		CLayer			*SubView;
		Vector3			LayerCam;

};


/*****************************************************************************/
#endif
