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

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#include "gfx/actor.h"


#ifndef __DATA_STRUCTS_HEADER__
#include <dstructs.h>
#endif

#ifndef	__PLAYER_PSTATES_H__
#include "player\pstates.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef enum
{
	PLAYER_MODE_BASICUNARMED,
	PLAYER_MODE_FULLUNARMED,
	PLAYER_MODE_BALLOON,
	PLAYER_MODE_BUBBLE_MIXTURE,
	PLAYER_MODE_NET,
	PLAYER_MODE_CORALBLOWER,
	PLAYER_MODE_JELLY_LAUNCHER,

	PLAYER_MODE_DEAD,

	PLAYER_MODE_FLY,

	NUM_PLAYERMODES,
}PLAYER_MODE;

typedef enum
{
	STATE_IDLE,
	STATE_IDLETEETER,
	STATE_JUMP,
	STATE_RUN,
	STATE_FALL,
	STATE_FALLFAR,
	STATE_HITGROUND,
	STATE_BUTTBOUNCE,
	STATE_BUTTFALL,
	STATE_BUTTLAND,
	STATE_DUCK,
	STATE_SOAKUP,
	STATE_GETUP,

	NUM_STATES,
}PLAYER_STATE;

enum
{
	FACING_LEFT=+1,
	FACING_RIGHT=-1,
};

typedef enum
{
	DAMAGE__NONE,			// put in so that an item can push the user but not damage them
	DAMAGE__FALL,
	DAMAGE__ELECTROCUTION,
	DAMAGE__LAVA,
	DAMAGE__HIT_ENEMY,
	DAMAGE__SHOCK_ENEMY,
	DAMAGE__GAS_ENEMY,
	DAMAGE__POISON_ENEMY,
	DAMAGE__SWALLOW_ENEMY,
	DAMAGE__PINCH_ENEMY,
	DAMAGE__SQUASH_ENEMY,
	DAMAGE__BURN_ENEMY,
	DAMAGE__BITE_ENEMY,
}DAMAGE_TYPE;


