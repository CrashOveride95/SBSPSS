/*************/
/*** Level ***/
/*************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"utils\utils.h"
#include	"utils\pak.h"
#include	"gfx\tpage.h"
#include	"gfx\prim.h"
#include	<DStructs.h>
#include	"game\game.h"

#include	"level\level.h"
#include	"level\layertile.h"
#include	"level\layerback.h"
#include	"level\layertile3d.h"
#include	"level\layercollision.h"

#include "gfx\font.h"

#ifndef	__TRIGGERS_TLEVEXIT_H__
#include "triggers\tlevexit.h"
#endif

#ifndef	__TRIGGERS_TLOOK_H__
#include "triggers\tlook.h"
#endif

#ifndef __PICKUPS_PICKUP_H__
#include "pickups\pickup.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#include	"enemy\npc.h"
#include	"gfx\actor.h"

/*****************************************************************************/
sLvlTab	LvlTable[]=
{
// C1
	{1,1,LEVELS_CHAPTER01_LEVEL01_LVL,LEVELS_CHAPTER01_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL1},
	{1,2,LEVELS_CHAPTER01_LEVEL02_LVL,LEVELS_CHAPTER01_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL2},
	{1,3,LEVELS_CHAPTER01_LEVEL03_LVL,LEVELS_CHAPTER01_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL3},
	{1,4,LEVELS_CHAPTER01_LEVEL04_LVL,LEVELS_CHAPTER01_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER1_LEVEL4},
// C2
	{2,1,LEVELS_CHAPTER02_LEVEL01_LVL,LEVELS_CHAPTER02_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL1},
	{2,2,LEVELS_CHAPTER02_LEVEL02_LVL,LEVELS_CHAPTER02_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL2},
	{2,3,LEVELS_CHAPTER02_LEVEL03_LVL,LEVELS_CHAPTER02_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL3},
	{2,4,LEVELS_CHAPTER02_LEVEL04_LVL,LEVELS_CHAPTER02_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER2_LEVEL4},
// C3
	{3,1,LEVELS_CHAPTER03_LEVEL01_LVL,LEVELS_CHAPTER03_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL1},
	{3,2,LEVELS_CHAPTER03_LEVEL02_LVL,LEVELS_CHAPTER03_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL2},
	{3,3,LEVELS_CHAPTER03_LEVEL03_LVL,LEVELS_CHAPTER03_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL3},
	{3,4,LEVELS_CHAPTER03_LEVEL04_LVL,LEVELS_CHAPTER03_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER3_LEVEL4},
// C4
	{4,1,LEVELS_CHAPTER04_LEVEL01_LVL,LEVELS_CHAPTER04_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL1},
	{4,2,LEVELS_CHAPTER04_LEVEL02_LVL,LEVELS_CHAPTER04_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL2},
	{4,3,LEVELS_CHAPTER04_LEVEL03_LVL,LEVELS_CHAPTER04_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL3},
	{4,4,LEVELS_CHAPTER04_LEVEL04_LVL,LEVELS_CHAPTER04_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER4_LEVEL4},
// C5
	{5,1,LEVELS_CHAPTER05_LEVEL01_LVL,LEVELS_CHAPTER05_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL1},
	{5,2,LEVELS_CHAPTER05_LEVEL02_LVL,LEVELS_CHAPTER05_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL2},
	{5,3,LEVELS_CHAPTER05_LEVEL03_LVL,LEVELS_CHAPTER05_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL3},
	{5,4,LEVELS_CHAPTER05_LEVEL04_LVL,LEVELS_CHAPTER05_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER5_LEVEL4},
// C6
	{6,1,LEVELS_CHAPTER06_LEVEL01_LVL,LEVELS_CHAPTER06_LEVEL01_TEX,	CSoundMediator::SONG_CHAPTER6_ROLLERCOASTER},
	{6,2,LEVELS_CHAPTER06_LEVEL02_LVL,LEVELS_CHAPTER06_LEVEL02_TEX,	CSoundMediator::SONG_CHAPTER6_SNAILRACE},
	{6,3,LEVELS_CHAPTER06_LEVEL03_LVL,LEVELS_CHAPTER06_LEVEL03_TEX,	CSoundMediator::SONG_CHAPTER6_BIGWHEEL},
	{6,4,LEVELS_CHAPTER06_LEVEL04_LVL,LEVELS_CHAPTER06_LEVEL04_TEX,	CSoundMediator::SONG_CHAPTER6_TUNNELOFLOVE},
	{6,5,LEVELS_CHAPTER06_LEVEL05_LVL,LEVELS_CHAPTER06_LEVEL05_TEX,	CSoundMediator::SONG_CHAPTER6_TUNNELOFLOVE},
};
const	static int	LvlTableSize=sizeof(LvlTable)/sizeof(sLvlTab);

const FileEquate	loadingScreens[6]=
{
	LOADINGSCREENS_PINEAPPLE_GFX,
	LOADINGSCREENS_CULTURE_GFX,
	LOADINGSCREENS_PICKLES_GFX,
	LOADINGSCREENS_MONITOR_GFX,
	LOADINGSCREENS_KARATE_GFX,
	LOADINGSCREENS_PIZZA_GFX,
};

