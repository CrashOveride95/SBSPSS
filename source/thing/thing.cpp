/*=========================================================================

	thing.cpp

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#define	USE_FREE_LIST
/*----------------------------------------------------------------------
	Includes
	-------- */

#include "thing\thing.h"

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#include "level\level.h"

// Needed for freelist table :o(
#include	"pickups\pickup.h"
#include	"platform\platform.h"
#include	"projectl\projectl.h"
#include	"enemy\npc.h"
#include	"friend\friend.h"
#include	"fx\fx.h"

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef __HAZARD_HRWEIGHT_H__
#include "hazard\hrweight.h"
#endif

#ifndef __HAZARD_HRWHEEL_H__
#include "hazard\hrwheel.h"
#endif

#ifndef __HAZARD_HPSWITCH_H__
#include "hazard\hpswitch.h"
#endif

/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static const int	s_RenderBBoxX0=0;
static const int	s_RenderBBoxX1=512;
static const int	s_RenderBBoxY0=0;
static const int	s_RenderBBoxY1=256;
static const int	s_ThinkBBoxX0=0-256;
static const int	s_ThinkBBoxX1=512+526;
static const int	s_ThinkBBoxY0=0-128;
static const int	s_ThinkBBoxY1=256+128;

CThing			*CThingManager::s_thingLists[CThing::MAX_TYPE];
CThing			*CThingManager::s_CollisionLists[CThing::MAX_TYPE];
int				CThingManager::s_initialised=false;
sBBox			CThingManager::m_RenderBBox;
sBBox			CThingManager::m_ThinkBBox;

#ifdef	USE_FREE_LIST
CThing			**CThingManager::s_FreeList[CThing::MAX_TYPE];
CThing			*DuffList;
int				FreeListCount=0;
int				DuffListCount=0;


struct	sFreeListTable
{
	u16	Type;
	u16	Count;
};