// The input from the control pad is remapped to this rather than keeping it in the
// normal pad format. This allows us to store all input in one byte ( as opposed to
// two bytes ) for demo recording and means that the player state codes don't have
// to keep using CPadConfig to remap the controls internally.
typedef enum
{
	PI_NONE			=0,
	PI_UP			=1<<0,
	PI_DOWN			=1<<1,
	PI_LEFT			=1<<2,
	PI_RIGHT		=1<<3,
	PI_JUMP			=1<<4,
	PI_ACTION		=1<<5,
}PLAYERINPUT;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayer : public CPlayerThing
{
public:
	enum
	{
		MAX_HEALTH=5,
		MAX_LIVES=99,
	};

	virtual void	init();
	virtual void	shutdown();
	virtual void	think(int _frames);
	virtual void	render();
	virtual void	shove(DVECTOR move);
	virtual void	setHasPlatformCollided( bool newVal );
	virtual bool	getHasPlatformCollided();

	DVECTOR			getCameraPos()										{return m_cameraPos;}

	void			setLayerCollision(class CLayerCollision *_layer)	{m_layerCollision=_layer;}
	void			setMapSize(DVECTOR _mapSize);
	void			setRespawnPos(DVECTOR _respawn)						{m_respawnPos=_respawn;}

	int				getHeightFromGround(int _x,int _y,int _maxHeight=32);
	int				getHeightFromGroundNoPlatform(int _x,int _y,int _maxHeight=32);

	void			addHealth(int _health);
	void			addLife();

public:
	void			setMode(PLAYER_MODE _mode);
	int				getFacing();
	void			setFacing(int _facing);
	int				getAnimFrame();
	void			setAnimFrame(int _animFrame);
	int				getAnimFrameCount();
	int				getAnimNo();
	void			setAnimNo(int _animNo);
	DVECTOR			getPlayerPos()										{return Pos;}
	void			setPlayerPos(DVECTOR *_pos)							{Pos=*_pos;}
	PLAYERINPUT		getPadInputHeld()									{return m_padInput;}
	PLAYERINPUT		getPadInputDown()									{return m_padInputDown;}
	class CLayerCollision	*getLayerCollision()						{return m_layerCollision;}
																		
	void			takeDamage(DAMAGE_TYPE _damage);

	void			respawn();



public:
	typedef struct
	{
		int						m_frame;
		CSoundMediator::SFXID	m_sfxId;
	} AnimFrameSfx;
	void			renderSb(DVECTOR *_pos,int _animNo,int _animFrame);
private:
	typedef struct
	{
		int							m_numAnimFrameSfx;
		const struct AnimFrameSfx	*m_animFrameSfx;
	} AnimSfx;
	static const AnimSfx	s_animSfx[];
	int				m_animFrame;
	int				m_animNo;
	CActorGfx		*m_actorGfx;

	DVECTOR			m_playerScreenGeomPos;
	DVECTOR			m_cameraPos;

	int				m_facing;

	enum
	{
		INVINCIBLE_FRAMES__START=120,	// Invincible for this many frames at start of life
		INVINCIBLE_FRAMES__HIT=50,		// Invincible for this many frames after taking damage

		LOOKAROUND_DELAY=90,			// Frames before look around starts
		LOOKAROUND_SCROLLSPEED=2,		// Speed to scroll at
		LOOKAROUND_MAXSCROLL=60,		// Maximum distance to scroll
		LOOKAROUND_RESETSPEED=6,		// Speed of scroll back when look around ended
	};
	int				m_invincibleFrameCount;		// Initial invincibility and also invincibility after taking damage

	static class CPlayerMode	*s_playerModes[NUM_PLAYERMODES];
	class CPlayerMode			*m_currentPlayerModeClass;
	int							m_currentMode;

	int				m_lives;
	int				m_health;
	int				m_healthReactFrames;

	DVECTOR			m_cameraOffset;

	void			updatePadInput();
protected:
	virtual PLAYERINPUT	readPadInput();
private:
	PLAYERINPUT			m_padInput;			// Controls that are being held down
	PLAYERINPUT			m_lastPadInput;		// Last frames controls
	PLAYERINPUT			m_padInputDown;		// Controls that were pressed this frame

	
	// Various info about the current map
	class CLayerCollision	*m_layerCollision;
	DVECTOR					m_mapCameraEdges;
	DVECTOR					m_mapEdge;
	DVECTOR					m_respawnPos;



	// Powerups
private:
	enum
	{
		SQUEAKY_BOOTS_TIME=60*10,
		INVINCIBILITY_RING_TIME=60*10,
	};
public:
	void			giveGlasses()				{m_glassesFlag=true;}
	void			giveSqueakyBoots()			{m_squeakyBootsTimer=SQUEAKY_BOOTS_TIME;}
	void			giveInvinvibilityRing()		{m_invinvibilityRingTimer=INVINCIBILITY_RING_TIME;}
	void			giveDivingHelmet()			{m_divingHelmet=true;}

	void			giveBubbleAmmo()			{m_bubbleAmmo+=10;if(m_bubbleAmmo>99)m_bubbleAmmo=99;}
	void			useOneBubble()				{m_bubbleAmmo--;}
	int				getBubbleAmmo()				{return m_bubbleAmmo;}

	void			giveJellyAmmo()				{m_jellyAmmo+=6;if(m_jellyAmmo>99)m_jellyAmmo=99;}
	void			useOneJelly()				{m_jellyAmmo--;}
	int				getJellyAmmo()				{return m_jellyAmmo;}

private:
	int				m_glassesFlag;
	int				m_squeakyBootsTimer;
	int				m_invinvibilityRingTimer;
	int				m_divingHelmet;
	int				m_bubbleAmmo;
	int				m_jellyAmmo;

	// Platforms
public:
	void			setPlatform(CThing *_newPlatform);
	void			clearPlatform();
	CThing			*isOnPlatform()						{return m_platform;}

private:
	CThing			*m_platform;
	/*
private:
	CThing			*m_platform;
	bool			m_onPlatform;
	bool			m_prevOnPlatform;
	DVECTOR			m_prevPlatformPos;
	*/

private:
	bool			m_hasPlatformCollided;


	// Graphical resources
public:
	class FontBank		*getFontBank()					{return m_fontBank;}
	class SpriteBank	*getSpriteBank()				{return m_spriteBank;}

	enum
	{
		POWERUPUI_ICONX=40,
		POWERUPUI_ICONY=200,
		POWERUPUI_TEXTX=60,
		POWERUPUI_TEXTY=200,
		POWERUPUI_OT=5,
	};

private:
	class FontBank		*m_fontBank;
	class SpriteBank	*m_spriteBank;

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
