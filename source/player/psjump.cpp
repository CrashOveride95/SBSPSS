
/*=========================================================================

	psjump.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psjump.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
#endif


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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateJump::enter(CPlayer *_player)
{
	const PlayerMetrics	*metrics;

	metrics=getPlayerMetrics(_player);

	setAnimNo(_player,ANIM_SPONGEBOB_HOVER);
	m_jumpFrames=0;
	DVECTOR	move=getMoveVelocity(_player);
	move.vy=-metrics->m_metric[PM__JUMP_VELOCITY]<<CPlayer::VELOCITY_SHIFT;
	setMoveVelocity(_player,&move);

	CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_JUMP);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateJump::think(CPlayer *_player)
{
	const PlayerMetrics	*metrics;
	int					controlHeld,controlDown;

	metrics=getPlayerMetrics(_player);
	controlHeld=getPadInputHeld(_player);
	controlDown=getPadInputDown(_player);

	if(m_jumpFrames<=metrics->m_metric[PM__MAX_JUMP_FRAMES]&&controlHeld&PI_JUMP)
	{
		m_jumpFrames++;
	}
	else
	{
		setState(_player,STATE_FALL);
	}

	if(controlDown&PI_ACTION)
	{
		setState(_player,STATE_AIRATTACK);
	}
	if(controlHeld&PI_LEFT)
	{
		moveLeft(_player);
	}
	else if(controlHeld&PI_RIGHT)
	{
		moveRight(_player);
	}
	else
	{
		slowdown(_player);
	}
//	advanceAnimFrameAndCheckForEndOfAnim(_player);
}


/*===========================================================================
 end */
