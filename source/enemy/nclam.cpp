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

#ifndef	__ENEMY_NCLAM_H__
#include "enemy\nclam.h"
#endif

#ifndef __PLATFORM_PCLAM_H__
#include "platform\pclam.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__ANIM_CLAM_HEADER__
#include <ACTOR_CLAM_ANIM.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamEnemy::processUserCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcClamEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				if ( playerXDistSqr + playerYDistSqr < 10000 )
				{
					m_controlFunc = NPC_CONTROL_CLOSE;
					m_extendDir = EXTEND_UP;
					m_extension = 0;
					m_movementTimer = GameState::getOneSecondInFrames() >> 3;
					m_velocity = ( getRnd() % 6 ) + 1;

					return( true );
				}
				else
				{
					return( false );
				}
			}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJumpingClamEnemy::postInit()
{
	CNpcClamEnemy::postInit();

	m_drawRotation = m_heading + 1024;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJumpingClamEnemy::processClose( int _frames )
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

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_CLAM_SNAPUP;
				m_frame = 0;
			}
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
			if ( !m_animPlaying )
			{
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::postInit()
{
	CNpcClamEnemy::postInit();

	m_isStunned = false;
	m_isAnimating = false;

	// create platform in same place

	CNpcClamPlatform *platform = new ("clam platform") CNpcClamPlatform;

	platform->setType( CNpcClamPlatform::NPC_CLAM_PLATFORM );
	platform->setGraphic( (u8) 0 );
	platform->init( Pos );
	platform->setTiltable( false );
	//platform->setBBox();
	platform->postInit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::processClose( int _frames )
{
	if ( !m_animPlaying && !m_isStunned )
	{
		m_animPlaying = true;
		m_animNo = ANIM_CLAM_SIDESNAP;
		m_frame = 0;
	}
	/*else if ( !m_animPlaying )
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_animNo = m_data[m_type].initAnim;
		m_frame = 0;
		m_isAnimating = false;
	}*/

	if ( m_isStunned )
	{
		m_isStunned = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				if ( m_frame < ( 5 << 8 ) )
				{
					m_animNo = ANIM_CLAM_SIDESNAP;
					m_frame = 0;
					m_isStunned = true;
					m_animPlaying = false;
				}
				else
				{
					m_oldControlFunc = m_controlFunc;
					m_controlFunc = NPC_CONTROL_COLLISION;
				}

				break;
			}

			default:
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcStaticClamEnemy::processCollision()
{
	CPlayer *player = GameScene.getPlayer();

	player->takeDamage( m_data[m_type].damageToUserType,REACT__GET_DIRECTION_FROM_THING,(CThing*)this );
	processUserCollision( (CThing *) player );

	m_controlFunc = m_oldControlFunc;

	if ( !m_animPlaying && !m_isStunned )
	{
		m_animPlaying = true;
		m_animNo = ANIM_CLAM_SIDESNAP;
		m_frame = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcStaticClamEnemy::getFrameShift( int _frames )
{
	return( ( _frames << 8 ) >> 2 );
}
