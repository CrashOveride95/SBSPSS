/*=========================================================================

	psfall.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSFALL_H__
#define __PLAYER_PSFALL_H__

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

class CPlayerStateFall : public CPlayerState
{
public:
	void			enter(class CPlayer *_player);
	void			think(class CPlayer *_player);

private:
	int				m_fallFrames;

};


class CPlayerStateFallFar : public CPlayerStateFall
{
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSFALL_H__ */

/*===========================================================================
 end */



