/*=========================================================================

	tteleprt.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "triggers\tteleprt.h"

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTeleportTrigger::collidedWith(CThing *_thisThing)
{
	/*
	CRECT	collisionArea;
	DVECTOR	respawnPos;

	ASSERT(_thisThing->getThingType()==TYPE_PLAYER);

	collisionArea=getCollisionArea();
	respawnPos.vx=collisionArea.x1+((collisionArea.x2-collisionArea.x1)/2);
	respawnPos.vy=collisionArea.y2;
	((CPlayer*)_thisThing)->setRespawnPos(respawnPos);
	*/
#ifdef __USER_paul__
	ASSERT(0);
#endif
}

/*===========================================================================
end */
