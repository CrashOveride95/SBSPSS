/*=========================================================================

	hfirebal.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFIREBAL_H__
#define __HAZARD_HFIREBAL_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcFireballHazard : public CNpcHazard
{
public:
	void				init();
protected:
	void				processMovement( int _frames );
	void				processTimer( int _frames );

	s32					m_velocity;
};

#endif