/*=========================================================================

	fma.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__FMA_FMA_H__
#define __FMA_FMA_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CFmaScene: public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"FMA";}

private:
	void	startShutdown();



	class CLevel	*m_level;

	DVECTOR			m_cameraPos;

	int				m_readyToShutdown;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CFmaScene	FmaScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FMA_FMA_H__ */

/*===========================================================================
 end */
