/*=========================================================================

	pspatula.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"	// Damnit.. include order! :( (pkg)
#endif

#include "pickups\pspatula.h"

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
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
void	CSpatulaPickup::init()
{
	CBasePickup::init();
	m_glint=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CSpatulaPickup::shutdown()
{
	CBasePickup::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int spat_glintspeed=3;
int spat_maxglint=600;
DVECTOR	spat_gxy1={-5,30};
DVECTOR spat_gxy2={10,-5};
int spat_glintFrames[]={FRM__GLINT1,FRM__GLINT2,FRM__GLINT3,FRM__GLINT4};
void	CSpatulaPickup::think(int _frames)
{
	CBasePickup::think(_frames);
	m_glint+=_frames*spat_glintspeed;
	if(m_glint>spat_maxglint)m_glint-=spat_maxglint;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CSpatulaPickup::render()
{
	DVECTOR		ofs;
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;
	int			angle;

	ofs=getRenderOffset();
	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__SPATULA);
	x=Pos.vx-ofs.vx-(fh->W/2);
	y=Pos.vy-ofs.vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,1);

	if(m_glint<=255)
	{
		fh=sprites->getFrameHeader(spat_glintFrames[m_glint&0x03]);
		x=x+(((spat_gxy2.vx-spat_gxy1.vx)*m_glint)>>8)+spat_gxy1.vx;
		y=y+(((spat_gxy2.vy-spat_gxy1.vy)*m_glint)>>8)+spat_gxy1.vy;
		sprites->printFT4(fh,x,y,0,0,0);
	}

	CBasePickup::render();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CSpatulaPickup::collect(class CPlayer *_player)
{
	CBasePickup::collect(_player);
}

/*===========================================================================
end */
