/*=========================================================================

	hmasher.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HMASHER_H__
#include "hazard\hmasher.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

void CNpcMasherHazard::init()
{
	CNpcHazard::init();

	m_state = MASHER_DROPPING;
}

void CNpcMasherHazard::processMovement( int _frames )
{
	switch ( m_state )
	{
		case MASHER_DROPPING:
		{
			s8 yMovement = 3 * _frames;

			s8 groundHeight;

			groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight < yMovement )
			{
				// colliding with ground

				Pos.vy += groundHeight;

				// pause and change direction

				m_state = MASHER_RISING;
			}
			else
			{
				// drop down

				Pos.vy += yMovement;
			}

			break;
		}

		case MASHER_RISING:
		{
			if ( m_base.vx - Pos.vx == 0 && m_base.vy - Pos.vy == 0 )
			{
				m_state = MASHER_DROPPING;
			}
			else
			{
				// return to original position

				Pos.vy -= 3 * _frames;

				if ( Pos.vy < m_base.vy )
				{
					Pos.vy = m_base.vy;
				}
			}

			break;
		}
	}
}
