/***********************/
/*** Data Structures ***/
/***********************/

#ifndef		__DATA_STRUCTS_HEADER__
#define		__DATA_STRUCTS_HEADER__


//***************************************************************************
// biped bone IDs
/*
enum BONE_NAME
{
	BIP0,
	BIP01_PELVI,
	BIP01_SPIN,
	BIP01_SPINE,
	BIP01_NEC,
	BIP01_HEA,
	BIP01_L_CLAVICL,
	BIP01_L_UPPERAR,
	BIP01_L_FOREAR,
	BIP01_L_HAN,
	BIP01_R_CLAVICL,
	BIP01_R_UPPERAR,
	BIP01_R_FOREAR,
	BIP01_R_HAN,
	BIP01_L_THIG,
	BIP01_L_CAL,
	BIP01_L_FOO,
	BIP01_L_TOE,
	BIP01_R_THIG,
	BIP01_R_CAL,
	BIP01_R_FOO,
	BIP01_R_TOE0,
	MAX_BONE
};
*/
//***************************************************************************
#ifndef	sQuat
struct 	sQuat
{
	s16		vx,vy,vz,vw;
};
#endif

struct sShortXYZ
{
		s16		vx,vy,vz;
};

//***************************************************************************
struct	sVtx
{
		s16	vx, vy, vz, pad;

#ifdef	WIN32
bool	operator==(sVtx const &v1)	{return((vx==v1.vx) && (vy==v1.vy) && (vz==v1.vz));}
#endif
};

#define	NormalScale	4096
struct	sNormal
{
		s16		nX,nY,nZ,pD;
};

//---------------------------------------------------------------------------
struct  sBBox
{
        s16			XMin,YMin,ZMin;
        s16			XMax,YMax,ZMax;
};

//***************************************************************************

struct	sMat
{
		u16			TPage;
		u16			Clut;
//		s32			DblFlag;
};

//***************************************************************************
struct	sTri
{
		u16			P0;						//  2
		u16			P1;						//  2
		u8			uv0[2];					//  2
		u16			Clut;					//  2
		u8			uv1[2];					//  2
		u16			TPage;					//  2
		u8			uv2[2];					//  2
		u16			P2;						//  2
};											// 16

//---------------------------------------------------------------------------
struct	sQuad
{
		u16			P0;						//  2
		u16			P1;						//  2
		u16			P2;						//  2
		u16			P3;						//  2
		u8			uv0[2];					//  2
		u16			Clut;					//  2
		u8			uv1[2];					//  2
		u16			TPage;					//  2
		u8			uv2[2];					//  2
		u8			uv3[2];					//  2
};											// 20

//***************************************************************************
struct sWeight
{
		s16		vx,vy,vz,VtxNo;				//  8
};

//***************************************************************************
struct sBone
{
		sVtx		BoneSize;				//  8
		s16			Parent,Idx;				//  4
		s32			WeightCount;			//  4
		sWeight		*WeightList;			//  4
};											// 20

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Maps

enum	TILE3D_FLAGS
{
	TILE3D_FLAGS_F=0,
	TILE3D_FLAGS_U,
	TILE3D_FLAGS_D,
	TILE3D_FLAGS_L,
	TILE3D_FLAGS_R,
//	TILE3D_FLAGS_B,	// Should never have back facing polys
	TILE3D_FLAGS_MAX
};

struct	sTileMapElem
{
		u16		Tile;
};

struct	sTileMapElem3d : public sTileMapElem
{
		u16		Flags;
};

/*
struct	sTileTable
{
		u16		TriList;
		u16		TriCount;
};
*/
struct	sTile
{
// 3d Tile
//		sTileTable	TileTable[TILE3D_FLAGS_MAX];	// 20 (4*5)
		u16		TriStart;							// 2
		u16		TileTable[TILE3D_FLAGS_MAX];		// 10
// 2d Tile
		u8		u0,v0;								// 2
		u16		Clut;								// 2
		u16		TPage;								// 2
		u16		Pad;	// :o( need this?			// 2

#ifdef	WIN32
bool	operator==(sTile const &v1)	{return(false);}
#endif
};													// 20

//---------------------------------------------------------------------------
// Layers
struct	sLayerHdr
{
	int		Type;
	int		SubType;
	int		Width;
	int		Height;

	/*int	TileData[W][H];....*/
};

//---------------------------------------------------------------------------
// Header
struct	sLvlHdr
{
	u32		TriList;
	u32		QuadList;
	u32		VtxList;
	u32		TileList;
// Layers
	u32		BackLayer;
	u32		MidLayer;
	u32		ActionLayer;
	u32		ForeLayer;
	u32		Pad0;
	u32		Pad1;
	u32		Pad2;
	u32		Pad3;
	u32		Pad4;
	u32		Pad5;
	u32		Pad6;
	u32		Pad7;

};


//***************************************************************************
//***************************************************************************
//***************************************************************************
// Actors

struct	sActor3dHdr
{
	u16		NodeCount;
	u16		TriCount;
	u16		QuadCount;
	u16		WeightCount;

	u32		NodeData;
	u32		TriData;
	u32		QuadData;
	u32		WeightData;
};


#endif