/*=========================================================================

	nbuttfly.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NBUTTFLY_H__
#define __ENEMY_NBUTTFLY_H__

#ifndef __ENEMY_NSJBACK_H__
#include "enemy\nsjback.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

class CNpcButterflyBackgroundEnemy : public CNpcSmallJellyfishBackgroundEnemy
{
public:
	virtual void		render();
	virtual int			getFrameCount()							{return( FRM_BUTTERFLY_FLAP04 - FRM_BUTTERFLY_FLAP01 + 1 );}
protected:
	virtual void		processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
};

#endif
