/*=========================================================================

	npc.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#include "enemy\npc.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __ENEMY_NPCPATH_H__
#include	"enemy\npcpath.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


CNpc::NPC_DATA CNpc::m_data[NPC_UNIT_TYPE_MAX] =
{
	{	// NPC_TEST_TYPE
		NPC_INIT_DEFAULT,
		NPC_SENSOR_JELLYFISH_USER_CLOSE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_JELLYFISH,
		NPC_CLOSE_JELLYFISH_EVADE,
		NPC_TIMER_NONE,
		false,
		3,
		128,
	},

	{	// NPC_SANDY_CHEEKS
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		true,
		3,
		128,
	},

	{	// NPC_SMALL_JELLYFISH_1
		NPC_INIT_DEFAULT,
		NPC_SENSOR_JELLYFISH_USER_CLOSE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_JELLYFISH,
		NPC_CLOSE_JELLYFISH_EVADE,
		NPC_TIMER_NONE,
		false,
		3,
		128,
	},

	{	// NPC_SMALL_JELLYFISH_2
		NPC_INIT_DEFAULT,
		NPC_SENSOR_JELLYFISH_USER_CLOSE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_JELLYFISH,
		NPC_CLOSE_JELLYFISH_EVADE,
		NPC_TIMER_NONE,
		false,
		3,
		128,
	},

	{	// NPC_LARGE_JELLYFISH
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		3,
		128,
	},

	{	// NPC_ANEMONE_1
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		0,
		128,
	},

	{	// NPC_CLAM
		NPC_INIT_DEFAULT,
		NPC_SENSOR_CLAM_USER_CLOSE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_CLAM_ATTACK,
		NPC_TIMER_NONE,
		0,
		128,
	},

	{	// NPC_SQUID_DART
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		5,
		256,
	},

	{	// NPC_FISH_FOLK
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		2,
		128,
	},

	{	// NPC_PRICKLY_BUG
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		1,
		128,
	},

	{	// NPC_SEA_SNAKE_1
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		3,
		256,
	},

	{	// NPC_SEA_SNAKE_2
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		3,
		256,
	},

	{	// NPC_PUFFA_FISH
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		2,
		128,
	},

	{	// NPC_ANGLER_FISH
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		1,
		128,
	},

	{	// NPC_HERMIT_CRAB
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		2,
		128,
	},

	{	// NPC_MINE
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		0,
		128,
	},

	{	// NPC_BOOGER_MONSTER
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		0,
		128,
	},

	{	// NPC_SPIDER_CRAB
		NPC_INIT_DEFAULT,
		NPC_SENSOR_SPIDER_CRAB_USER_CLOSE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_SPIDER_CRAB_ATTACK,
		NPC_TIMER_NONE,
		false,
		3,
		2048,
	},

	{	// NPC_EYEBALL
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		0,
		128,
	},

	{	// NPC_BABY_OCTOPUS
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		2,
		128,
	},

	{	// NPC_ZOMBIE_FISH_FOLK
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		2,
		128,
	},

	{	// NPC_NINJA_STARFISH
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NINJA_STARFISH_USER_CLOSE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_GENERIC_USER_SEEK,
		NPC_TIMER_NONE,
		false,
		3,
		64,
	},

	{	// NPC_GHOST
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		2,
		128,
	},

	{	// NPC_GHOST_PIRATE
		NPC_INIT_DEFAULT,
		NPC_SENSOR_GHOST_PIRATE_USER_CLOSE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_GHOST_PIRATE_ATTACK,
		NPC_TIMER_NONE,
		false,
		0,
		128,
	},

	{	// NPC_FLAMING_SKULL
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_NONE,
		NPC_TIMER_NONE,
		false,
		0,
		128,
	},

	{	// NPC_SHARK_MAN
		NPC_INIT_DEFAULT,
		NPC_SENSOR_SHARK_MAN_USER_VISIBLE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_SHARK_MAN_ATTACK,
		NPC_TIMER_NONE,
		false,
		3,
		2048,
	},

	{	// NPC_OIL_BLOB
		NPC_INIT_DEFAULT,
		NPC_SENSOR_OIL_BLOB_USER_CLOSE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_GENERIC_USER_SEEK,
		NPC_TIMER_NONE,
		false,
		3,
		64,
	},
};


void CNpc::init()
{
	m_type = NPC_OIL_BLOB;

	m_heading = 3072;
	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;
	m_extension = 0;

	Pos.vx = 100;
	Pos.vy = 100;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;

	switch ( m_data[this->m_type].initFunc )
	{
		case NPC_INIT_DEFAULT:
			m_controlFunc = NPC_CONTROL_MOVEMENT;

			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = 10;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			newPos.vy = 10;

			m_npcPath.addWaypoint( newPos );

			/*newPos.vx = 500;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 100;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );*/

			m_npcPath.setPathType( REPEATING_PATH );

			break;

		default:
			m_controlFunc = NPC_CONTROL_MOVEMENT;

			break;
	}
}


