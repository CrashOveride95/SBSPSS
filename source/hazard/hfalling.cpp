/*=========================================================================

	hfalling.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFALLING_H__
#include "hazard\hfalling.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::init()
{
	CNpcHazard::init();

	m_movementTimer = 3 * GameState::getOneSecondInFrames();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::processMovement( int _frames )
{
	s8 groundHeight;
	s8 yMovement;

	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;

		if ( m_movementTimer <= 0 )
		{
			Pos = m_base;
		}
		else
		{
			Pos.vx = m_base.vx + ( -3 + ( getRnd() % 7 ) );
			Pos.vy = m_base.vy + ( -3 + ( getRnd() % 7 ) );
		}
	}
	else
	{
		yMovement = 3 * _frames;

		groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

		if ( groundHeight < yMovement )
		{
			// colliding with ground

			Pos.vy += groundHeight;

			m_isActive = false;
			m_timerActive = true;
			m_timer = 3 * GameState::getOneSecondInFrames();
		}
		else
		{
			// drop down

			Pos.vy += yMovement;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::processTimer( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		m_timerActive = false;
		m_isActive = true;
		Pos = m_base;
		m_movementTimer = 3 * GameState::getOneSecondInFrames();
	}
}
