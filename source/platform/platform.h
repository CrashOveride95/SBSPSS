/*=========================================================================

	platform.h

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PLATFORM_PLATFORM_H__
#define	__PLATFORM_PLATFORM_H__

//#include <dstructs.h>

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#include "Gfx/Actor.h"

#ifndef __ENEMY_NPCPATH_H__
#include "enemy\npcpath.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

// temporary
#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

/*****************************************************************************/

class	CNpcPlatform : public CPlatformThing
{
public:
	enum NPC_PLATFORM_UNIT_TYPE
	{
		NPC_LINEAR_PLATFORM = 0,
		NPC_CIRCULAR_PLATFORM = 1,
		NPC_BUBBLE_PLATFORM,
		NPC_COLLAPSING_BUBBLE_PLATFORM,
		NPC_FISH_HOOK_PLATFORM,
		NPC_RETRACTING_PLATFORM,
		NPC_GEYSER_PLATFORM,
		NPC_BOBBING_PLATFORM,
		NPC_FALLING_PLATFORM,
		NPC_CART_PLATFORM,
		NPC_FISH_HOOK_2_PLATFORM,
		NPC_BRANCH_PLATFORM,
		NPC_SEESAW_PLATFORM,
		NPC_OILDRUM_PLATFORM,
		NPC_CRATE_PLATFORM,
		NPC_PLAYER_BUBBLE_PLATFORM,
		NPC_PLATFORM_TYPE_MAX,
	};

	void				init();
	void				init( DVECTOR initPos );
	void				init( DVECTOR initPos, s32 initLifetime );
	virtual void		postInit();
	void				shutdown();
	void				think(int _frames);
	void				render();
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}
	void				setType( NPC_PLATFORM_UNIT_TYPE newType )				{m_type = newType;}
	void				setTypeFromMapEdit( u16 newType );
#ifdef REMOVETHIS
//	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
	virtual s32			getNewYPos( CThing *_thisThing );
#endif
	int					getHeightFromPlatformAtPosition(int _x,int _y);
	void				setTiltable( bool isTiltable );
	void				addWaypoint( s32 xPos, s32 yPos );
	void				setGraphic( sThingPlatform *ThisPlatform );
	void				setGraphic( u8 graphicNum );
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );

	static NPC_PLATFORM_UNIT_TYPE	getTypeFromMapEdit( u16 newType );
	static CNpcPlatform	*Create(sThingPlatform *ThisPlatform);

	void				setToShutdown();
	u8					isSetToShutdown()										{return( m_isShuttingDown );}

protected:
	// NPC data structure definitions //

	enum NPC_PLATFORM_LIFETIME_TYPE
	{
		NPC_PLATFORM_FINITE_LIFE = 0,
		NPC_PLATFORM_INFINITE_LIFE = 1,
		NPC_PLATFORM_FINITE_LIFE_RESPAWN,
		NPC_PLATFORM_INFINITE_LIFE_COLLAPSIBLE,
	};

	enum NPC_PLATFORM_TIMER_TYPE
	{
		NPC_PLATFORM_TIMER_NONE = 0,
		NPC_PLATFORM_TIMER_RESPAWN = 1,
	};

	enum
	{
		EXTEND_UP = true,
		EXTEND_DOWN = false,
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,
		EXTEND_CLOCKWISE = true,
		EXTEND_ANTICLOCKWISE = false,
	};

	typedef struct NPC_PLATFORM_DATA_TYPE
	{
		FileEquate						ActorType;
//		FileEquate						animData;
		u16								initAnim;
		u8								speed;
		u16								turnSpeed;
		bool							detectCollision;
		DAMAGE_TYPE						damageToUserType;
		u16								initHealth;
		s32								lifetime;
		NPC_PLATFORM_LIFETIME_TYPE		lifetimeType;
		s32								initTimer;
		u8								initTimerType;
	}
	NPC_PLATFORM_DATA;

	// functions

	void				reinit();
	bool				processSensor();
	virtual void		processMovement( int _frames );
	virtual void		processLifetime( int _frames );
	void				processShot();
	void				processClose( int _frames );
	void				processCollision();
	virtual void		processTimer( int _frames );
	void				processTilt( int _frames );
	bool				isCollisionWithGround();

	// data

	static NPC_PLATFORM_DATA		m_data[NPC_PLATFORM_TYPE_MAX];

	static s32			playerXDist;
	static s32			playerYDist;

	static s32			playerXDistSqr;
	static s32			playerYDistSqr;

	static class CLayerCollision	*m_layerCollision;

	// internal variables

	NPC_PLATFORM_UNIT_TYPE		m_type;
	CNpcPath					m_npcPath;
	s32							m_heading;
	s32							m_velocity;
	s16							m_rotation;
	DVECTOR						m_base;
	DVECTOR						m_initPos;
	u8							m_state;
	bool						m_animPlaying;
	bool						m_reversed;
	s32							m_extension;
	s32							m_lifetime;
	s32							m_initLifetime;
	NPC_PLATFORM_LIFETIME_TYPE	m_lifetimeType;
	bool						m_contact;
	s32							m_timer;
	bool						m_isActive;
	u8							m_timerType;
	bool						m_detectCollision;
	bool						m_tiltable;
	s32							m_tiltAngle;
	s32							m_tiltVelocity;
	bool						m_extendDir;

	int							m_frame;
	int							m_animNo;
	CModelGfx					*m_modelGfx;
	u8							m_isShuttingDown;

	virtual void		collidedWith(CThing *_thisThing);

	static NPC_PLATFORM_UNIT_TYPE mapEditConvertTable[NPC_PLATFORM_TYPE_MAX];



protected:
	virtual void	setCollisionAngle(int newAngle);	// Actually.. this probly doesn't need to be in the base calss anymore.. :/
	void			calculateBoundingBoxSize();


};


/*****************************************************************************/
#endif