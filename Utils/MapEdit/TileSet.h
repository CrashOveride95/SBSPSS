/*********************/
/*** TileSet Stuph ***/
/*********************/

#ifndef	__TILESET_HEADER__
#define	__TILESET_HEADER__

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<Vector>
#include	<gfname.hpp>

#include	"TexCache.h"
#include	"Tile.h"

#include	"MapEdit.h"
#include	"LayerTileGui.h"

/*****************************************************************************/
enum	TileSetEnum
{
DefTileBrowserWidth=8,
};

/*****************************************************************************/
class	CCore;
class	CTile;

/*****************************************************************************/
class	CTileSet;
class	CTileBank
{
public:
		CTileBank();
		~CTileBank();

		enum	BrushEnum
		{
			LBrush=0,
			RBrush,
			MaxBrush
		};

		void		AddTileSet(const char *Filename);
		int			NeedLoad()							{return(LoadFlag);}
		void		Delete();
		void		Reload();
		void		LoadTileSets(CCore *Core);
		CTile		&GetTile(int Bank,int Tile);
		
		void		SetCurrent()						{CurrentSet=TileBankGUI.m_List.GetCurSel()+1;}
		int			GetCurrent()						{return(CurrentSet);}
		int			GetSetCount()						{return(TileSet.size());}
		
		CMap		&GetLBrush()						{return(Brush[LBrush]);}
		CMap		&GetRBrush()						{return(Brush[RBrush]);}
		CMap		&GetBrush(int i)					{return(Brush[i]);}
		CMap		&GetActiveBrush()					{return(GetBrush(ActiveBrush));}

		void		RenderSet(CCore *Core,Vector3 &CamPos,BOOL Is3d);
		void		FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);
		void		RenderCursor(CCore *Core,Vector3 &CamPos,BOOL Is3d);

		void		GUIInit(CCore *Core);
		void		GUIKill(CCore *Core);
		void		GUIUpdate(CCore *Core);

		BOOL		IsTileValid(int Set,int Tile);
		bool		CanClose()							{return(SelStart==-1);}

		void		SetCollision(bool f);
		CTileSet	&GetSet(int Set)					{return(TileSet[Set]);}

// Functions
		BOOL		SelectL(BOOL DownFlag)				{return(Select(LBrush,DownFlag));}
		BOOL		SelectR(BOOL DownFlag)				{return(Select(RBrush,DownFlag));}
		BOOL		SelectCancel();

		void		SetActiveBrushL()					{ActiveBrush=LBrush;}
		void		SetActiveBrushR()					{ActiveBrush=RBrush;}

		void		Load(CFile *File,int Version);
		void		Save(CFile *File);


protected:
		BOOL		Select(int BrushID,BOOL DownFlag);
		void		SetBrush(CMap &ThisBrush);

//		std::vector<CTileSet>	TileSet;
		CList<CTileSet>			TileSet;
		int						CurrentSet,LastSet;
		CMap					Brush[2];
		int						ActiveBrush;
		int						SelStart,SelEnd;

		BOOL					LoadFlag;
		int						LastCursorPos,CursorPos;

		CLayerTileGUI			TileBankGUI;

};

/*****************************************************************************/
class	CTileSet
{
public:
		CTileSet(const char *_Filename,int Idx);
		~CTileSet();
		
		int			IsLoaded()			{return(Loaded);}
		int			GetTileCount()		{return(Tile.size());}

		void		Load(CCore *Core);
		void		Load2d(CCore *Core);
		void		Load3d(CCore *Core);

const	char		*GetFilename()		{return(Filename);}
const	char		*GetName()			{return(Name);}

		CTile		&GetTile(int No)	{return(Tile[No]);}
		void		Purge();
		int			FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);
		void		Render(CCore *Core,Vector3 &CamPos,CMap &LBrush,CMap &RBrush,BOOL Render3d);
		void		RenderCursor(Vector3 &CamPos,int Pos,int Width, int Height);
		void		RenderBrush(Vector3 &CamPos,CMap &LBrush,CMap &RBrush);
		void		RenderGrid(Vector3 &CamPos);
		int			GetTileBrowserWidth()		{return(TileBrowserWidth);}
		BOOL		IsTileValid(int No);

bool			operator==(CTileSet const &v1)		{return (Name==v1.Name);}

//			if (IsStrSame((char*)Name,(char*)v1.Name) return(i);

private:
		BOOL		Create16x16Tile(sRGBData &Src,u8 *Dst,int XOfs,int YOfs);
		CPoint		GetTilePos(int ID);

		GString			Filename,Name;
		
		int				SetNumber;
		CList<CTile>	Tile;
		BOOL			Loaded;
		int				TileBrowserWidth;
};

/*****************************************************************************/
#endif