void CNpc::shutdown()
{
}


void CNpc::think(int _frames)
{
	switch ( this->m_controlFunc )
	{
		case NPC_CONTROL_MOVEMENT:
			if ( !processSensor() )
			{
				processMovement(_frames);
			}
			else
			{
				processClose(_frames);
			}

			break;

		case NPC_CONTROL_SHOT:
			processShot();

			break;

		case NPC_CONTROL_CLOSE:
			processClose(_frames);

			break;

		case NPC_CONTROL_COLLISION:
			processCollision();

			break;
	}

	processTimer(_frames);
}


bool CNpc::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				CPlayer *player = GameScene.getPlayer();

				DVECTOR playerPos = player->getPos();

				s32 xDist, yDist;
				s32 xDistSqr, yDistSqr;

				xDist = playerPos.vx - this->Pos.vx;
				xDistSqr = xDist * xDist;

				yDist = playerPos.vy - this->Pos.vy;
				yDistSqr = yDist * yDist;

				switch( m_sensorFunc )
				{
					case NPC_SENSOR_JELLYFISH_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_evadeClockwise = ( getRnd() % 2 ) - 1;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_CLAM_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
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

					case NPC_SENSOR_SPIDER_CRAB_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extension = 0;
							m_velocity = 5;
							m_extensionBase = Pos;

							if ( playerPos.vx < Pos.vx )
							{
								m_extendDir = EXTEND_LEFT;
							}
							else
							{
								m_extendDir = EXTEND_RIGHT;
							}

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_OIL_BLOB_USER_CLOSE:
					case NPC_SENSOR_NINJA_STARFISH_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_velocity = m_data[m_type].speed;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_GHOST_PIRATE_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extendDir = EXTEND_UP;
							m_extension = 0;
							m_movementTimer = GameState::getOneSecondInFrames() >> 1;
							m_velocity = 4;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_SHARK_MAN_USER_VISIBLE:
					{
						s32 xDistWaypoint, yDistWaypoint;

						if ( abs( xDist ) < 500 )
						{
							// within range

							// make sure user is closer to shark man than next waypoint

							s32 xDistWaypoint, yDistWaypoint;

							m_npcPath.getDistToNextWaypoint( Pos, &xDistWaypoint, &yDistWaypoint );

							if ( abs( xDist ) < abs( xDistWaypoint ) )
							{
								s16 headingToPlayer = ratan2( yDist, xDist );

								s16 decDir, incDir, moveDist;

								s32 headingToWaypoint = ratan2( yDistWaypoint, xDistWaypoint );

								// check waypoint is in the same direction as the user

								decDir = headingToPlayer - headingToWaypoint;

								if ( decDir < 0 )
								{
									decDir += ONE;
								}

								incDir = headingToWaypoint - headingToPlayer;

								if ( incDir < 0 )
								{
									incDir += ONE;
								}

								if ( decDir < incDir )
								{
									moveDist = decDir;
								}
								else
								{
									moveDist = incDir;
								}

								if ( moveDist > 512 )
								{
									return( false );
								}
								else
								{
									// check if shark man is facing user

									decDir = m_heading - headingToPlayer;

									if ( decDir < 0 )
									{
										decDir += ONE;
									}

									incDir = headingToPlayer - m_heading;

									if ( incDir < 0 )
									{
										incDir += ONE;
									}

									if ( decDir < incDir )
									{
										moveDist = decDir;
									}
									else
									{
										moveDist = incDir;
									}

									if ( moveDist < 1024 )
									{
										m_controlFunc = NPC_CONTROL_CLOSE;
										m_velocity = 8;

										return( true );
									}
									else
									{
										return( false );
									}
								}
							}
							else
							{
								return( false );
							}
						}
						else
						{
							return( false );
						}
					}

					default:
						return( false );
				}
			}

			break;
	}
}

