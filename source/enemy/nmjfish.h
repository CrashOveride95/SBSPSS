/*=========================================================================

	nmjfish.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NMJFISH_H__
#define	__ENEMY_NMJFISH_H__

#ifndef __ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif

#include "fx\fx.h"
#include "fx\fxjfish.h"

#define MJ_PINK ( 255 + ( 128 << 8 ) + ( 255 << 16 ) )
#define MJ_WHITE ( 255 + ( 255 << 8 ) + ( 255 << 16 ) )

class CNpcMotherJellyfishEnemy : public CNpcBossEnemy
{
public:
	void				postInit();
	void				render();
	void				shutdown();
	virtual void		setupWaypoints( sThingActor *ThisActor );
protected:
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	virtual void		processShot( int _frames );
	virtual void		collidedWith(CThing *_thisThing);
	//void				spawnJellyfish( int _frames );
	//virtual void		processUserCollision( CThing *thisThing );

	enum NPC_MOTHER_JELLYFISH_STATE
	{
		/*MOTHER_JELLYFISH_RETURN_TO_START_1 = 0,
		MOTHER_JELLYFISH_CYCLE_1 = 1,	
		MOTHER_JELLYFISH_ATTACK_PLAYER_SPAWN_1,
		MOTHER_JELLYFISH_RETURN_TO_START_2,
		MOTHER_JELLYFISH_CYCLE_2,
		MOTHER_JELLYFISH_ATTACK_PLAYER_SPAWN_2,
		MOTHER_JELLYFISH_RETURN_TO_START_3,
		MOTHER_JELLYFISH_CYCLE_3,
		MOTHER_JELLYFISH_ATTACK_PLAYER_SHOCK,*/
		MOTHER_JELLYFISH_CYCLE = 0,
		MOTHER_JELLYFISH_BEGIN_CIRCLE = 1,
		MOTHER_JELLYFISH_CIRCLE,
		MOTHER_JELLYFISH_EXIT,
		MOTHER_JELLYFISH_STRAFE_START,
		MOTHER_JELLYFISH_STRAFE,
	};

	int					m_jellyfishCount;
	s32					m_spawnTimer;
	s32					m_pulsateTimer;
	s32					m_pauseTimer;
	s32					m_maxPauseTimer;
	s32					m_cycleWidth;
	s32					m_halfCycleWidth;
	s16					m_renderScale;
	s32					m_invulnerableTimer;
	u8					m_attackCounter;

	CFXJellyFishLegs	*legs[4];
	DVECTOR				legsPos[4];

	DVECTOR				targetPos;
};

#endif