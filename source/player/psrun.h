/*=========================================================================

	psjump.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSRUN_H__
#define __PLAYER_PSRUN_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pstates.h"


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayerStateRun : public CPlayerState
{
public:
	void			enter(class CPlayer *_player);
	void			think(class CPlayer *_player);

private:
	int				m_numberOfTimeAnimHasLooped;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSRUN_H__ */

/*===========================================================================
 end */



