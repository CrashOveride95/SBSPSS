/*=========================================================================

	pfishhk.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFISHHK_H__
#define __PLATFORM_PFISHHK_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcFishHookPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		render();
protected:
	virtual void		processLifetime( int _frames );
	virtual void		processMovement( int _frames );

	u8					m_isMoving;
	DVECTOR				m_lineBase;
};

#endif