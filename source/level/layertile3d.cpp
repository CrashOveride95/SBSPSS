/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#include 	"system\global.h"
#include	"system\vid.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"game\game.h"

#include	"LayerTile.h"
#include	"LayerTile3d.h"


#if		defined(__USER_sbart__) || defined(__USER_daveo__)
#define	_SHOW_POLYZ_	1
#include	"gfx\font.h"	
static		FontBank		*Font;
#endif

static const int	BLOCK_SIZE				=16;
static const int	SCREEN_TILE_ADJ_U		=2;
static const int	SCREEN_TILE_ADJ_D		=1;
static const int	SCREEN_TILE_ADJ_L		=3;
static const int	SCREEN_TILE_ADJ_R		=3;

static const int	SCREEN_TILE3D_WIDTH		=(INGAME_SCREENW/BLOCK_SIZE)+SCREEN_TILE_ADJ_L+SCREEN_TILE_ADJ_R;
static const int	SCREEN_TILE3D_HEIGHT	=(INGAME_SCREENH/BLOCK_SIZE)+SCREEN_TILE_ADJ_U+SCREEN_TILE_ADJ_D;

static const int	RENDER_X_OFS			=INGAME_SCREENOFS_X-(SCREEN_TILE_ADJ_L*BLOCK_SIZE)+INGAME_RENDER_OFS_X;
static const int	RENDER_Y_OFS			=INGAME_SCREENOFS_Y-(SCREEN_TILE_ADJ_U*BLOCK_SIZE)+INGAME_RENDER_OFS_Y;

static const int	DeltaTableSizeX=SCREEN_TILE3D_WIDTH+1;
static const int	DeltaTableSizeY=SCREEN_TILE3D_HEIGHT+1;


/*****************************************************************************/
// 0 LUF
// 1 RUF
// 2 LDF
// 3 RDF

// 4 LUB
// 5 RUB
// 6 LDB
// 7 RDB

