/*=========================================================================

	pcirculr.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCIRCULR_H__
#define __PLATFORM_PCIRCULR_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcCircularPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
protected:
	virtual void		processMovement( int _frames );
};

#endif