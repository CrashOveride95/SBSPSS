/*=========================================================================

	pclam.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCLAM_H__
#define __PLATFORM_PCLAM_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcClamPlatform : public CNpcPlatform
{
public:
	virtual void		render();
	void				setBBox();
};

#endif