static const sFreeListTable	FreeListTable[]=
{
/* 0*/	{CThing::TYPE_PICKUP			,CBasePickup::MAX_SUBTYPE},
/* 1*/	{CThing::TYPE_PLATFORM			,CNpcPlatform::MAX_SUBTYPE},
/* 2*/	{CThing::TYPE_PLAYER			,CPlayerThing::MAX_SUBTYPE},
/* 3*/	{CThing::TYPE_PLAYERPROJECTILE	,CPlayerProjectile::MAX_SUBTYPE},
/* 4*/	{CThing::TYPE_NPC				,CNpcFriend::MAX_SUBTYPE},
/* 5*/	{CThing::TYPE_ENEMY				,CNpcEnemy::MAX_SUBTYPE},
/* 6*/	{CThing::TYPE_ENEMYPROJECTILE	,CProjectile::MAX_SUBTYPE},
/* 7*/	{CThing::TYPE_TRIGGER			,CTriggerThing::MAX_SUBTYPE},
/* 8*/	{CThing::TYPE_HAZARD			,CNpcHazard::MAX_SUBTYPE},
/* 9*/	{CThing::TYPE_FX				,CFX::MAX_SUBTYPE},
};
static const	int	FreeListTableSize=sizeof(FreeListTable)/sizeof(sFreeListTable);
#endif
/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::init()
{
	ASSERT(!s_initialised);
	initList(s_thingLists);
	initList(s_CollisionLists);
	initFreeList();
	s_initialised=true;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::shutdown()
{
	int		i;
	CThing	*thing;

	ASSERT(s_initialised);
	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		while(s_thingLists[i])
		{
			thing=s_thingLists[i];
			thing->shutdown();
			DeleteThing(thing);
		}
	}
	s_initialised=false;
	shutdownFreeList();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::initList(CThing **List)
{
int		i;

		for(i=0 ;i<CThing::MAX_TYPE; i++)
		{
			List[i]=NULL;
		}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::killAllThingsForRespawn()
{
	int		i;

	ASSERT(s_initialised);
	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		// Hey - it's not optimal in speed, but it's vaguely funny :)
		// ( and anyway.. it probly *is* optimal in size.. )
		CThing	*thing;
		thing=s_thingLists[i];
		while(thing)
		{
			if(thing->dontKillDuringLevelRespawn())
			{
				thing=thing->m_nextListThing;
			}
			else
			{
				thing->shutdown();
				DeleteThing(thing);
				thing=s_thingLists[i];
			}
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::initAllThings()
{
	for(int	i=0; i<CThing::MAX_TYPE; i++)
	{
		CThing	*thing=s_thingLists[i];
		while(thing)
		{
			thing->updateCollisionArea();
			thing=thing->m_nextListThing;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void		CThingManager::matchPressureSwitches()
{
	CNpcHazard *hazard;

	hazard = (CNpcHazard *) s_thingLists[CThing::TYPE_HAZARD];

	while( hazard )
	{
		if ( hazard->getType() == CNpcHazard::NPC_PRESSURE_SWITCH_HAZARD )
		{
			CNpcPressureSwitchHazard *switchHazard = (CNpcPressureSwitchHazard *) hazard;

			DVECTOR triggerPos = switchHazard->getTriggerPos();

			CNpcPlatform *platform;

			platform = (CNpcPlatform *) s_thingLists[CThing::TYPE_PLATFORM];

			while( platform )
			{
				if ( platform->getType() == CNpcPlatform::NPC_TRAPDOOR_PLATFORM )
				{
					CNpcTrapdoorPlatform *trapdoor = (CNpcTrapdoorPlatform *) platform;

					DVECTOR testPos = trapdoor->getTriggerPos();

					if ( testPos.vx == triggerPos.vx && testPos.vy == triggerPos.vy )
					{
						switchHazard->linkToPlatform( trapdoor );
					}
				}

				platform = (CNpcPlatform *) platform->m_nextListThing;
			}
		}

		hazard = (CNpcHazard *) hazard->m_nextListThing;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void		CThingManager::matchWheelsAndWeights()
{
	CNpcHazard *hazard1;
	CNpcHazard *hazard2;

	hazard1 = (CNpcHazard *) s_thingLists[CThing::TYPE_HAZARD];

	while( hazard1 )
	{
		if ( hazard1->getType() == CNpcHazard::NPC_RISING_WEIGHT_HAZARD )
		{
			CNpcRisingWeightHazard *weight = (CNpcRisingWeightHazard *) hazard1;
			DVECTOR wheelPos = weight->getWheelPos();

			hazard2 = (CNpcHazard *) s_thingLists[CThing::TYPE_HAZARD];

			while( hazard2 )
			{
				if ( hazard2->getType() == CNpcHazard::NPC_RISING_WEIGHT_WHEEL_HAZARD )
				{
					CNpcRisingWeightWheelHazard *wheel = (CNpcRisingWeightWheelHazard *) hazard2;

					DVECTOR testPos = wheel->getWheelPos();

					if ( testPos.vx == wheelPos.vx && testPos.vy == wheelPos.vy )
					{
						wheel->linkToWeight( weight );
					}
				}

				hazard2 = (CNpcHazard *) hazard2->m_nextListThing;
			}
		}

		hazard1 = (CNpcHazard *) hazard1->m_nextListThing;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void		CThingManager::thinkAllThings(int _frames)
{
// Setup Screen BBox's
DVECTOR	const	&CamPos=CLevel::getCameraPos();

	m_ThinkBBox.XMin=s_ThinkBBoxX0+CamPos.vx;
	m_ThinkBBox.XMax=s_ThinkBBoxX1+CamPos.vx;
	m_ThinkBBox.YMin=s_ThinkBBoxY0+CamPos.vy;
	m_ThinkBBox.YMax=s_ThinkBBoxY1+CamPos.vy;

	int		i;
	CThing	*thing;
	CThing	*thing1,*thing2,*playerThing;

	initList(s_CollisionLists);

	for(i=0; i<CThing::MAX_TYPE; i++)
	{
		thing=s_thingLists[i];
		while(thing)
		{
			// Check If in Thinkable range
			CRECT	const *ThingRect= thing->getThinkBBox();
			int		lastFlag=thing->getThinkFlag()<<1;
			int		Flag=1;
			// Will speed this up
			if (!thing->alwaysThink())
			{
				if (ThingRect->x2<m_ThinkBBox.XMin || ThingRect->x1>m_ThinkBBox.XMax) Flag=0;
				if (ThingRect->y2<m_ThinkBBox.YMin || ThingRect->y1>m_ThinkBBox.YMax) Flag=0;
			}
			thing->setThinkFlag(Flag);
			if (Flag)
			{
				thing->think(_frames);
//				thing->updateCollisionArea();
				if (thing->canCollide())
				{
					CThingManager::addToCollisionList(thing);
				}
			}
			Flag|=lastFlag;
			switch (Flag)
			{			// Last This
				case 0: // 0    0
					break;
				case 1: // 0    1
					thing->enterThingZone(_frames);
					break;
				case 2: // 1    0
					thing->leftThingZone(_frames);
					break;
				case 3: // 1    1
					break;
				default:
					ASSERT("Invalid Think State");
			}



/* THIS WILL NOT STAY HERE, THINGS MUST BE INITIALISED CORRECTLY */
			thing->updateCollisionArea();

			thing=thing->m_nextListThing;
		}
	}

	CPlayer *player = GameScene.getPlayer();

	if ( player )
	{
		player->clearPlatform();
	}

	playerThing=s_CollisionLists[CThing::TYPE_PLAYER];

	if (player && playerThing)
	{
		playerThing->setHasPlatformCollided( false );

		// Player -> Platform collision
		thing1=s_CollisionLists[CThing::TYPE_PLATFORM];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Pickup collision
		thing1=s_CollisionLists[CThing::TYPE_PICKUP];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Enemy collision
		thing1=s_CollisionLists[CThing::TYPE_ENEMY];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Friend collision
		thing1=s_CollisionLists[CThing::TYPE_NPC];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Hazard collision
		thing1=s_CollisionLists[CThing::TYPE_HAZARD];
		while(thing1)
		{
		if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Enemy projectile collision
		thing1=s_CollisionLists[CThing::TYPE_ENEMYPROJECTILE];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Trigger collision
		thing1=s_CollisionLists[CThing::TYPE_TRIGGER];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Enemy -> Player projectile collision
		thing1=s_CollisionLists[CThing::TYPE_PLAYERPROJECTILE];
		thing2=s_CollisionLists[CThing::TYPE_ENEMY];
		while(thing1)
		{
			while(thing2)
			{
				if(thing1->checkCollisionAgainst(thing2, _frames))
				{
					thing1->collidedWith(thing2);
				}
				thing2=thing2->m_nextCollisionThing;
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Enemy -> Enemy collision
		thing1=s_CollisionLists[CThing::TYPE_ENEMY];
		while(thing1)
		{
			thing2=thing1->m_nextCollisionThing;//s_CollisionLists[CThing::TYPE_ENEMY];

			while(thing2)
			{
				ASSERT(thing1 != thing2 );
//				if ( thing1 != thing2 )
				{
					if (thing1->checkCollisionAgainst( thing2, _frames ) )
					{
						thing1->collidedWith( thing2 );
						//thing2->collidedWith( thing1 );
					}
				}
				thing2 = thing2->m_nextCollisionThing;
			}
			thing1 = thing1->m_nextCollisionThing;
		}

		// Hazard -> Platform collision
		thing1=s_CollisionLists[CThing::TYPE_PLATFORM];
		while(thing1)
		{
			thing2=s_CollisionLists[CThing::TYPE_HAZARD];

			while(thing2)
			{
				if ( thing1 != thing2 )
				{
					if (thing1->checkCollisionAgainst( thing2, _frames ) )
					{
						thing1->collidedWith( thing2 );
						//thing2->collidedWith( thing1 );
					}
				}

				thing2 = thing2->m_nextCollisionThing;
			}

			thing1 = thing1->m_nextCollisionThing;
		}

		// Platform -> Player projectile collision
		thing1=s_CollisionLists[CThing::TYPE_PLATFORM];
		while(thing1)
		{
			thing2=s_CollisionLists[CThing::TYPE_PLAYERPROJECTILE];
			while(thing2)
			{
				if(thing1->checkCollisionAgainst(thing2, _frames))
				{
					thing1->collidedWith(thing2);
				}
				thing2=thing2->m_nextCollisionThing;
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Friendly npc -> Platform projectile collision - first clear platforms

		CNpcFriend *friendNpc = (CNpcFriend *) s_CollisionLists[CThing::TYPE_NPC];
		while( friendNpc )
		{
			friendNpc->clearPlatform();
			friendNpc = (CNpcFriend *) friendNpc->m_nextCollisionThing;
		}

		// now detect new platform

		thing1=s_CollisionLists[CThing::TYPE_PLATFORM];
		while(thing1)
		{
			thing2=s_CollisionLists[CThing::TYPE_NPC];
			while(thing2)
			{
				if(thing1->checkCollisionAgainst(thing2, _frames))
				{
					thing1->collidedWith(thing2);
				}
				thing2=thing2->m_nextCollisionThing;
			}
			thing1=thing1->m_nextCollisionThing;
		}
	}
// Shut emm down, sh sh shut em down, we shutem down
	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		thing=s_thingLists[i];
		CThing	*nextThing = thing;
		while(thing)
		{
			nextThing=thing->m_nextListThing;

			if ( thing->isSetToShutdown() )
			{
				thing->shutdown();
				DeleteThing(thing);
			}

			thing = nextThing;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::renderAllThings()
{
// Setup Screen BBox's
DVECTOR	const	&CamPos=CLevel::getCameraPos();

	m_RenderBBox.XMin=s_RenderBBoxX0+CamPos.vx;
	m_RenderBBox.XMax=s_RenderBBoxX1+CamPos.vx;
	m_RenderBBox.YMin=s_RenderBBoxY0+CamPos.vy;
	m_RenderBBox.YMax=s_RenderBBoxY1+CamPos.vy;


	for(int i=0;i<CThing::MAX_TYPE;i++)
	{
		CThing	*thing=s_thingLists[i];
		while(thing)
		{
			thing->render();
			thing=thing->m_nextListThing;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::processEventAllThings(GAME_EVENT _event,CThing *_sourceThing)
{
	int		i;
	CThing	*thing;

	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		thing=s_thingLists[i];
		while(thing)
		{
			thing->processEvent(_event,_sourceThing);
			thing=thing->m_nextListThing;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:	Searches through a list of things to check for collision against an area.
				The first time this is called, _continue should be false. If no colliding things are found then
				NULL will be returned. If a colliding thing is found then it's address gets returned. To continue
				searching through the list for the next colliding thing, call the function again with _continue set
				to true.
				NB: This function could probly cause weird bugs if not used properly! BE AWARE!
	Params:		*_area		Area to check against
				_type		Type of thing to search for
				_continue	If false then the list is searched from the start, if true then the search continues
							from the last thing that was found ( um.. see above )
	Returns:
  ---------------------------------------------------------------------- */
CThing		*CThingManager::checkCollisionAreaAgainstThings(CRECT *_area,int _type,int _continue)
{
	static CThing	*thing=NULL;

	ASSERT(_type<CThing::MAX_TYPE);

	if(_continue)
	{
		ASSERT(thing);
		thing=thing->m_nextListThing;
	}
	else
	{
		thing=s_thingLists[_type];
	}
	while(thing)
	{
		if(thing->canCollide() && thing->checkCollisionAgainstArea(_area))
		{
			return thing;
		}
		thing=thing->m_nextListThing;
	}

	return NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThingManager::addToThingList(CThing *_this)
{
int	Type=_this->getThingType();

	_this->m_nextListThing=s_thingLists[Type];
	s_thingLists[Type]=_this;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThingManager::removeFromThingList(CThing *_this)
{
	CThing	*prevThing,*thing;

	prevThing=NULL;
	thing=s_thingLists[_this->getThingType()];
	while(thing!=_this)
	{
		prevThing=thing;
		thing=thing->m_nextListThing;
		ASSERT(thing);	// Not in the list!?!?
	}
	if(prevThing)
	{
		prevThing->m_nextListThing=_this->m_nextListThing;
	}
	else
	{
		s_thingLists[_this->getThingType()]=_this->m_nextListThing;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThingManager::addToCollisionList(CThing *thing)
{
int	Type=thing->getThingType();

		thing->m_nextCollisionThing=s_CollisionLists[Type];
		s_CollisionLists[Type]=thing;
}

/********************************************************************/
/********************************************************************/
/*** Free List Stuff ************************************************/
/********************************************************************/
/********************************************************************/
void		CThingManager::initFreeList()
{
#ifdef	USE_FREE_LIST
// Make sure no-one is being naughty
		ASSERT(FreeListTableSize==CThing::MAX_TYPE)

		for (int i=0; i<FreeListTableSize; i++)
		{
			sFreeListTable	const &ThisType=FreeListTable[i];
			int	Count=ThisType.Count;
			CThing	**List=(CThing**)MemAlloc(Count*sizeof(CThing**),"ThingCache");
			for (int t=0; t<Count; t++)
			{
				List[t]=0;

			}
			s_FreeList[ThisType.Type]=List;

		}
		DuffList=0;
#endif
}

/********************************************************************/
void	CThingManager::resetFreeList()
{
#ifdef	USE_FREE_LIST
		for (int i=0; i<FreeListTableSize; i++)
		{
			sFreeListTable	const &ThisType=FreeListTable[i];
			int	Count=ThisType.Count;

			CThing	**List=s_FreeList[i];
			for (int t=0; t<Count; t++)
			{
				CThing	*ThisThing=List[t];
				while (ThisThing)
				{
					CThing	*Next=ThisThing->NextFreeThing;
					delete ThisThing;
					FreeListCount--;
					ThisThing=Next;
				}
				List[t]=0;
			}
		}
		CThing	*Duff=DuffList;
		while (Duff)
		{
			CThing	*next=Duff->NextFreeThing;
			delete Duff;
			DuffListCount--;
			Duff=next;
		}
#endif
}

/********************************************************************/
void	CThingManager::shutdownFreeList()
{
#ifdef	USE_FREE_LIST
		resetFreeList();
		for (int i=0; i<FreeListTableSize; i++)
		{
			sFreeListTable	const &ThisType=FreeListTable[i];
			MemFree(s_FreeList[ThisType.Type]);
		}
#endif
}

/********************************************************************/
CThing	*CThingManager::GetThing(int Type,int SubType)
{
#ifdef	USE_FREE_LIST
CThing	**List=s_FreeList[Type];
CThing	*Thing=List[SubType];

		if (Thing)
		{
			List[SubType]=Thing->NextFreeThing;
			Thing->initDef();
			Thing->NextFreeThing=0;
			FreeListCount--;
		}

		return(Thing);
#else
		return(0);
#endif
}

/********************************************************************/
void	CThingManager::DeleteThing(CThing *Thing)
{
#ifdef	USE_FREE_LIST
int		Type=Thing->getThingType();
int		SubType=Thing->getThingSubType();
CThing	**List=s_FreeList[Type];

// Check its been aquired/set correctly
// Temp workaround
		ASSERT(SubType!=1234);
		if (SubType!=1234)
		{
			Thing->NextFreeThing=List[SubType];
			List[SubType]=Thing;
			FreeListCount++;
		}
		else
		{
//			delete Thing;
			Thing->NextFreeThing=DuffList;
			DuffList=Thing;
			DuffListCount++;
		}

#else
		delete Thing;
#endif
}

/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::init()
{
	ParentThing=NULL;
	NextThing=NULL;
	m_numChildren = 0;

	Pos.vx=Pos.vy=10;
// These need to stay for init
	setCollisionSize(20,20);	// Some temporary defaults.. (pkg)
	setCollisionCentreOffset(0,0);

// Add to thing list
	CThingManager::addToThingList(this);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::shutdown()
{
	
	if (ParentThing)
	{ // Is child
		ParentThing->removeChild(this);
	}
	else
	{ // Is Parent
		removeAllChild();
	}

	// Remove from thing list
	CThingManager::removeFromThingList(this);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::think(int _frames)
{
	PosDelta.vx=Pos.vx-PosLast.vx;
	PosDelta.vy=Pos.vy-PosLast.vy;
	PosLast=Pos;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#if defined (__USER_paul__) || defined (__USER_charles__)
#define	SHOW_BBOX	1
int showthings=true;
#endif

#if defined (__USER_daveo__)
#define	SHOW_BBOX	1
int showthings=false;
#endif

void	CThing::render()
{
// Check Is Onscreen
CRECT	const	*ThingRect= getRenderBBox();
sBBox			&ScrBBox=CThingManager::getRenderBBox();
DVECTOR	const	&CamPos=CLevel::getCameraPos();

		m_RenderPos.vx = Pos.vx - CamPos.vx;
		m_RenderPos.vy = Pos.vy - CamPos.vy;

// Will speed this up
		m_renderFlag=true;
		if (ThingRect->x2<ScrBBox.XMin || ThingRect->x1>ScrBBox.XMax) m_renderFlag=false;
		if (ThingRect->y2<ScrBBox.YMin || ThingRect->y1>ScrBBox.YMax) m_renderFlag=false;

/***/
#ifdef	SHOW_BBOX
	if(showthings) ShowBBox();
#endif

}
/****************************************************************************************/
#ifdef	SHOW_BBOX
#include "gfx\prim.h"

void	CThing::ShowBBox()
{
	if(showthings)
	{
		DVECTOR	const	&ofs=CLevel::getCameraPos();
		CRECT	area;

		area=getCollisionArea();
		area.x1-=ofs.vx;
		area.y1-=ofs.vy;
		area.x2-=ofs.vx;
		area.y2-=ofs.vy;

		if(area.x1<=511&&area.x2>=0 && area.y1<=255&&area.y2>=0)
		{
			DrawLine(area.x1,area.y1,area.x2,area.y1,255,255,255,0);
			DrawLine(area.x2,area.y1,area.x2,area.y2,255,255,255,0);
			DrawLine(area.x2,area.y2,area.x1,area.y2,255,255,255,0);
			DrawLine(area.x1,area.y2,area.x1,area.y1,255,255,255,0);

			area.x1=Pos.vx-10-ofs.vx;
			area.y1=Pos.vy-10-ofs.vy;
			area.x2=Pos.vx+10-ofs.vx;
			area.y2=Pos.vy+10-ofs.vy;
			DrawLine(area.x1,area.y1,area.x2,area.y2,255,0,0,0);
			DrawLine(area.x2,area.y1,area.x1,area.y2,255,0,0,0);
		}
/*
		DVECTOR	ofs;
		CRECT	area;

		ofs=CLevel::getCameraPos();

		area=getCollisionArea();
		area.x1-=ofs.vx;
		area.y1-=ofs.vy;
		area.x2-=ofs.vx;
		area.y2-=ofs.vy;

		if(area.x1<=511&&area.x2>=0&&
		   area.y1<=255&&area.y2>=0)
		{
			area=getCollisionArea();

			SVECTOR points[4];
			VECTOR vecPoints[4];

			points[0].vx = area.x1 - Pos.vx;
			points[0].vy = area.y1 - Pos.vy;

			points[1].vx = area.x1 - Pos.vx;
			points[1].vy = area.y2 - Pos.vy;

			points[2].vx = area.x2 - Pos.vx;
			points[2].vy = area.y2 - Pos.vy;

			points[3].vx = area.x2 - Pos.vx;
			points[3].vy = area.y1 - Pos.vy;

			MATRIX mtx;

			SetIdentNoTrans(&mtx );
			RotMatrixZ( m_collisionAngle, &mtx );

			ApplyMatrix( &mtx, &points[0], &vecPoints[0] );
			ApplyMatrix( &mtx, &points[1], &vecPoints[1] );
			ApplyMatrix( &mtx, &points[2], &vecPoints[2] );
			ApplyMatrix( &mtx, &points[3], &vecPoints[3] );

			vecPoints[0].vx += Pos.vx - ofs.vx;
			vecPoints[0].vy += Pos.vy - ofs.vy;

			vecPoints[1].vx += Pos.vx - ofs.vx;
			vecPoints[1].vy += Pos.vy - ofs.vy;

			vecPoints[2].vx += Pos.vx - ofs.vx;
			vecPoints[2].vy += Pos.vy - ofs.vy;

			vecPoints[3].vx += Pos.vx - ofs.vx;
			vecPoints[3].vy += Pos.vy - ofs.vy;

			//DrawLine(area.x1,area.y1,area.x1,area.y2,255,255,255,0);
			//DrawLine(area.x1,area.y2,area.x2,area.y2,255,255,255,0);
			//DrawLine(area.x2,area.y2,area.x2,area.y1,255,255,255,0);
			//DrawLine(area.x2,area.y1,area.x1,area.y1,255,255,255,0);

			DrawLine( vecPoints[0].vx, vecPoints[0].vy, vecPoints[1].vx, vecPoints[1].vy,255,255,255,0);
			DrawLine( vecPoints[1].vx, vecPoints[1].vy, vecPoints[2].vx, vecPoints[2].vy,255,255,255,0);
			DrawLine( vecPoints[2].vx, vecPoints[2].vy, vecPoints[3].vx, vecPoints[3].vy,255,255,255,0);
			DrawLine( vecPoints[3].vx, vecPoints[3].vy, vecPoints[0].vx, vecPoints[0].vy,255,255,255,0);

			area.x1=Pos.vx-10-ofs.vx;
			area.y1=Pos.vy-10-ofs.vy;
			area.x2=Pos.vx+10-ofs.vx;
			area.y2=Pos.vy+10-ofs.vy;
			DrawLine(area.x1,area.y1,area.x2,area.y2,255,0,0,0);
			DrawLine(area.x2,area.y1,area.x1,area.y2,255,0,0,0);
		}
*/
	}
}
#endif

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::addChild(CThing *Child)
{
CThing	*List=NextThing;

	if ( List )
	{
// Find end of list
		while (List->NextThing)
		{
			List=List->NextThing;
		}
		List->NextThing=Child;
		Child->ParentThing=this;
		m_numChildren++;
	}
	else
	{
// List does not exist, create
		NextThing = Child;
		Child->ParentThing=this;
		m_numChildren++;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::removeChild(CThing *Child)
{
CThing	*List=NextThing;
CThing	*Last=NULL;

// Find Child
		while ( List != Child && List->NextThing )
		{
			Last = List;
			List = List->NextThing;
		}

// if there are no more links to continue down, the current 'List' must either be the correct item, or the item does not
// exist in the list at all

		ASSERT( List == Child );

		if ( Last )
		{
			Last->NextThing = List->NextThing;
		}
		else
		{
			this->NextThing = List->NextThing;
		}

		m_numChildren--;

		Child->ParentThing=NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::removeAllChild()
{
CThing	*List=NextThing;

		while (List)
		{
			CThing	*NextThing=List->NextThing;
			List->ParentThing=NULL;
			List->NextThing=NULL;
			List=NextThing;
		}
		NextThing=NULL;

		m_numChildren = 0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::deleteAllChild()
{
CThing	*List=NextThing;

		while (List)
		{
			CThing	*NextThing=List->NextThing;
			List->ParentThing=NULL;
			List->NextThing=NULL;
			List->shutdown();
			CThingManager::DeleteThing(List);
			List=NextThing;
		}
		NextThing=NULL;

		m_numChildren = 0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CThing::getNumChildren()
{
	return( m_numChildren );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool	CThing::hasChild(CThing *Child)
{
CThing *nextChild = NextThing;

	while( nextChild )
	{
		if ( nextChild == Child )
		{
			return( true );
		}

		nextChild = nextChild->NextThing;
	}

	return( false );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::setCollisionSize(int _w,int _h)
{
	m_collisionSize.vx=_w;
	m_collisionSize.vy=_h;
	if(m_collisionSize.vx>m_collisionSize.vy)
	{
		m_collisionRadius=m_collisionSize.vx;
	}
	else
	{
		m_collisionRadius=m_collisionSize.vy;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::updateCollisionArea()
{
	m_collisionCentre.vx=Pos.vx+m_collisionCentreOffset.vx;
	m_collisionCentre.vy=Pos.vy+m_collisionCentreOffset.vy;
	m_collisionArea.x1=m_collisionCentre.vx-(m_collisionSize.vx/2);
	m_collisionArea.x2=m_collisionArea.x1+m_collisionSize.vx;
	m_collisionArea.y1=m_collisionCentre.vy-(m_collisionSize.vy/2);
	m_collisionArea.y2=m_collisionArea.y1+m_collisionSize.vy;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

s32		CThing::getNewYPos(CThing *_thisThing)
{
	CRECT	thisRect;
	thisRect = getCollisionArea();

	if ( thisRect.y1 < thisRect.y2 )
	{
		return( thisRect.y1 );
	}
	else
	{
		return( thisRect.y2 );
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CThing::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

	pos=getCollisionCentre();
	thisThingPos=_thisThing->getCollisionCentre();

	radius=getCollisionRadius()+_thisThing->getCollisionRadius();
	collided=false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
		CRECT	thisRect,thatRect;

		thisRect=getCollisionArea();

		thatRect=_thisThing->getCollisionArea();

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			collided=true;
		}
	}

	return collided;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CThing::checkCollisionAgainstArea(CRECT *_rect)
{
	CRECT	thisRect;
	int		ret;

	thisRect=getCollisionArea();
	ret=false;

	if(((thisRect.x1>=_rect->x1&&thisRect.x1<=_rect->x2)||(thisRect.x2>=_rect->x1&&thisRect.x2<=_rect->x2)||(thisRect.x1<=_rect->x1&&thisRect.x2>=_rect->x2))&&
	   ((thisRect.y1>=_rect->y1&&thisRect.y1<=_rect->y2)||(thisRect.y2>=_rect->y1&&thisRect.y2<=_rect->y2)||(thisRect.y1<=_rect->y1&&thisRect.y2>=_rect->y2)))
	{
		ret=true;
	}

	return ret;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::processEvent(GAME_EVENT _event,CThing *_sourceThing)
{
	// do nothing by default - ignore event
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTriggerThing::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	Pos.vx=_x+(_w/2);
	Pos.vy=_y+(_h/2);
	setCollisionSize(_w,_h);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTriggerThing::setTargetBox(int _x,int _y,int _w,int _h)
{
	m_boxX1=_x;
	m_boxY1=_y;
	m_boxX2=_x+_w;
	m_boxY2=_y+_h;
}