/*****************************************************************************/
DVECTOR CLevel::MapPos;
DVECTOR	CLevel::s_playerSpawnPos;

/*****************************************************************************/
CLevel::CLevel()
{
}

/*****************************************************************************/
int		CLevel::GetNextLevel(int Lvl)
{
		Lvl++;
// TMP
		if (Lvl>=LvlTableSize) Lvl=0;
// Skip Blanks
		while (CFileIO::getFileSize(LvlTable[Lvl ].LevelFilename)<=4000)	// Dodgy blank level skip
		{
			Lvl++;
			if (Lvl>=LvlTableSize) Lvl=0;
		}
// End TMP
		return(Lvl);
}

/*****************************************************************************/
void 	CLevel::init(int LevelNo)
{
// Load it
sLvlTab *lvlTab=&LvlTable[LevelNo];

		CActorGfx::ResetCache();
		CSoundMediator::setSong((CSoundMediator::SONGID)lvlTab->songId);

		DisplayLoadingScreen(lvlTab);

		LevelHdr=(sLevelHdr*)CFileIO::loadFile(lvlTab->LevelFilename,"Level");
		LevelHdr->TileBank2d=(sTile2d*)	MakePtr(LevelHdr,(int)LevelHdr->TileBank2d);
		LevelHdr->TileBank3d=(sTile3d*)	MakePtr(LevelHdr,(int)LevelHdr->TileBank3d);
		LevelHdr->TriList=(sTri*)		MakePtr(LevelHdr,(int)LevelHdr->TriList);
		LevelHdr->QuadList=(sQuad*)		MakePtr(LevelHdr,(int)LevelHdr->QuadList);
		LevelHdr->VtxList=(sVtx*)		MakePtr(LevelHdr,(int)LevelHdr->VtxList);

		printf("ActorList %i\n",(int)LevelHdr->ActorList);
		printf("ItemList %i\n",(int)LevelHdr->ItemList);
		printf("Platfrom List %i\n",(int)LevelHdr->PlatformList);
		printf("TriggerList %i\n",(int)LevelHdr->TriggerList);
		printf("FXList %i\n",(int)LevelHdr->FXList);

		m_levelTPage=TPLoadTex(lvlTab->TexFilename);

		s_playerSpawnPos.vx=LevelHdr->PlayerStartX*16;
		s_playerSpawnPos.vy=LevelHdr->PlayerStartY*16+16;

		MapPos.vx=MapPos.vy=0;
		initLayers();
}

/*****************************************************************************/
void	CLevel::DisplayLoadingScreen(sLvlTab *lvlTab)
{
FontBank	font;
char		buf[128];
u8			*s_image;
int			i;

			font.initialise(&standardFont);
			font.setJustification(FontBank::JUST_CENTRE);
			sprintf(buf,"LOADING CHAPTER %d LEVEL %d",lvlTab->Chapter,lvlTab->Level);
			s_image=CFileIO::loadFile(loadingScreens[lvlTab->Chapter-1]);
			ASSERT(s_image);
			SetScreenImage(s_image);
			for(i=0;i<2;i++)
			{
				font.setColour(255,255,255);
				font.print(256  ,120  ,buf);
				font.setColour(0,0,0);
				font.print(256-1,120-1,buf);
				font.print(256-1,120  ,buf);
				font.print(256-1,120+1,buf);
				font.print(256  ,120-1,buf);
				font.print(256  ,120+1,buf);
				font.print(256+1,120-1,buf);
				font.print(256+1,120  ,buf);
				font.print(256+1,120+1,buf);
				PrimDisplay();
				VSync(0);
				VidSwapDraw();
			}
			ClearScreenImage();
			MemFree(s_image);
			VSync(20);
			font.dump();
}

/*****************************************************************************/
void	CLevel::initLayers()
{
// Init Layers
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			TileLayers[i]=0;
		}
		CollisionLayer=0;
		ActorList=0;
		ItemList=0;
		PlatformList=0;
		FXList=0;

// Back
		if (LevelHdr->BackLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->BackLayer);
			CLayerTile	*NewLayer=new ("Back Layer") CLayerBack(LevelHdr,Layer);
			NewLayer->init(MapPos,3);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_BACK]=NewLayer;
		}

// Mid
		if (LevelHdr->MidLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->MidLayer);
			CLayerTile *NewLayer=new ("Mid Layer") CLayerTile(LevelHdr,Layer);
			NewLayer->init(MapPos,2);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_MID]=NewLayer;
		}

// Action
		if (LevelHdr->ActionLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
			CLayerTile *NewLayer=new ("Action Layer") CLayerTile3d(LevelHdr,Layer);
			NewLayer->init(MapPos,0);
			TileLayers[CLayerTile::LAYER_TILE_TYPE_ACTION]=NewLayer;
		}
// Collision
		if (LevelHdr->CollisionLayer) 
		{
			sLayerHdr	*Layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->CollisionLayer);
			CollisionLayer=new ("Collision Layer") CLayerCollision(Layer);
		}

