/*=========================================================================

	pjellfsh.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PJELLFSH_H__
#define __PLATFORM_PLANTERN_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcJellyfishPlatform : public CNpcPlatform
{
public:
	virtual void		render();
};

#endif