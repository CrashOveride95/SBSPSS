/*=========================================================================

	psballoon.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psballoon.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
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
void CPlayerStateBalloon::enter(CPlayer *_player)
{
	int	controlHeld;
	controlHeld=getPadInputHeld(_player);

	if(getMoveVelocity(_player).vx)
	{
		setAnimNo(_player,ANIM_SPONGEBOB_RUN);
	}
	else
	{
		setAnimNo(_player,ANIM_SPONGEBOB_RUNSTART);
	}

	if(controlHeld&PI_LEFT)
	{
		setFacing(_player,FACING_LEFT);
	}
	else if(controlHeld&PI_RIGHT)
	{
		setFacing(_player,FACING_RIGHT);
	}
}


/*----------------------------------------------------------------------
  Function:
  Purpose:
  Params:
  Returns:
---------------------------------------------------------------------- */
void CPlayerStateBalloon::think(CPlayer *_player)
{
	int	controlDown,controlHeld;
	controlDown=getPadInputDown(_player);
	controlHeld=getPadInputHeld(_player);

	if(controlDown&PI_JUMP)
	{
		setState(_player,STATE_JUMP);
	}
	if(controlHeld&PI_DOWN)
	{
		setState(_player,STATE_DUCK);
	}
	if(controlDown&PI_ACTION)
	{
		setState(_player,STATE_RUNATTACK);
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
		if(getMoveVelocity(_player).vx==0)
		{
			setState(_player,STATE_IDLE);
		}
		else
		{
			slowdown(_player);
		}
	}

	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setAnimNo(_player,ANIM_SPONGEBOB_RUN);
	}
}


/*===========================================================================
 end */
