/*******************/
/*** Level Class ***/
/*******************/

#ifndef __LEVEL_LEVEL_H__
#define __LEVEL_LEVEL_H__

#include "system\global.h"
#include "level/layertile.h"
#include "level/layercollision.h"

/*****************************************************************************/
// Woo.. this is getting big now isn't it?
struct	sLvlTab
{
	FileEquate				LevelInfo;
	FileEquate				TileBank,Level,Tex;
	int						exitX,exitY,exitW,exitH;
	int						spawnX,spawnY;
	int						Spr0,Spr1;
	int						songId;
};

/*****************************************************************************/
class CLayer;
class CLevel
{
public:
	CLevel();
// Scene Handlers
	void		init();
	void		shutdown(bool CleanUp);
	void		render();
	void		think(int _frames);

	void					setCameraCentre(DVECTOR _pos)		{MapPos=_pos;}
	static DVECTOR			getCameraPos()						{return MapPos;}
	static DVECTOR			getPlayerSpawnPos()					{return s_playerSpawnPos;}
	int						getActorCount()						{return ActorCount;}
	sThingActor				**getActorList()					{return ActorList;}
	CLayerCollision			*getCollisionLayer()				{return CollisionLayer;}
	DVECTOR					getMapSize();

	int						GetNextLevel(int Lvl);
private:
	void		initLayers();
	void		initNewLevel(sLvlTab *LevelDat);
	void		DisplayLoadingScreen();

	void		DoPaulsTempCrap(sLvlTab *LevelDat);

	sLevelInfo		*LevelInfo;
	u8				*PakBuffer,*LevelBuffer;

	sTile			*TileBank;
	static DVECTOR	MapPos;
	static DVECTOR	s_playerSpawnPos;

	TPAGE_DESC		m_levelTPage;

// Tile Layers
	CLayerTile		*TileLayers[CLayerTile::LAYER_TILE_TYPE_MAX];

// Collision
	CLayerCollision	*CollisionLayer;

// Things
	int				ActorCount;
	sThingActor		**ActorList;
	int				ItemCount;
	sThingItem		*ItemList;
	int				PlatformCount;
	sThingPlatform	**PlatformList;

};

/*****************************************************************************/

#endif