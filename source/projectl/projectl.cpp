/*=========================================================================

	projectl.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif


/*****************************************************************************/

void CProjectile::init()
{
	m_spriteBank=new ("projectile sprites") SpriteBank();
	m_spriteBank->load(UI_UIGFX_SPR);

	m_heading = 0;
	m_lifetime = GameState::getOneSecondInFrames() * 2;
	m_movementType = PROJECTILE_DUMBFIRE;
	m_lifetimeType = PROJECTILE_FINITE_LIFE;
	m_state = PROJECTILE_ATTACK;
	m_turnSpeed = 256;
}

void CProjectile::init( DVECTOR initPos, s16 initHeading )
{
	init();

	m_heading = initHeading;
	Pos = initPos;
}

void CProjectile::init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType )
{
	init( initPos, initHeading );

	m_movementType = initMoveType;
	m_lifetimeType = initLifeType;
}

void CProjectile::shutdown()
{
	m_spriteBank->dump();		delete m_spriteBank;
}

bool CProjectile::processTargetSeek( int _frames, DVECTOR targetPos )
{
	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	s32 xDist, yDist;
	s32 xDistSqr, yDistSqr;

	xDist = targetPos.vx - this->Pos.vx;
	xDistSqr = xDist * xDist;

	yDist = targetPos.vy - this->Pos.vy;
	yDistSqr = yDist * yDist;

	//if ( xDistSqr + yDistSqr > 22500 )
	//{
		//this->m_controlFunc = NPC_CONTROL_MOVEMENT;
	//}
	//else
	{
		s16 headingToTarget = ratan2( yDist, xDist );
		s16 maxTurnRate = m_turnSpeed;
		s16 decDir, incDir;

		decDir = m_heading - headingToTarget;

		if ( decDir < 0 )
		{
			decDir += ONE;
		}

		incDir = headingToTarget - m_heading;

		if ( incDir < 0 )
		{
			incDir += ONE;
		}

		if ( decDir < incDir )
		{
			moveDist = -decDir;
		}
		else
		{
			moveDist = incDir;
		}

		if ( moveDist < -maxTurnRate )
		{
			moveDist = -maxTurnRate;
		}
		else if ( moveDist > maxTurnRate )
		{
			moveDist = maxTurnRate;
		}

		m_heading += moveDist;

		m_heading = m_heading % ONE;
		
		s32 preShiftX = _frames * 3 * rcos( m_heading );
		s32 preShiftY = _frames * 3 * rsin( m_heading );

		moveX = preShiftX >> 12;
		if ( !moveX && preShiftX )
		{
			moveX = preShiftX / abs( preShiftX );
		}

		moveY = preShiftY >> 12;
		if ( !moveY && preShiftY )
		{
			moveY = preShiftY / abs( preShiftY );
		}

		Pos.vx += moveX;
		Pos.vy += moveY;
	}

	xDist = targetPos.vx - this->Pos.vx;
	xDistSqr = xDist * xDist;

	yDist = targetPos.vy - this->Pos.vy;
	yDistSqr = yDist * yDist;

	if ( xDistSqr + yDistSqr < 100 )
	{
		return( true );
	}
	else
	{
		return( false );
	}
}

void CProjectile::think(int _frames)
{
	switch( m_movementType )
	{
		case PROJECTILE_USER_SEEK:
		{
			switch( m_state )
			{
				case PROJECTILE_RETURN:
				{
					if ( processTargetSeek( _frames, Parent->getPos() ) )
					{
						Parent->processEvent( PROJECTILE_RETURNED_TO_SOURCE_EVENT, this );
					}

					break;
				}

				case PROJECTILE_ATTACK:
				default:
				{
					CPlayer *player = GameScene.getPlayer();
					DVECTOR playerPos = player->getPos();

					if ( processTargetSeek( _frames, playerPos ) )
					{
						m_state = PROJECTILE_RETURN;
					}

					break;
				}
			}

			break;
		}

		case PROJECTILE_GAS_CLOUD:
		{
			// expand but don't move

			break;
		}

		case PROJECTILE_DUMBFIRE:
		default:
		{
			Pos.vx += ( _frames * 3 * rcos( m_heading ) ) >> 12;
			Pos.vy += ( _frames * 3 * rsin( m_heading ) ) >> 12;

			break;
		}
	}

	if ( m_lifetimeType == PROJECTILE_FINITE_LIFE )
	{
		m_lifetime -= _frames;

		if ( m_lifetime <= 0 )
		{
			shutdown();
			delete this;
		}
	}
}

void CProjectile::render()
{
	DVECTOR	offset;
	int		x,y;
	int		scrnWidth = VidGetScrW();
	int		scrnHeight = VidGetScrH();
	int		spriteWidth = m_spriteBank->getFrameWidth(FRM_BARNACLEBOY);
	int		spriteHeight = m_spriteBank->getFrameHeight(FRM_BARNACLEBOY);

	offset = getScreenOffset();

	x = Pos.vx - offset.vx + ( scrnWidth >> 1 ) - ( spriteWidth >> 1 );
	y = Pos.vy - offset.vy + ( scrnHeight >> 1 ) - ( spriteHeight >> 1 );

	m_spriteBank->printFT4(FRM_BARNACLEBOY,x,y,0,0,0);
}

DVECTOR CProjectile::getScreenOffset()
{
	return CLevel::getCameraPos();
}

void CProjectile::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
}

/*****************************************************************************/
