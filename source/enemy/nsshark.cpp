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

#ifndef	__ENEMY_NSSHARK_H__
#include "enemy\nsshark.h"
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

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#include "fx\fx.h"
#include "fx\fxnrgbar.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::postInit()
{
	m_state = SUB_SHARK_MINE_1;
	m_extendDir = EXTEND_RIGHT;
	m_npcPath.setPathType( CNpcPath::PONG_PATH );
	m_salvoCount = 0;
	m_meterOn=false;

	if ( CLevel::getIsBossRespawn() )
	{
		m_health = CLevel::getBossHealth();
		m_speed = m_data[m_type].speed + ( ( 3 * ( m_data[m_type].initHealth - m_health ) ) / m_data[m_type].initHealth );
	}

	m_invulnerableTimer = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::think( int _frames )
{
	if ( m_invulnerableTimer > 0 )
	{
		m_invulnerableTimer -= _frames;
	}

	CNpcEnemy::think( _frames );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::processMovement( int _frames )
{
	if ( !m_animPlaying )
	{
		if ( playerXDistSqr + playerYDistSqr < 100 && !m_salvoCount )
		{
			m_animPlaying = true;
			m_animNo = ANIM_SHARKSUB_SWIPE;
			m_frame = 0;
		}
		else
		{
			m_animPlaying = true;
			m_animNo = ANIM_SHARKSUB_SWIM;
			m_frame = 0;
		}
	}

	if ( m_timerTimer <= 0 )
	{
		if ( m_salvoCount > 0 )
		{
			// drop mine

			CProjectile *projectile;
			projectile = CProjectile::Create();
			projectile->init( Pos, 1024, CProjectile::PROJECTILE_MINE, CProjectile::PROJECTILE_FINITE_LIFE );
			projectile->setGraphic( FRM__SHARKMINE );

			m_salvoCount--;

			m_timerTimer = ( GameState::getOneSecondInFrames() >> 2 ) * ( 1 + ( ( 3 * m_health ) / m_data[m_type].initHealth ) );
		}
	}

	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;
	}

	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

	if ( moveX > 0 )
	{
		m_extendDir = EXTEND_RIGHT;
	}
	else
	{
		m_extendDir = EXTEND_LEFT;
	}

	Pos.vx += moveX;
	Pos.vy += moveY;

	if ( m_movementTimer <= 0 && m_salvoCount < 1 )
	{
		m_controlFunc = NPC_CONTROL_CLOSE;
	}



	/*if ( m_extendDir == EXTEND_RIGHT )
	{
		s32 xDist = 600 - Pos.vx;
		s32 xDistSqr = xDist * xDist;
		s32 yDist = m_base.vy - Pos.vy;
		s32 yDistSqr = yDist * yDist;

		if ( ( xDistSqr + yDistSqr ) > 100 )
		{
			processGenericGotoTarget( _frames, xDist, yDist, m_speed );
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
			processGenericGotoTarget( _frames, xDist, yDist, m_speed );
		}
		else
		{
			m_extendDir = EXTEND_RIGHT;

			if ( m_movementTimer <= 0 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
			}
		}
	}*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::processClose( int _frames )
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
			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SWIM;
				m_frame = 0;
			}

			processGenericGotoTarget( _frames, playerXDist, 0, m_speed );

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			if ( Pos.vx < minX || Pos.vx > maxX || playerXDistSqr < 100 )
			{
				// fire at player

				m_salvoCount = 5;
				m_state++;
				m_movementTimer = GameState::getOneSecondInFrames() * ( 1 + ( ( 7 * m_health ) / m_data[m_type].initHealth ) );
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
				m_animNo = ANIM_SHARKSUB_SPRINTOPEN;
				m_frame = 0;
			}

			processGenericGotoTarget( _frames, playerXDist, 0, NPC_SUB_SHARK_HIGH_SPEED );

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			if ( Pos.vx < minX || Pos.vx > maxX || playerXDistSqr < 10000 )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_CHOMP;
				m_frame = 0;

				m_state = SUB_SHARK_SWALLOW;
			}

			break;
		}

		case SUB_SHARK_SWALLOW:
		{
			// if ( collision )
			// else

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			if ( m_extendDir == EXTEND_RIGHT )
			{
				//s32 xDist = 600 - Pos.vx;
				s32 xDist = maxX - Pos.vx;
				s32 xDistSqr = xDist * xDist;

				if ( xDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, xDist, 0, NPC_SUB_SHARK_HIGH_SPEED );
				}
				else
				{
					m_extendDir = EXTEND_LEFT;
				}
			}
			else
			{
				//s32 xDist = 100 - Pos.vx;
				s32 xDist = minX - Pos.vx;
				s32 xDistSqr = xDist * xDist;

				if ( xDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, xDist, 0, NPC_SUB_SHARK_HIGH_SPEED );
				}
				else
				{
					m_extendDir = EXTEND_RIGHT;
				}
			}

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = ANIM_SHARKSUB_SWIM;
				m_frame = 0;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_movementTimer = GameState::getOneSecondInFrames() * ( 1 + ( ( 7 * m_health ) / m_data[m_type].initHealth ) );
				m_state = SUB_SHARK_MINE_1;
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::processShot( int _frames )
{
	switch( m_data[m_type].shotFunc )
	{
		case NPC_SHOT_NONE:
		{
			// do nothing

			break;
		}

		case NPC_SHOT_GENERIC:
		{
			switch ( m_state )
			{
				case NPC_GENERIC_HIT_CHECK_HEALTH:
				{
					m_health -= 5;

					if ( m_health < 0 )
					{
						m_state = NPC_GENERIC_HIT_DEATH_START;
						m_isDying = true;
					}
					else
					{
						m_state = NPC_GENERIC_HIT_RECOIL;

						m_animPlaying = true;
						m_animNo = m_data[m_type].recoilAnim;
						m_frame = 0;
						m_speed = m_data[m_type].speed + ( ( 3 * ( m_data[m_type].initHealth - m_health ) ) / m_data[m_type].initHealth );
					}

					break;
				}

				case NPC_GENERIC_HIT_RECOIL:
				{
					m_invulnerableTimer = 2 * GameState::getOneSecondInFrames();

					if ( !m_animPlaying )
					{
						m_state = 0;
						m_controlFunc = NPC_CONTROL_MOVEMENT;
					}

					break;
				}

				case NPC_GENERIC_HIT_DEATH_START:
				{
					m_animPlaying = true;
					m_animNo = m_data[m_type].dieAnim;
					m_frame = 0;
					m_state = NPC_GENERIC_HIT_DEATH_END;
					m_isDying = true;

					if ( m_data[m_type].deathSfx < CSoundMediator::NUM_SFXIDS )
					{
						if( m_soundId != NOT_PLAYING )
						{
							CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
						}

						m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].deathSfx, true );
					}

					m_speed = -5;

					if (m_data[m_type].skelType)
					{
						m_actorGfx->SetOtPos( 0 );
					}

					break;
				}

				case NPC_GENERIC_HIT_DEATH_END:
				{
					if ( !m_animPlaying )
					{
						m_drawRotation += 64 * _frames;
						m_drawRotation &= 4095;

						Pos.vy += m_speed * _frames;

						if ( m_speed < 5 )
						{
							m_speed++;
						}

						DVECTOR	offset = CLevel::getCameraPos();

						if ( Pos.vy - offset.vy > VidGetScrH() )
						{
							if ( m_data[m_type].respawning )
							{
								m_isActive = false;

								m_timerFunc = NPC_TIMER_RESPAWN;
								m_timerTimer = 4 * GameState::getOneSecondInFrames();
							}
							else
							{
								setToShutdown();
								CGameScene::setBossHasBeenKilled();
							}
						}
					}

					break;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::shutdown()
{
	if ( m_state != NPC_GENERIC_HIT_DEATH_END )
	{
		CLevel::setIsBossRespawn( true );
		CLevel::setBossHealth( m_health );
	}

	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			if (!m_meterOn)
			{
				CFXNRGBar	*T=(CFXNRGBar*)CFX::Create(CFX::FX_TYPE_NRG_BAR,this);
				T->SetMax(m_health);
				m_meterOn=true;
			}

			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			m_actorGfx->RotateScale( SprFrame, renderPos, 0, 4096, 4096 );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSubSharkEnemy::collidedWith(CThing *_thisThing)
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				ATTACK_STATE playerState = player->getAttackState();

				if(playerState==ATTACK_STATE__NONE)
				{
					if ( !player->isRecoveringFromHit() )
					{
						switch( m_data[m_type].detectCollision )
						{
							case DETECT_NO_COLLISION:
							{
								// ignore

								break;
							}

							case DETECT_ALL_COLLISION:
							{
								m_oldControlFunc = m_controlFunc;
								m_controlFunc = NPC_CONTROL_COLLISION;

								processUserCollision( _thisThing );

								break;
							}

							case DETECT_ATTACK_COLLISION_GENERIC:
							{
								processAttackCollision();
								processUserCollision( _thisThing );

								break;
							}
						}
					}
				}
				else if ( m_invulnerableTimer <= 0 )
				{
					// player is attacking, respond appropriately

					if ( m_controlFunc != NPC_CONTROL_SHOT )
					{
						if(playerState==ATTACK_STATE__BUTT_BOUNCE)
						{
							player->justButtBouncedABadGuy();
						}
						m_controlFunc = NPC_CONTROL_SHOT;
						m_state = NPC_GENERIC_HIT_CHECK_HEALTH;

						drawAttackEffect();
					}
				}

				break;
			}

			case TYPE_ENEMY:
			{
				CNpcEnemy *enemy = (CNpcEnemy *) _thisThing;

				if ( canCollideWithEnemy() && enemy->canCollideWithEnemy() )
				{
					processEnemyCollision( _thisThing );
				}

				break;
			}

			default:
				ASSERT(0);
				break;
		}
	}
}