sFlipTable	FlipTable[4]=
{
/*00 <0*/	{{+4096,0,+4096,0},	{	((DVECTOR*)SCRATCH_RAM)+0,
									((DVECTOR*)SCRATCH_RAM)+1,
									((DVECTOR*)SCRATCH_RAM)+2,
									((DVECTOR*)SCRATCH_RAM)+3,
									((DVECTOR*)SCRATCH_RAM)+4,
									((DVECTOR*)SCRATCH_RAM)+5,
									((DVECTOR*)SCRATCH_RAM)+6,
									((DVECTOR*)SCRATCH_RAM)+7,
								}, 0<<31},	
/*01 >0*/	{{-4096,0,+4096,0},	{	((DVECTOR*)SCRATCH_RAM)+1,
									((DVECTOR*)SCRATCH_RAM)+0,
									((DVECTOR*)SCRATCH_RAM)+3,
									((DVECTOR*)SCRATCH_RAM)+2,
									((DVECTOR*)SCRATCH_RAM)+5,
									((DVECTOR*)SCRATCH_RAM)+4,
									((DVECTOR*)SCRATCH_RAM)+7,
									((DVECTOR*)SCRATCH_RAM)+6,
								},1<<31},
/*10 >0*/	{{+4096,0,-4096,0},	{	((DVECTOR*)SCRATCH_RAM)+2,
									((DVECTOR*)SCRATCH_RAM)+3,
									((DVECTOR*)SCRATCH_RAM)+0,
									((DVECTOR*)SCRATCH_RAM)+1,
									((DVECTOR*)SCRATCH_RAM)+6,
									((DVECTOR*)SCRATCH_RAM)+7,
									((DVECTOR*)SCRATCH_RAM)+4,
									((DVECTOR*)SCRATCH_RAM)+5,
								},1<<31},
/*11 <0*/	{{-4096,0,-4096,0},	{	((DVECTOR*)SCRATCH_RAM)+3,
									((DVECTOR*)SCRATCH_RAM)+2,
									((DVECTOR*)SCRATCH_RAM)+1,
									((DVECTOR*)SCRATCH_RAM)+0,
									((DVECTOR*)SCRATCH_RAM)+7,
									((DVECTOR*)SCRATCH_RAM)+6,
									((DVECTOR*)SCRATCH_RAM)+5,
									((DVECTOR*)SCRATCH_RAM)+4,
								},0<<31}
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile3d::CLayerTile3d(sLevelHdr *LevelHdr,sLayerHdr *Hdr) : CLayerTile(LevelHdr,Hdr)
{
		ElemBank3d=LevelHdr->ElemBank3d;
		TriList=LevelHdr->TriList;
		QuadList=LevelHdr->QuadList;
		VtxList=LevelHdr->VtxList;
		VtxIdxList=LevelHdr->VtxIdxList;

#if		defined(_SHOW_POLYZ_)
		Font=new ("PrimFont") FontBank;
		Font->initialise( &standardFont );
		Font->setOt( 0 );
		Font->setTrans(1);
#endif
}

/*****************************************************************************/
CLayerTile3d::~CLayerTile3d()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

void	CLayerTile3d::init(DVECTOR &MapPos,int Shift)
{
		CLayerTile::init(MapPos,Shift);
		CalcDelta();
}

/*****************************************************************************/
void	CLayerTile3d::shutdown()
{
#if		defined(_SHOW_POLYZ_)
		Font->dump();
		delete Font;
#endif
		for (int i=0; i<16; i++)
		{
			MemFree(FTableX[i]);
			MemFree(FTableY[i]);
			MemFree(BTableX[i]);
			MemFree(BTableY[i]);
		}

}

/*****************************************************************************/
void	CLayerTile3d::CalcDelta()
{
VECTOR	BlkPos;
SVECTOR	Pnt={-BLOCK_SIZE/2,-BLOCK_SIZE/2,-BLOCK_SIZE*4};
s16		*FTab,*BTab;
		CGameScene::setCameraMtx();

		for (int i=0; i<16; i++)
		{
			FTab=(s16*)MemAlloc(DeltaTableSizeX*sizeof(s16),"FTableXTable");
			BTab=(s16*)MemAlloc(DeltaTableSizeX*sizeof(s16),"BTableXTable");
			FTableX[i]=FTab;
			BTableX[i]=BTab;
			ASSERT(FTab);
			ASSERT(BTab);
			BlkPos.vx=RENDER_X_OFS-i;
			BlkPos.vy=RENDER_Y_OFS;
			for (int x=0; x<DeltaTableSizeX; x++)
			{
				s32		Tmp;
				DVECTOR	O;
				CMX_SetTransMtxXY(&BlkPos);
				Pnt.vz=-BLOCK_SIZE*4;
				RotTransPers(&Pnt,(s32*)&O,&Tmp,&Tmp);
				FTab[x]=O.vx;
				Pnt.vz=+BLOCK_SIZE*4;
				RotTransPers(&Pnt,(s32*)&O,&Tmp,&Tmp);
				BTab[x]=O.vx;
				BlkPos.vx+=BLOCK_SIZE;
			}

			FTab=(s16*)MemAlloc(DeltaTableSizeY*sizeof(s16),"FTableYTable");
			BTab=(s16*)MemAlloc(DeltaTableSizeY*sizeof(s16),"BTableYTable");
			FTableY[i]=FTab;
			BTableY[i]=BTab;
			ASSERT(FTab);
			ASSERT(BTab);
			BlkPos.vx=RENDER_X_OFS;
			BlkPos.vy=RENDER_Y_OFS-i;
			for (int y=0; y<DeltaTableSizeY; y++)
			{
				s32		Tmp;
				DVECTOR	O;
				CMX_SetTransMtxXY(&BlkPos);
				Pnt.vz=-BLOCK_SIZE*4;
				RotTransPers(&Pnt,(s32*)&O,&Tmp,&Tmp);
				FTab[y]=O.vy;
				Pnt.vz=+BLOCK_SIZE*4;
				RotTransPers(&Pnt,(s32*)&O,&Tmp,&Tmp);
				BTab[y]=O.vy;
				BlkPos.vy+=BLOCK_SIZE;
			}
		}
		DeltaF=FTableX[0][1]-FTableX[0][0];
		DeltaB=BTableY[0][1]-BTableY[0][0];

}

/*****************************************************************************/
void	CLayerTile3d::think(DVECTOR &MapPos)
{
			MapXY.vx=MapPos.vx>>4;
			MapXY.vy=MapPos.vy>>4;
			
			MapXY.vx-=SCREEN_TILE_ADJ_L;
			MapXY.vy-=SCREEN_TILE_ADJ_U;

			ShiftX=(MapPos.vx & 15);
			ShiftY=(MapPos.vy & 15);

			RenderOfs.vx=RenderOfs.vy=0;
			DeltaFOfs.vx=DeltaFOfs.vy=0;
			DeltaBOfs.vx=DeltaBOfs.vy=0;
			if (MapXY.vx<0) 
			{
				RenderOfs.vx=-MapXY.vx*BLOCK_SIZE;
				DeltaFOfs.vx=-MapXY.vx*DeltaF;
				DeltaBOfs.vx=-MapXY.vx*DeltaB;
				MapXY.vx=0;
			}
			if (MapXY.vy<0) 
			{
				RenderOfs.vy=-MapXY.vy*BLOCK_SIZE;
				DeltaFOfs.vy=-MapXY.vy*DeltaF;
				DeltaBOfs.vy=-MapXY.vy*DeltaB;
				MapXY.vy=0;
			}

			if (MapXY.vx+SCREEN_TILE3D_WIDTH<=MapWidth)
				RenderW=SCREEN_TILE3D_WIDTH;
			else
				RenderW=MapWidth-MapXY.vx;

			if (MapXY.vy+SCREEN_TILE3D_HEIGHT<=MapHeight)
				RenderH=SCREEN_TILE3D_HEIGHT;
			else
				RenderH=MapHeight-MapXY.vy;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile3d::CacheElemVtx(sElem3d *Elem)
{
int		Count=Elem->VtxTriCount;
sVtx	*V0,*V1,*V2;
u16		*IdxTable=&VtxIdxList[Elem->VtxIdxStart];
s32		*OutVtx=(s32*)SCRATCH_RAM;
s32		*OutPtr;

		OutVtx+=8;

		V0=&VtxList[*IdxTable++];
		V1=&VtxList[*IdxTable++];
		V2=&VtxList[*IdxTable++];
		gte_ldv3(V0,V1,V2);

		while (Count--)
		{
			gte_rtpt_b(); // 22 cycles
// Preload next (when able) - Must check this
			V0=&VtxList[*IdxTable++];
			V1=&VtxList[*IdxTable++];
			V2=&VtxList[*IdxTable++];
			OutPtr=OutVtx;
			OutVtx+=3;
			gte_ldv3(V0,V1,V2);
			gte_stsxy3c(OutPtr);	// read XY back
		}

}
/*****************************************************************************/
void	CLayerTile3d::render()
{
sTileMapElem	*MapPtr=GetMapPos();
u8				*PrimPtr=GetPrimPtr();
POLY_FT3		*TPrimPtr=(POLY_FT3*)PrimPtr;
u32				*XYList=(u32*)SCRATCH_RAM;
u32				T0,T1,T2;
u32				P0,P1,P2;
s32				ClipZ;
sOT				*ThisOT;
VECTOR			BlkPos;
DVECTOR			*DP0,*DP1,*DP2,*DP3;

s16				*DeltaFY=FTableY[ShiftY];
s16				*DeltaBY=BTableY[ShiftY];

// Setup Trans Matrix
		BlkPos.vx=RENDER_X_OFS-(ShiftX)+RenderOfs.vx;
		BlkPos.vy=RENDER_Y_OFS-(ShiftY)+RenderOfs.vy;

		for (int Y=0; Y<RenderH; Y++)
		{
			sTileMapElem	*MapRow=MapPtr;
			s32				BlkXOld=BlkPos.vx;
			s16				*DeltaFX=FTableX[ShiftX];
			s16				*DeltaBX=BTableX[ShiftX];

			for (int X=0; X<RenderW; X++)
			{
				u16			Tile=MapRow->Tile;
				u16			TileIdx=Tile>>2;
				u16			Flip=Tile&3;
				sFlipTable	*FTab=&FlipTable[Flip];
				sElem3d		*Elem=&ElemBank3d[TileIdx];

				int			TriCount=Elem->TriCount;				
				sTri		*TList=&TriList[Elem->TriStart];

				if (TriCount)	// Blank tiles rejected here, to prevent over processing (as no tri-count)
				{
					CMX_SetTransMtxXY(&BlkPos);
					CMX_SetRotMatrixXY(&FTab->Mtx);

					CacheElemVtx(Elem);

					s16	FL=DeltaFX[0]+DeltaFOfs.vx;
					s16	FR=DeltaFX[1]+DeltaFOfs.vx;
					s16	FU=DeltaFY[0]+DeltaFOfs.vy;
					s16	FD=DeltaFY[1]+DeltaFOfs.vy;
					DP0=FTab->DeltaTab[0];
					DP1=FTab->DeltaTab[1];
					DP2=FTab->DeltaTab[2];
					DP3=FTab->DeltaTab[3];
					DP0->vx=FL;
					DP0->vy=FU;
					DP1->vx=FR;
					DP1->vy=FU;
					DP2->vx=FL;
					DP2->vy=FD;
					DP3->vx=FR;
					DP3->vy=FD;

					s16	BL=DeltaBX[0]+DeltaBOfs.vx;
					s16	BR=DeltaBX[1]+DeltaBOfs.vx;
					s16	BU=DeltaBY[0]+DeltaBOfs.vy;
					s16	BD=DeltaBY[1]+DeltaBOfs.vy;
					DP0=FTab->DeltaTab[4];
					DP1=FTab->DeltaTab[5];
					DP2=FTab->DeltaTab[6];
					DP3=FTab->DeltaTab[7];
					DP0->vx=BL;
					DP0->vy=BU;
					DP1->vx=BR;
					DP1->vy=BU;
					DP2->vx=BL;
					DP2->vy=BD;
					DP3->vx=BR;
					DP3->vy=BD;

					while (TriCount--)
					{
						P0=XYList[TList->P0]; 
						P1=XYList[TList->P1]; 
						P2=XYList[TList->P2];
						gte_ldsxy0(P0);
						gte_ldsxy1(P1);
						gte_ldsxy2(P2);
						
						setlen(TPrimPtr, GPU_PolyFT3Tag);
						TPrimPtr->code=TList->PolyCode;
						gte_nclip_b();	// 8 cycles

						setShadeTex(TPrimPtr,1);

						T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
						T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
						T2=*(u16*)&TList->uv2;		// Get UV2
						*(u32*)&TPrimPtr->u0=T0;	// Set UV0
						*(u32*)&TPrimPtr->u1=T1;	// Set UV1
						*(u16*)&TPrimPtr->u2=T2;	// Set UV2
						gte_stopz(&ClipZ);
						ThisOT=OtPtr+TList->OTOfs;
						ClipZ^=FTab->ClipCode;
						TList++;
						if (ClipZ<0)
						{
							*(u32*)&TPrimPtr->x0=P0;	// Set XY0
							*(u32*)&TPrimPtr->x1=P1;	// Set XY1
							*(u32*)&TPrimPtr->x2=P2;	// Set XY2
							addPrim(ThisOT,TPrimPtr);
							TPrimPtr++;
						}
					}
				}
				MapRow++;
				BlkPos.vx+=BLOCK_SIZE;
				DeltaFX++; DeltaBX++;
			}
			MapPtr+=MapWidth;
			BlkPos.vx=BlkXOld;
			BlkPos.vy+=BLOCK_SIZE;
			DeltaFY++; DeltaBY++;
		}

		SetPrimPtr((u8*)TPrimPtr);

#if		defined(_SHOW_POLYZ_)
char	Txt[256];
int		TCount=((u8*)TPrimPtr-PrimPtr)/sizeof(POLY_FT3);
int		QCount=0;
		sprintf(Txt,"TC %i\nQC %i",TCount,QCount);
		Font->print( 128, 32, Txt);
#endif

}
