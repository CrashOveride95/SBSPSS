/*************************/
/*** Base Level Holder ***/
/*************************/

#ifndef	__MKLEVEL_HEADER__
#define	__MKLEVEL_HEADER__

#include	<vector>
#include	<List.h>
#include	<FaceStore.h>
#include	"IniClass.h"

// Map Editor header files
#include	"..\mapedit\LayerDef.h"
#include	"..\mapedit\ExportHdr.h"

using namespace std;

//***************************************************************************
struct	sMkLevelTex
{
		int		Set;
		int		XOfs,YOfs;
		u8		*RGB;
		int		Flags;
		int		RChk,GChk,BChk;
		int		TexID;

bool	operator ==(sMkLevelTex const &v1)
		{
		if (Set!=v1.Set) return(false);
		if (XOfs!=v1.XOfs) return(false);
		if (YOfs!=v1.YOfs) return(false);
		if (Flags!=v1.Flags) return(false);
		return(true);
		}
};

//***************************************************************************
class	CMkLevelLayer;
class	CMkLevel
{
public:
		CMkLevel();
		~CMkLevel();

		void			Init(const char *InFilename,const char *OutPath,int TPBase,int TPW,int TPH);

		void			Load();

		void			Process();
		int				AddTile3d(sExpLayerTile &Tile)	{return(Tile3dList.Add(Tile));}
		int				AddTile2d(sExpLayerTile &Tile)	{return(Tile2dList.Add(Tile));}

		void			Write();

		int				Create2dTex(sExpLayerTile &ThisTile);
		int				Create3dTile(sExpLayerTile &ThisTile);
		int				FindRGBMatch(sMkLevelTex &ThisTex);
		bool			IsRGBSame(const sMkLevelTex &Tile0,const sMkLevelTex &Tile1);
		void			MakeTexName(sMkLevelTex &InTex,GString &OutStr);
		int				BuildTileTex(sMkLevelTex &InTex);

		char			*GetConfigStr(const char *Grp,const char *Key)	{return(Config.GetStr(Grp,Key));}
		CTexGrab		&GetTexGrab()									{return(TexGrab);}

		void			SetStart(int X,int Y)				{LvlHdr.PlayerStartX=X; LvlHdr.PlayerStartY=Y;}
protected:	
		CMkLevelLayer	*FindLayer(int Type,int SubType);
		void			LoadStrList(CList<GString> &List,char *TexPtr,int Count);

		void			LoadTiles(sExpFileHdr *FileHdr);
		void			LoadLayers(sExpFileHdr *FileHdr);
		void			LoadLayer(sExpLayerHdr *LayerHdr);

		void			PreProcessLayers();
		void			ProcessTileBanks();
		void			PreProcessTileBank2d();
		void			ProcessTileBank2d();
		void			PreProcessTileBank3d();
		void			ProcessTileBank3d();
		void			ProcessLayers();
		void            SetUpTileUV(sTile2d &Out, sTexOutInfo &Info);

		void			WriteLevel();
		void			WriteLayers();
		int				WriteLayer(int Type,int SubType,const char *LayerName);
		int				WriteThings(int Type,const char *LayerName);
		int				WriteTriList();
		int				WriteQuadList();
		void			WriteTileBank();

		void            BuildTiles();

		FILE					*File;
		GString					InFilename,InPath,LevelName;
		GString					OutPath,OutName;

		int						TileW,TileH;
		CIni					Config;

		CList<sExpTri>			InTriList;
		CList<sExpTile>			InTileList;
		CList<GString>			InSetNameList;
		CList<GString>			InTexNameList;
		CList<GString>			UsedSetNameList;
		CList<GString>			UsedTexNameList;

		CFaceStore				OutTriList;
		CList<sTile2d>			OutTile2dList;
		CList<sTile3d>			OutTile3dList;

		CList<sExpLayerTile>	Tile2dList;
		CList<sExpLayerTile>	Tile3dList;
		CList<sMkLevelTex>		Tex2dList;
		CTexGrab				TexGrab;
		CList<Frame>			BmpList;

		vector<CMkLevelLayer*>	LayerList;

		sLvlHdr					LvlHdr;
		sTileBankHdr			TileBankHdr;
		CFace					FlatFace[2];

};

//***************************************************************************
#endif
