/*=========================================================================

	nclam.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


void CNpcEnemy::processCloseClamAttack( int _frames )
{
	s32 velocity;

	if ( m_extendDir == EXTEND_UP )
	{
		m_movementTimer -= _frames;

		if ( m_movementTimer > 0 )
		{
			// extend

			velocity = m_velocity * _frames;

			m_extension += velocity;

			Pos.vx += ( velocity * rcos( m_heading ) ) >> 12;
			Pos.vy += ( velocity * rsin( m_heading ) ) >> 12;
		}
		else
		{
			m_extendDir = EXTEND_DOWN;
		}
	}
	else if ( m_extendDir == EXTEND_DOWN )
	{
		// retract

		if ( m_extension > 0 )
		{
			velocity = -_frames;

			if ( m_extension < _frames )
			{
				velocity = m_extension - _frames;
			}
			
			m_extension += velocity;


			Pos.vx += ( velocity * rcos( m_heading ) ) >> 12;
			Pos.vy += ( velocity * rsin( m_heading ) ) >> 12;
		}
		else
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;
		}
	}
}

void CNpcEnemy::processClamRetract( int _frames )
{
	s32 velocity;

	// retract

	if ( m_extension > 0 )
	{
		velocity = -_frames;

		if ( m_extension < _frames )
		{
			velocity = m_extension - _frames;
		}
		
		m_extension += velocity;


		Pos.vx += ( velocity * rcos( m_heading ) ) >> 12;
		Pos.vy += ( velocity * rsin( m_heading ) ) >> 12;
	}
	else
	{
		// halt clam

		m_controlFunc = NPC_CONTROL_NONE;
		m_timerFunc = NPC_TIMER_NONE;
		m_sensorFunc = NPC_SENSOR_NONE;
	}
}