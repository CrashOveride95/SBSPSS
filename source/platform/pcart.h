/*=========================================================================

	pcart.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCART_H__
#define __PLATFORM_PCART_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcCartPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		render();
	virtual u8			isCart()												{return( true );}
	virtual void		jump();
protected:
	virtual void		processMovement( int _frames );
	virtual void		collidedWith(CThing *_thisThing);

	s32					m_carSpeed;
	u8					m_isActivated;
	u8					m_inJump;
	s32					m_vertSpeed;
	u8					m_playerAttached;
};

#endif