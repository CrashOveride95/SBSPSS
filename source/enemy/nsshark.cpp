/*=========================================================================

	nsshark.cpp

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

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef	__ANIM_SHARKSUB_HEADER__
#include <ACTOR_SHARKSUB_ANIM.h>
#endif


void CNpcEnemy::processSubSharkMovement( int _frames )
{
	if ( !m_animPlaying )
	{
		if ( playerXDistSqr + playerYDistSqr < 100 && !m_salvoCount )
		{
			m_animPlaying = true;
			m_animNo = ANIM_SHARKSUB_SHARKSUBSWIPE;
			m_frame = 0;
		}
		else
		{
			m_animPlaying = true;
			m_animNo = ANIM_SHARKSUB_SHARKSUBSWIM;
			m_frame = 0;
		}
	}

	if ( m_timerTimer <= 0 )
	{
		if ( m_salvoCount > 0 )
		{
			// drop mine

			CProjectile *projectile;
			projectile = new( "test projectile" ) CProjectile;
			projectile->init( Pos, 1024 );

			m_salvoCount--;

			m_timerTimer = GameState::getOneSecondInFrames() * 1;
		}
	}

	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;
	}

	if ( m_extendDir == EXTEND_RIGHT )
	{
		s32 xDist = 600 - Pos.vx;
		s32 xDistSqr = xDist * xDist;
		s32 yDist = m_base.vy - Pos.vy;
		s32 yDistSqr = yDist * yDist;

		if ( ( xDistSqr + yDistSqr ) > 100 )
		{
			processGenericGotoTarget( _frames, xDist, yDist, m_data[m_type].speed );
		}
		else
		{
			m_extendDir = EXTEND_LEFT;

			if ( m_movementTimer <= 0 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
			}
		}
	}
	else
	{
		s32 xDist = 100 - Pos.vx;
		s32 xDistSqr = xDist * xDist;
		s32 yDist = m_base.vy - Pos.vy;
		s32 yDistSqr = yDist * yDist;

		if ( ( xDistSqr + yDistSqr ) > 100 )
		{
			processGenericGotoTarget( _frames, xDist, yDist, m_data[m_type].speed );
		}
		else
		{
			m_extendDir = EXTEND_RIGHT;

			if ( m_movementTimer <= 0 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
			}
		}
	}
}

void CNpcEnemy::processCloseSubSharkAttack( int _frames )
{
	if ( m_state != SUB_SHARK_SWALLOW )
	{
		if ( playerXDist > 0 )
		{
			m_extendDir = EXTEND_RIGHT;
		}
		else
		{
			m_extendDir = EXTEND_LEFT;
		}
	}

	switch( m_state )
	{
		case SUB_SHARK_MINE_1:
		case SUB_SHARK_MINE_2:
		{
			processGenericGotoTarget( _frames, playerXDist, 0, m_data[m_type].speed );

			if ( playerXDistSqr < 100 )
			{
				// fire at player

				m_salvoCount = 5;
				m_state++;
				m_movementTimer = GameState::getOneSecondInFrames() * 8;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
			}

			break;
		}

		case SUB_SHARK_CYCLE:
		{
			// charge player

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SHARKSUBSPRINTOPEN;
				m_frame = 0;
			}

			processGenericGotoTarget( _frames, playerXDist, 0, NPC_SUB_SHARK_HIGH_SPEED );

			if ( playerXDistSqr < 10000 )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SHARKSUBCHOMP;
				m_frame = 0;

				m_state = SUB_SHARK_SWALLOW;
			}

			break;
		}

		case SUB_SHARK_SWALLOW:
		{
			// if ( collision )
			// else

			if ( m_extendDir == EXTEND_RIGHT )
			{
				Pos.vx += NPC_SUB_SHARK_HIGH_SPEED;
			}
			else
			{
				Pos.vx -= NPC_SUB_SHARK_HIGH_SPEED;
			}

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SHARKSUBSWIM;
				m_frame = 0;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_movementTimer = GameState::getOneSecondInFrames() * 8;
				m_state = SUB_SHARK_MINE_1;
			}

			break;
		}
	}
}