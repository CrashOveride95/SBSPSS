/*=========================================================================

	map.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__MAP_MAP_H__
#define __MAP_MAP_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CMapScene: public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"Map";}


private:
	void	copyImageToScreen(int _file,int _x,int _y,int _w,int _h);


	class FontBank		*m_font;
	int					m_readyToExit;

	char				*m_screenImage;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CMapScene	MapScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __MAP_MAP_H__ */

/*===========================================================================
 end */
