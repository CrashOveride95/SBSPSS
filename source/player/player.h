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
	STATE_SPRINGUP,
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

typedef enum
{
	PLAYER_ADDON_NET,
	PLAYER_ADDON_CORALBLOWER,
	PLAYER_ADDON_JELLYLAUNCHER,
	PLAYER_ADDON_GLASSES,
	PLAYER_ADDON_BUBBLEWAND,

	NUM_PLAYER_ADDONS,
	NO_ADDON,
}PLAYER_ADDONS;

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


// For getAttackState()
typedef enum
{
	ATTACK_STATE__NONE,
	ATTACK_STATE__KARATE,
	ATTACK_STATE__BUTT_BOUNCE,
} ATTACK_STATE;



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
		
		WATERLEVELSHIFT=4,
		WATERMAXHEALTH=(255<<WATERLEVELSHIFT),
		WATERHEALTHPART=WATERMAXHEALTH/(MAX_HEALTH+1),
	};

	typedef struct
	{
		s16			x1,y1;
		s16			x2,y2;
	} CameraBox;

	
	virtual void	init();
	virtual void	shutdown();
	virtual void	think(int _frames);
	virtual void	render();
	virtual void	shove(DVECTOR move);
	virtual int		moveVertical(int _moveDistance);
	virtual int		moveHorizontal(int _moveDistance);
	virtual void	setHasPlatformCollided( bool newVal );
	virtual bool	getHasPlatformCollided();

	int				isTryingToConversateWithFriend()					{return m_allowConversation;}

	DVECTOR			getCameraPos()										{return m_cameraPos;}

	void			setLayerCollision(class CLayerCollision *_layer)	{m_layerCollision=_layer;}
	void			setCameraBox(CameraBox _cameraBox);
	void			setRespawnPos(DVECTOR _respawn)						{m_respawnPos=_respawn;}

	int				getHeightFromGround(int _x,int _y,int _maxHeight=32);
	int				getHeightFromGroundNoPlatform(int _x,int _y,int _maxHeight=32);

	void			setLedgeLookAhead(int _lookAhead)					{m_ledgeLookAhead=_lookAhead;}

	void			addHealth(int _health);
	void			addLife();
	ATTACK_STATE	getAttackState();
	int				isRecoveringFromHit()								{return m_invincibleFrameCount!=0||m_currentMode==PLAYER_MODE_DEAD;}

	void			registerAddon(PLAYER_ADDONS _addon);
	DVECTOR			*getMoveVelocity()									{return &m_moveVelocity;}
	void			setMoveVelocity(const DVECTOR *_moveVelocity)		{m_moveVelocity=*_moveVelocity;}

public:
	void			setMode(PLAYER_MODE _mode);
	int				getFacing();
	void			setFacing(int _facing);
	int				getAnimFrame();
	void			setAnimFrame(int _animFrame);
	int				getAnimFrameCount();
	int				getAnimNo();
	void			setAnimNo(int _animNo);

	void			springPlayerUp();
	void			teleportTo(int _x,int _y);

private:
	void			playAnimFrameSfx(int _animNo,int _animFrame);
	void			calcCameraFocusPointTarget();

	DVECTOR			m_moveVelocity;
	int				m_allowConversation;



public:
	DVECTOR			getPlayerPos()										{return Pos;}
	void			setPlayerPos(DVECTOR *_pos)							{Pos=*_pos;}
	PLAYERINPUT		getPadInputHeld()									{return m_padInput;}
	PLAYERINPUT		getPadInputDown()									{return m_padInputDown;}
	class CLayerCollision	*getLayerCollision()						{return m_layerCollision;}
								
	void			inSoakUpState();
	void			takeDamage(DAMAGE_TYPE _damage);

	void			respawn();



public:
	typedef struct
	{
		int						m_frame;
		CSoundMediator::SFXID	m_sfxId;
	} AnimFrameSfx;
	void			renderSb(DVECTOR *_pos,int _animNo,int _animFrame);

	int				canDoLookAround();

	