void CNpc::processMovement(int _frames)
{
	if ( _frames > 2 )
	{
		_frames = 2;
	}

	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	switch( m_data[this->m_type].movementFunc )
	{
		case NPC_MOVEMENT_STATIC:
		{
			break;
		}

		case NPC_MOVEMENT_FIXED_PATH:
		{
			bool pathComplete;

			s16 headingToTarget = m_npcPath.think( Pos, &pathComplete );

			if ( !pathComplete )
			{
				s16 decDir, incDir;
				s16 maxTurnRate = m_data[m_type].turnSpeed;

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
				
				s32 preShiftX = _frames * m_data[m_type].speed * rcos( m_heading );
				s32 preShiftY = _frames * m_data[m_type].speed * rsin( m_heading );

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

				moveVel = ( _frames * m_data[m_type].speed ) << 8;
			}

			break;
		}

		case NPC_MOVEMENT_USER_SEEK:
		{
			CPlayer *player;
			
			player = GameScene.getPlayer();

			break;
		}

		case NPC_MOVEMENT_VERTICAL:
		{
			Pos.vy--;

			break;
		}

		default:

			break;
	}

	processMovementModifier(_frames, moveX, moveY, moveVel, moveDist);
}

void CNpc::processMovementModifier(int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange)
{
	switch( m_data[m_type].movementModifierFunc )
	{
		case NPC_MOVEMENT_MODIFIER_NONE:
		{
			Pos.vx += distX;
			Pos.vy += distY;

			break;
		}

		case NPC_MOVEMENT_MODIFIER_BOB:
		{
			break;
		}

		case NPC_MOVEMENT_MODIFIER_JELLYFISH:
		{
			processSmallJellyfishMovementModifier( _frames, distX, distY, dist, headingChange );

			break;
		}
	}
}

void CNpc::processShot()
{
}

void CNpc::processClose(int _frames)
{
	switch( m_data[this->m_type].closeFunc )
	{
		case NPC_CLOSE_JELLYFISH_EVADE:
			processCloseSmallJellyfishEvade( _frames );

			break;

		case NPC_CLOSE_CLAM_ATTACK:
			processCloseClamAttack( _frames );

			break;

		case NPC_CLOSE_SPIDER_CRAB_ATTACK:
			processCloseSpiderCrabAttack( _frames );

			break;

		case NPC_CLOSE_GENERIC_USER_SEEK:
			processCloseGenericUserSeek( _frames );

			break;

		case NPC_CLOSE_GHOST_PIRATE_ATTACK:
			processCloseGhostPirateAttack( _frames );

			break;

		case NPC_CLOSE_SHARK_MAN_ATTACK:
			processCloseSharkManAttack( _frames );

			break;

		default:
			break;
	}
}

void CNpc::processCollision()
{
}

void CNpc::processTimer(int _frames)
{
	switch( m_timerFunc )
	{
		case NPC_TIMER_NONE:
			{
				break;
			}

		case NPC_TIMER_EVADE_DONE:
		case NPC_TIMER_ATTACK_DONE:
			{
				this->m_timerTimer -= _frames;

				if ( m_timerTimer <= 0 )
				{
					this->m_timerFunc = NPC_TIMER_NONE;
					this->m_sensorFunc = m_data[this->m_type].sensorFunc;
				}

				break;
			}

		default:
			break;
	}
}

void CNpc::render()
{
}

void CNpc::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CConversation *currentConversation = GameScene.getConversation();

	if ( m_data[this->m_type].canTalk )
	{
		DVECTOR sourcePos;
		s32 xDiffSqr, yDiffSqr;

		// check talk distance

		sourcePos = sourceThing->getPos();

		xDiffSqr = this->Pos.vx - sourcePos.vx;
		xDiffSqr *= xDiffSqr;

		yDiffSqr = this->Pos.vy - sourcePos.vy;
		yDiffSqr *= yDiffSqr;

		if ( xDiffSqr + yDiffSqr < 250 )
		{
			if( !currentConversation->isActive() )
			{
				currentConversation->trigger( SCRIPTS_SPEECHTEST_DAT );
			}
		}
	}
}
