/*=========================================================================

	ndustdev.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NDUSTDEV_H__
#define	__ENEMY_NDUSTDEV_H__

class CNpcDustDevilEnemy : public CNpcEnemy
{
public:
	void				postInit();
protected:
	virtual s32			getFrameShift( int _frames );
	virtual void		processMovement( int _frames );
	virtual void		processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange );
};

#endif