private:
	typedef struct
	{
		int							m_animNumber;
		int							m_numAnimFrameSfx;
		const struct AnimFrameSfx	*m_animFrameSfx;
	} AnimSfx;
	static const AnimSfx	s_animSfx[];
	static const int		s_numAnimSfx;
	int				m_animFrame;
	int				m_animNo;
	CActorGfx		*m_actorGfx;

	int				m_lockCamera;
	DVECTOR			m_cameraPos;
	DVECTOR			m_currentCamFocusPointTarget;
	DVECTOR			m_currentCamFocusPoint;
	int				m_facing;
	int				m_padLookAroundTimer;
	int				m_ledgeLookAhead,m_lastLedgeLookAhead;
	int				m_ledgeLookOffset;
	int				m_ledgeLookTimer;
	int				m_leftRightScrollPosition;


	enum
	{
		INVINCIBLE_FRAMES__START=200,	// Invincible for this many frames at start of life
		INVINCIBLE_FRAMES__HIT=100,		// Invincible for this many frames after taking damage
	};
	int				m_invincibleFrameCount;		// Initial invincibility and also invincibility after taking damage

	static class CPlayerMode	*s_playerModes[NUM_PLAYERMODES];
	class CPlayerMode			*m_currentPlayerModeClass;
	int							m_currentMode;


private:
	int				m_lives;
	int				m_health;
	int				m_healthWaterLevel;
	int				m_healthReactFrames;

	void			dieYouPorousFreak();


	void			updatePadInput();
protected:
	virtual PLAYERINPUT	readPadInput();
private:
	PLAYERINPUT			m_padInput;			// Controls that are being held down
	PLAYERINPUT			m_lastPadInput;		// Last frames controls
	PLAYERINPUT			m_padInputDown;		// Controls that were pressed this frame

	
	// Various info about the current map
	class CLayerCollision	*m_layerCollision;
	CameraBox				m_cameraPosLimitBox;
	CameraBox				m_playerPosLimitBox;
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
	int				isWearingGlasses()			{return m_glassesFlag;}
	void			giveSqueakyBoots()			{m_squeakyBootsTimer=SQUEAKY_BOOTS_TIME;}
	int				isWearingBoots()			{return m_squeakyBootsTimer;}
	void			giveInvincibilityRing()		{m_invincibilityRingTimer=INVINCIBILITY_RING_TIME;}
	void			giveDivingHelmet()			{m_divingHelmet=true;}
	int				isWearingDivingHelmet()		{return m_divingHelmet;}

	void			giveBubbleAmmo()			{m_bubbleAmmo+=10;if(m_bubbleAmmo>99)m_bubbleAmmo=99;}
	void			useOneBubble()				{m_bubbleAmmo--;}
	int				getBubbleAmmo()				{return m_bubbleAmmo;}

	void			giveJellyAmmo()				{m_jellyAmmo+=6;if(m_jellyAmmo>99)m_jellyAmmo=99;}
	void			useOneJelly()				{m_jellyAmmo--;}
	int				getJellyAmmo()				{return m_jellyAmmo;}

	void			setIsInWater(int _in)			{m_isInWater=_in;}
	int				getIsInWater()				{return m_isInWater;}

private:
	int				m_glassesFlag;
	int				m_squeakyBootsTimer;
	int				m_invincibilityRingTimer;
	int				m_divingHelmet;
	int				m_bubbleAmmo;
	int				m_jellyAmmo;
	int				m_isInWater;

	// Platforms
public:
	void			setPlatform(CThing *_newPlatform);
	void			clearPlatform();
	CThing			*isOnPlatform()						{return m_platform;}

private:
	CThing			*m_platform;

private:
	bool			m_hasPlatformCollided;


	// Player collision size
public:
	enum
	{
		COLSIZE_BASE_WIDTH=30,
		COLSIZE_BASE_HEIGHT=60,
	};
	void			resetPlayerCollisionSizeToBase();
	void			setPlayerCollisionSize(int _x,int _y,int _w,int _h);
	void			getPlayerCollisionSize(int *_x,int *_y,int *_w,int *_h);




	// Graphical resources
public:
	class FontBank		*getFontBank()					{return m_fontBank;}
	class SpriteBank	*getSpriteBank()				{return m_spriteBank;}

	enum
	{
		HEALTH_ICONX=40,
		HEALTH_ICONY=40,
		POWERUPUI_ICONX=400,
		POWERUPUI_ICONY=40,
		POWERUPUI_TEXTX=440,
		POWERUPUI_TEXTY=37,
		POWERUPUI_OT=0,
		COLLECTEDITEM_BASEX=110,
		COLLECTEDITEM_BASEY=60,
		COLLECTEDITEM_GAP=40,
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
