/******************/
/*** Layer Tile ***/
/******************/

#ifndef	__LAYER_TILE_HEADER__
#define	__LAYER_TILE_HEADER__

#include	"Layer.h"

/*****************************************************************************/
enum	TileLayerEnum
{
	TileLayerMinWidth=32,
	TileLayerMinHeight=22,
};

/*****************************************************************************/
class	CCore;
class	CMapEditView;
class	CLayerTile : public CLayer
{

public:
		enum	MouseMode
		{
			MouseModePaint=0,
			MouseModeSelect,
		};

		CLayerTile(int SubType,int Width,int Height,float Scale,BOOL Is3d,BOOL Resizable);					// New Layer
		CLayerTile(CFile *File,int Version);																// Load Layer
		~CLayerTile();

		int				GetType()			{return(LAYER_TYPE_TILE);}
		int				GetSubType()		{return(SubType);}
		char			*GetName()			{return(LayerName[SubType]);}

		void			Render(CCore *Core,Vector3 &CamPos,BOOL Is3d);
		void			RenderGrid(CCore *Core,Vector3 &CamPos,BOOL Active);
		void			RenderSelection(CCore *Core,Vector3 &ThisCam);

		void			FindCursorPos(CCore *Core,CMapEditView *View,Vector3 &CamPos,CPoint &MousePos);
		void			RenderCursor(CCore *Core,Vector3 &CamPos,BOOL Is3d);

		void			InitGUI(CCore *Core);
		void			UpdateGUI(CCore *Core);

		int				GetWidth()			{return(Map.GetWidth());}
		int				GetHeight()			{return(Map.GetHeight());}
		BOOL			Resize(int Width,int Height);

		void			Load(CFile *File,float Version);
		void			Save(CFile *File);
		void			CheckLayerSize(int Width,int Height);

		void			Export(CCore *Core,CExport &Exp);

// Functions
		BOOL			SetMode(int NewMode);
		BOOL			InitMode();
		BOOL			ExitMode();

		BOOL			LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		BOOL			RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
		BOOL			MouseMove(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &point);

		BOOL			MirrorX(CCore *Core);
		BOOL			MirrorY(CCore *Core);

		BOOL			CopySelection(CCore *Core);
		BOOL			PasteSelection(CCore *Core);

		void			DeleteSet(int Set);
		void			RemapSet(int OrigSet,int NewSet);


protected:
		void			Render(CCore *Core,Vector3 &CamPos,CMap &ThisMap,BOOL Render3d,float Alpha=1.0f);
		void			RenderCursorPaint(CCore *Core,Vector3 &CamPos,BOOL Is3d);

		BOOL			Paint(CMap &Blk,CPoint &CursorPos);

		CMap			Map;
		int				SubType;
		MouseMode		Mode;

static	char			*LayerName[];

};

/*****************************************************************************/
#endif
