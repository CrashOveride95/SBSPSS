/*=========================================================================

	nshell.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSHELL_H__
#define __ENEMY_NSHELL_H__

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

class CNpcShellEnemy : public CNpcEnemy
{
public:
	virtual void		render();
	virtual int			getFrameCount()							{return( 1 );}
};

#endif