// Actors
		if (LevelHdr->ActorList)
		{
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->ActorList);
			ActorCount=Hdr->Count;
			ActorList=(sThingActor**)MemAlloc(ActorCount*sizeof(sThingActor**),"Actor List");
			u8	*ThingPtr=(u8*)MakePtr(Hdr,sizeof(sThingHdr));
			for (int i=0; i<ActorCount; i++)
			{
				ActorList[i]=(sThingActor*)ThingPtr;
				ThingPtr+=sizeof(sThingActor);
				ThingPtr+=ActorList[i]->PointCount*sizeof(u16)*2;
			}
		}
// Items
		if (LevelHdr->ItemList)
		{
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->ItemList);
			ItemCount=Hdr->Count;
			ItemList=(sThingItem*)MakePtr(Hdr,sizeof(sThingHdr));
			DVECTOR	pos;
			for(int i=0;i<ItemCount;i++)
			{
				pos.vx=ItemList->Pos.X<<4;
				pos.vy=ItemList->Pos.Y<<4;
				createPickup((PICKUP_TYPE)ItemList->Type,&pos);
				ItemList++;
			}
		}
// Platforms
		if (LevelHdr->PlatformList)
		{
			
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->PlatformList);
			PlatformCount=Hdr->Count;
			PlatformList=(sThingPlatform**)MemAlloc(PlatformCount*sizeof(sThingPlatform**),"Platform List");
			u8	*ThingPtr=(u8*)MakePtr(Hdr,sizeof(sThingHdr));
			for (int i=0; i<PlatformCount; i++)
			{
				PlatformList[i]=(sThingPlatform*)ThingPtr;
				ThingPtr+=sizeof(sThingPlatform);
				ThingPtr+=PlatformList[i]->PointCount*sizeof(u16)*2;
			}
		}

// Triggers
		if (LevelHdr->TriggerList)
		{
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->TriggerList);
			TriggerCount=Hdr->Count;
			TriggerList=(sThingTrigger*)MakePtr(Hdr,sizeof(sThingHdr));

PAUL_DBGMSG("%d triggers",TriggerCount);
			for(int i=0;i<TriggerCount;i++)
			{
				CTriggerThing	*trigger=NULL;		// I hate having to do this just to keep the compiler quiet :/ (pkg)
				switch(TriggerList->Type)
				{
					// Exit trigger
					case 0:
						trigger=(CTriggerThing*)new ("LevelExitTrigger") CLevelExitTrigger();
						break;

					// Look down trigger
					case 1:
						trigger=(CTriggerThing*)new ("LookTrigger") CLookTrigger();
						break;
				}
				trigger->init();
				trigger->setPositionAndSize(TriggerList->Pos.X<<4,TriggerList->Pos.Y<<4,
											TriggerList->Width<<4,TriggerList->Height<<4);
				TriggerList++;
			}
		}
// FX
		if (LevelHdr->FXList)
		{
			sThingHdr	*Hdr=(sThingHdr*)MakePtr(LevelHdr,LevelHdr->FXList);
			FXCount=Hdr->Count;
			FXList=(sThingFX*)MakePtr(Hdr,sizeof(sThingHdr));
		}

}

/*****************************************************************************/
void	CLevel::shutdown()
{
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i])
			{
				TileLayers[i]->shutdown();
				delete TileLayers[i];
			}
		}

		if (CollisionLayer)
		{
			CollisionLayer->shutdown();	
			delete CollisionLayer;
		}

		CSoundMediator::stopAllSound();
		VSync(0);CSoundMediator::think(1);		// This is needed to let xmplay kill off the sounds properly (pkg)

		if (ActorList) MemFree(ActorList);
		if (PlatformList) MemFree(PlatformList);

		MemFree(LevelHdr);
		CActorPool::DumpActors();
		TPFree(m_levelTPage);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void 	CLevel::render()
{
// Setup dummy prim to ensure OtPos 0 is initialised (for fast add)
/*
		for (int i=0;i<8; i++)
		{
		TILE_16	*Prim=GetPrimTILE16();
		sOT		*ThisOT=OtPtr+LayerOT+i-4;
				Prim->x0=1024;
				Prim->y0=1024;
				AddPrim(ThisOT,Prim);
				ASSERT(ThisOT->FirstPrim);
		}
*/
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i]) TileLayers[i]->render();
		}
#ifdef __SHOW_COLLISION__
		if(CollisionLayer)
		{
			CollisionLayer->render(MapPos);
		}
#endif
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLevel::think(int _frames)
{
		for (int i=0; i<CLayerTile::LAYER_TILE_TYPE_MAX; i++)
		{
			if (TileLayers[i]) TileLayers[i]->think(MapPos);
		}
}



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
DVECTOR	CLevel::getMapSize()
{
DVECTOR		size;
sLayerHdr	*layer;

		layer=(sLayerHdr*)MakePtr(LevelHdr,LevelHdr->ActionLayer);
		size.vx=layer->Width;
		size.vy=layer->Height;

		return size;
}


/*****************************************************************************/
