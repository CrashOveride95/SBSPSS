/*=========================================================================

	ptrpdoor.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PTRPDOOR_H__
#define __PLATFORM_PTRPDOOR_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcTrapdoorPlatform : public CNpcPlatform
{
public:
	void				postInit();
	DVECTOR const		&getTriggerPos()										{return( m_triggerPos );}
	void				setTriggered()											{m_triggered = true;}
	virtual void		render();
	virtual	CRECT const	*getThinkBBox()											{return( CThing::getThinkBBox() );}
protected:
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );
	virtual void		processMovement( int _frames );

	DVECTOR				m_triggerPos;
	u8					m_triggered;
	u8					m_pointRight;
};

#endif