/*=========================================================================

	pschop.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSCHOP_H__
#define __PLAYER_PSCHOP_H__

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

class CPlayerStateChop : public CPlayerState
{
public:
	void			enter(class CPlayer *_player);
	void			think(class CPlayer *_player);

};


class CPlayerStateRunChop : public CPlayerStateChop
{
public:
	void			think(class CPlayer *_player);

};


class CPlayerStateAirChop : public CPlayerStateChop
{
public:
	void			think(class CPlayer *_player);

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSCHOP_H__ */

/*===========================================================================
 end */



