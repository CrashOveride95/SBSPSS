/*=========================================================================

	player.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PLAYER_H__
#define __PLAYER_PLAYER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GAME_THING_H__
#include "Game/Thing.h"
#endif

#ifndef __SKEL_HEADER__
#include "Gfx/Skel.h"
#endif

#ifndef __DATA_STRUCTS_HEADER__
#include <dstructs.h>
#endif

#ifndef	__PLAYER_PSTATES_H__
#include "player\pstates.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef enum
{
	STATE_IDLE,
	STATE_JUMP,
	STATE_RUN,
	STATE_FALL,
	STATE_FALLFAR,
	STATE_BUTTBOUNCE,
	STATE_BUTTFALL,
	STATE_BUTTLAND,
	STATE_ATTACK,
	STATE_RUNATTACK,
	STATE_AIRATTACK,
	STATE_DUCK,
	STATE_SOAKUP,
	STATE_GETUP,
		
	NUM_STATES,
}PLAYER_STATE;

typedef enum
{
	PLAYER_MODE_BASICUNARMED,
	PLAYER_MODE_FULLUNARMED,
	PLAYER_MODE_CORALBLOWER,

	NUM_PLAYERMODES,
}PLAYER_MODE;

enum
{
	FACING_LEFT=+1,
	FACING_RIGHT=-1,
};

typedef enum
{
	PM__JUMP_VELOCITY,
	PM__MAX_JUMP_FRAMES,
	PM__MAX_SAFE_FALL_FRAMES,
	PM__MAX_RUN_VELOCITY,
	PM__RUN_SPEEDUP,
	PM__RUN_REVERSESLOWDOWN,
	PM__RUN_SLOWDOWN,
		
	NUM_PLAYER_METRICS
}PLAYER_METRIC;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

struct PlayerMetrics
{
	s16		m_metric[NUM_PLAYER_METRICS];
};


class CPlayer : public CThing
{
public:
	enum
	{
		VELOCITY_SHIFT=2
	};

	void			init();
	void			shutdown();
	void			think(int _frames);
	void			render();

	DVECTOR			getCameraPos();

protected:		
	enum
	{
		DEFAULT_PLAYER_JUMP_VELOCITY=8,
		DEFAULT_PLAYER_MAX_JUMP_FRAMES=10,
		DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES=20,
		DEFAULT_PLAYER_MAX_RUN_VELOCITY=8,
		DEFAULT_PLAYER_RUN_SPEEDUP=4,
		DEFAULT_PLAYER_RUN_REVERSESLOWDOWN=2,
		DEFAULT_PLAYER_RUN_SLOWDOWN=1,
		PLAYER_GRAVITY=4,
		PLAYER_TERMINAL_VELOCITY=8,
	};
	const PlayerMetrics	*getPlayerMetrics();

	// State
	int				setState(PLAYER_STATE _state);
	void			setMode(PLAYER_MODE _mode);
	int				getFacing();
	void			setFacing(int _facing);
	int				getAnimFrame();
	void			setAnimFrame(int _animFrame);
	int				getAnimFrameCount();
	int				getAnimNo();
	void			setAnimNo(int _animNo);
	DVECTOR			getMoveVelocity();
	void			setMoveVelocity(DVECTOR *_moveVel);
	DVECTOR			getPlayerPos();
	int				getPadInputHeld();
	int				getPadInputDown();

	// Collision
	int				isOnSolidGround();

	// Movement
	void moveLeft();
	void moveRight();
	void slowdown();
	void jump();
	void fall();

friend class CPlayerState;



private:

	typedef struct
	{
		PlayerMetrics		m_metrics;
		class CPlayerState	*m_states[NUM_STATES];
	}PlayerMode;

	
	int				m_animFrame;
	int				m_animNo;
	CSkel			m_skel;

	DVECTOR			m_moveVel;
	int				m_facing;
	int				m_fallFrames;

	enum
	{
		INVIBCIBLE_FRAMES__START=120,	// Invincible for this many frames at start of life
		INVINCIBLE_FRAMES__HIT=25,		// Invincible for this many frames after taking damage

		LOOKAROUND_DELAY=90,			// Frames before look around starts
		LOOKAROUND_SCROLLSPEED=2,		// Speed to scroll at
		LOOKAROUND_MAXSCROLL=60,		// Maximum distance to scroll
		LOOKAROUND_RESETSPEED=6,		// Speed of scroll back when look around ended
	};
	int				m_invincibleFrameCount;

	static PlayerMode	s_modes[NUM_PLAYERMODES];
	int					m_currentMode;
	class CPlayerState	*m_currentStateClass;
	PLAYER_STATE		m_currentState;

	int				m_lives;

	DVECTOR			m_cameraOffsetTarget;
	DVECTOR			m_cameraOffset;
	int				m_cameraLookYOffset;
	int				m_cameraLookTimer;

	void			updatePadInput();
	virtual int		readPadInput();
	int				m_padInput;			// Controls that are being held down
	int				m_lastPadInput;		// Last frames controls
	int				m_padInputDown;		// Controls that were pressed this frame

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PLAYER_H__ */

/*===========================================================================
 end */
