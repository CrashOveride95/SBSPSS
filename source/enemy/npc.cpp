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

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif


#include "Gfx\actor.h"

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENEMY_NSJFISH_H__
#include "enemy\nsjfish.h"
#endif

#ifndef __ENEMY_NHCRAB_H__
#include "enemy\nhcrab.h"
#endif

#ifndef __ENEMY_NSCRAB_H__
#include "enemy\nscrab.h"
#endif

#ifndef __ENEMY_NGEN_H__
#include "enemy\ngen.h"
#endif

#ifndef	__ENEMY_NANEMONE_H__
#include "enemy\nanemone.h"
#endif

#ifndef	__ENEMY_NCLAM_H__
#include "enemy\nclam.h"
#endif

#ifndef	__ENEMY_NOCTO_H__
#include "enemy\nocto.h"
#endif

#ifndef __ENEMY_NFFOLK_H__
#include "enemy\nffolk.h"
#endif

#ifndef __ENEMY_NBBLOB_H__
#include "enemy\nbblob.h"
#endif

#ifndef	__ENEMY_NGPIRATE_H__
#include "enemy\ngpirate.h"
#endif

#ifndef	__ENEMY_NPUFFA_H__
#include "enemy\npuffa.h"
#endif

#ifndef	__ENEMY_NSHRKMAN_H__
#include "enemy\nshrkman.h"
#endif

#ifndef	__ENEMY_NSKLFISH_H__
#include "enemy\nsklfish.h"
#endif

#ifndef	__ENEMY_NEYEBALL_H__
#include "enemy\neyeball.h"
#endif

#ifndef	__ENEMY_NFSKULL_H__
#include "enemy\nfskull.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Friend NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::init()
{
	CNpcThing::init();

	Pos.vx = 100;
	Pos.vy = 100;

	m_extension = EXTEND_RIGHT;

	// temporary
	m_actorGfx=CActorPool::GetActor(ACTORS_CLAM_SBK);

	//m_animPlaying = true;
	m_animNo = 0;
	m_frame = 0;
	m_reversed = false;

	DVECTOR ofs = getCollisionSize();

	m_drawOffset.vx = 0;
	m_drawOffset.vy = -( ofs.vy >> 1 );

	setCollisionCentreOffset( 0, -( ofs.vy >> 1 ) );

	//m_spriteBank=new ("enemy sprites") SpriteBank();
	//m_spriteBank->load(UI_UIGFX_SPR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::shutdown()
{
	//m_spriteBank->dump();		delete m_spriteBank;

	delete m_actorGfx;
	CNpcThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::think(int _frames)
{
	CNpcThing::think(_frames);

	switch( m_data[m_type].movementFunc )
	{
		case NPC_FRIEND_MOVEMENT_GARY:
			processGaryMovement( _frames );

			break;

		case NPC_FRIEND_MOVEMENT_STATIC:
		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::render()
{
	CNpcThing::render();

	// Render
	DVECTOR renderPos;
	DVECTOR	offset = CLevel::getCameraPos();

	renderPos.vx = Pos.vx - offset.vx;
	renderPos.vy = Pos.vy - offset.vy;

	if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
	{
		if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
		{
			m_actorGfx->Render(renderPos,m_animNo,m_frame,m_reversed);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
	switch( evt )
	{
		case USER_REQUEST_TALK_EVENT:
		{
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

				if ( xDiffSqr + yDiffSqr < 10000 )
				{
					if( !CConversation::isActive() )
					{
						CConversation::trigger( SCRIPTS_SPEECHTEST_DAT );
					}
				}
			}

			break;
		}

		default:
			// ignore

			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enemy NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcEnemy::playerXDist;
s32 CNpcEnemy::playerYDist;
s32 CNpcEnemy::playerXDistSqr;
s32 CNpcEnemy::playerYDistSqr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcEnemy::NPC_UNIT_TYPE CNpcEnemy::getTypeFromMapEdit( u16 newType )
{
	return( mapEditConvertTable[newType - NPC_ENEMY_MAPEDIT_OFFSET] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNpcEnemy	*CNpcEnemy::Create(sThingActor *ThisActor)
{
	CNpcEnemy *enemy;

	NPC_UNIT_TYPE enemyType = CNpcEnemy::getTypeFromMapEdit( ThisActor->Type );

	switch( enemyType )
	{
		case CNpcEnemy::NPC_SMALL_JELLYFISH_1:
		{
			enemy = new ("small jellyfish") CNpcSmallJellyfishEnemy;
			break;
		}

		case CNpcEnemy::NPC_HERMIT_CRAB:
		{
			enemy = new ("hermit crab") CNpcHermitCrabEnemy;
			break;
		}

		case CNpcEnemy::NPC_SPIDER_CRAB:
		{
			enemy = new ("spider crab") CNpcSpiderCrabEnemy;
			break;
		}

		case CNpcEnemy::NPC_SPIDER_CRAB_SPAWNER:
		{
			enemy = new ("spider crab spawner") CNpcEnemyGenerator;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_1:
		{
			enemy = new ("anemone 1") CNpcAnemone1Enemy;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_2:
		{
			enemy = new ("anemone 2") CNpcAnemone2Enemy;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_3:
		{
			enemy = new ("anemone 3") CNpcAnemone3Enemy;
			break;
		}

		case CNpcEnemy::NPC_CLAM_JUMP:
		{
			enemy = new ("jumping clam") CNpcJumpingClamEnemy;
			break;
		}

		case CNpcEnemy::NPC_CLAM_STATIC:
		{
			enemy = new ("static clam") CNpcStaticClamEnemy;
			break;
		}

		case CNpcEnemy::NPC_BABY_OCTOPUS:
		{
			enemy = new ("baby octopus") CNpcBabyOctopusEnemy;
			break;
		}

		case CNpcEnemy::NPC_FISH_FOLK:
		case CNpcEnemy::NPC_ZOMBIE_FISH_FOLK:
		{
			enemy = new ("fish folk") CNpcFishFolk;
			break;
		}

		case CNpcEnemy::NPC_BALL_BLOB:
		{
			enemy = new ("ball blob") CNpcBallBlobEnemy;
			break;
		}

		case CNpcEnemy::NPC_GHOST_PIRATE:
		{
			enemy = new ("ghost pirate") CNpcGhostPirateEnemy;
			break;
		}

		case CNpcEnemy::NPC_PUFFA_FISH:
		{
			enemy = new ("puffa fish") CNpcPuffaFishEnemy;
			break;
		}

		case CNpcEnemy::NPC_SHARK_MAN:
		{
			enemy = new ("shark man") CNpcSharkManEnemy;
			break;
		}

		case CNpcEnemy::NPC_SKELETAL_FISH:
		{
			enemy = new ("skeletal fish") CNpcSkeletalFishEnemy;
			break;
		}

		case CNpcEnemy::NPC_EYEBALL:
		{
			enemy = new ("eyeball") CNpcEyeballEnemy;
			break;
		}

		case CNpcEnemy::NPC_FLAMING_SKULL:
		{
			enemy = new ("flaming skull") CNpcFlamingSkullEnemy;
			break;
		}

		default:
		{
			enemy = new ("npc enemy") CNpcEnemy;
			break;
		}
	}

	ASSERT(enemy);

	enemy->setType( enemyType );
	enemy->init();

	return(enemy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setupWaypoints( sThingActor *ThisActor )
{
	u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	setStartPos( newXPos, newYPos );
	addWaypoint( newXPos, newYPos );

	if ( ThisActor->PointCount > 1 )
	{
		for (int pointNum = 1 ; pointNum < ThisActor->PointCount ; pointNum++ )
		{
			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			addWaypoint( newXPos, newYPos );

			if ( pointNum == 1 )
			{
				setHeading( newXPos, newYPos );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::addWaypoint( s32 xPos, s32 yPos )
{
	DVECTOR newPos;

	newPos.vx = xPos << 4;
	newPos.vy = yPos << 4;

	m_npcPath.addWaypoint( newPos );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setStartPos( s32 xPos, s32 yPos )
{
	Pos.vx = xPos << 4;
	Pos.vy = yPos << 4;

	m_initPos = m_base = Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setHeading( s32 xPos, s32 yPos )
{
	m_heading = ratan2( ( yPos << 4 ) - Pos.vy, ( xPos << 4 ) - Pos.vx );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::init()
{
	CEnemyThing::init();
	if (m_data[m_type].skelType)
	{
		m_actorGfx=CActorPool::GetActor((FileEquate)m_data[m_type].skelType);
		m_spriteBank=0;

	}
	else
	{
		m_actorGfx=0;
		m_spriteBank=new ("enemy sprites") SpriteBank();
		m_spriteBank->load(SPRITES_SPRITES_SPR);
	}

	m_animPlaying = true;
	m_animNo = m_data[m_type].initAnim;
	m_frame = 0;

	m_heading = m_fireHeading = 0;
	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;
	m_extension = 0;
	m_rotation = 0;
	m_reversed = false;
	m_salvoCount = 0;
	m_isActive = true;

	m_health = m_data[this->m_type].initHealth;

	m_extendDir = EXTEND_RIGHT;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;
	m_movementFunc = m_data[this->m_type].movementFunc;

	m_controlFunc = NPC_CONTROL_MOVEMENT;

	m_npcPath.initPath();

	DVECTOR ofs = getCollisionSize();

	m_drawOffset.vx = 0;
	m_drawOffset.vy = 0;

	setCollisionCentreOffset( 0, -( ofs.vy >> 1 ) );

	m_positionHistory = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::postInit()
{
	switch ( m_data[this->m_type].initFunc )
	{
		case NPC_INIT_DEFAULT:
		{
			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			break;
		}

		case NPC_INIT_BALL_BLOB:
		{
			m_heading = m_fireHeading = 128;

			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			break;
		}

		case NPC_INIT_HERMIT_CRAB:
		{
			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			m_state = HERMIT_CRAB_NO_ATTACK;

			break;
		}

		case NPC_INIT_GHOST_PIRATE:
			m_heading = m_fireHeading = 3072;

			break;

		case NPC_INIT_SKULL_STOMPER:
		{
			m_extendDir = EXTEND_DOWN;

			break;
		}

		case NPC_INIT_MOTHER_JELLYFISH:
		{
			m_state = MOTHER_JELLYFISH_RETURN_TO_START_1;

			break;
		}

		case NPC_INIT_FLYING_DUTCHMAN:
		{
			m_state = FLYING_DUTCHMAN_ATTACK_PLAYER_1;
			m_extendDir = EXTEND_UP;

			s32 minX, maxX;
			m_npcPath.getPathXExtents( &minX, &maxX );

			m_extension = minX;

			break;
		}

		case NPC_INIT_SUB_SHARK:
		{
			m_state = SUB_SHARK_MINE_1;
			m_extendDir = EXTEND_RIGHT;

			break;
		}

		case NPC_INIT_IRON_DOGFISH:
		{
			m_state = IRON_DOGFISH_THUMP_1;
			m_extendDir = EXTEND_RIGHT;

			break;
		}

		case NPC_INIT_FISH_HOOK:
		{
			m_heading = m_fireHeading = 3072;

			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = -100;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

			break;
		}

		case NPC_INIT_PENDULUM:
		{
			m_extendDir = EXTEND_LEFT;
			m_extension = 0;
			m_heading = 1024;

			break;
		}

		case NPC_INIT_FIREBALL:
		{
			DVECTOR newPos;

			newPos.vx = 300;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );
			m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

			m_extension = 0;
			m_velocity = m_data[m_type].speed;
			m_timerTimer = GameState::getOneSecondInFrames() * 4;

			break;
		}

		case NPC_INIT_RETURNING_HAZARD:
		{
			m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

			break;
		}

		case NPC_INIT_FISH_FOLK:
		{
			m_heading = m_fireHeading = 0;

			DVECTOR newPos;

			newPos.vx = 200;
			newPos.vy = 400;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			newPos.vy = 400;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			break;
		}

		case NPC_INIT_ANEMONE_2:
		{
			CProjectile *projectile;
			s16 heading;

			for ( int fireLoop = 0 ; fireLoop < 5 ; fireLoop++ )
			{
				DVECTOR spikePos;

				heading = m_heading - 1024 + ( fireLoop * 512 );
				heading &= 4095;

				spikePos = Pos;
				spikePos.vx += ( 40 * rcos( heading ) ) >> 12;
				spikePos.vy += ( 40 * rsin( heading ) ) >> 12;

				projectile = new( "anemone lev2 projectile" ) CProjectile;
				projectile->init( spikePos, heading, CProjectile::PROJECTILE_FIXED, CProjectile::PROJECTILE_INFINITE_LIFE );
				projectile->setLayerCollision( m_layerCollision );

				addChild( projectile );
			}

			break;
		}

		case NPC_INIT_CIRCULAR_PLATFORM:
		{
			Pos.vx = 300;
			Pos.vy = 300;

			m_base = Pos;

			m_extendDir = EXTEND_CLOCKWISE;
			m_extension = 100;

			break;
		}

		case NPC_INIT_PARASITIC_WORM:
		{
			DVECTOR newPos;

			newPos.vx = 100;
			//newPos.vy = 10;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			//newPos.vy = 10;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			//newPos.vy = 100;
			newPos.vy = 300;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 100;
			//newPos.vy = 100;
			newPos.vy = 300;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			// create start of list
			CNpcPositionHistory *newPosition;
			newPosition = new ("position history") CNpcPositionHistory;
			newPosition->pos = Pos;
			m_positionHistory = newPosition;

			CNpcPositionHistory *currentPosition = m_positionHistory;

			// create rest of list

			for ( int histLength = 1 ; histLength < ( NPC_PARASITIC_WORM_LENGTH * NPC_PARASITIC_WORM_SPACING ) ; histLength++ )
			{
				newPosition = new ("position history") CNpcPositionHistory;
				newPosition->pos = Pos;
				newPosition->next = NULL;
				newPosition->prev = currentPosition;

				currentPosition->next = newPosition;
				currentPosition = newPosition;
			}

			// link ends together for circular list

			currentPosition->next = m_positionHistory;
			m_positionHistory->prev = currentPosition;

			for ( int segCount = 0 ; segCount < NPC_PARASITIC_WORM_LENGTH ; segCount++ )
			{
				CNpcEnemy *segment;
				segment = new ("segment") CNpcEnemy;
				segment->setType( CNpcEnemy::NPC_PARASITIC_WORM_SEGMENT );
				segment->init();
				segment->setLayerCollision( m_layerCollision );

				this->addChild( segment );
			}

			m_movementTimer = 2 * GameState::getOneSecondInFrames();

			break;
		}

		case NPC_INIT_PARASITIC_WORM_SEGMENT:
		{
			break;
		}

		case NPC_INIT_SPIDER_CRAB:
		{
			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			if ( m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, 16 ) < 0 )
			//if ( m_layerCollision->Get( Pos.vx >> 4, Pos.vy >> 4 ) )
			{
				// starting off below ground, jump initially

				m_velocity = 5;

				m_movementFunc = NPC_MOVEMENT_SPIDER_CRAB_INITJUMP;
			}

			break;
		}

		default:

			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::reinit()
{
	m_animPlaying = true;
	m_animNo = m_data[m_type].initAnim;
	m_frame = 0;

	m_heading = m_fireHeading = 0;
	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;
	m_extension = 0;
	m_rotation = 0;
	m_reversed = false;
	m_salvoCount = 0;
	m_isActive = true;

	m_health = m_data[this->m_type].initHealth;

	m_extendDir = EXTEND_RIGHT;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;
	m_movementFunc = m_data[this->m_type].movementFunc;

	m_controlFunc = NPC_CONTROL_MOVEMENT;

	Pos = m_initPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::shutdown()
{
	if (m_spriteBank) m_spriteBank->dump();		delete m_spriteBank;
	// remove waypoints

	m_npcPath.removeAllWaypoints();

	// remove position history

	CNpcPositionHistory *currentPosition;
	CNpcPositionHistory *oldPosition;

	currentPosition = m_positionHistory;

	while( currentPosition )
	{
		oldPosition = currentPosition;
		currentPosition = currentPosition->next;

		delete oldPosition;
	}

	m_positionHistory = NULL;

	if (m_actorGfx)	delete m_actorGfx;

	CEnemyThing::shutdown();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcEnemy::getFrameCount()
{
	return( m_actorGfx->getFrameCount( m_animNo ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::think(int _frames)
{
	CEnemyThing::think(_frames);

	if ( m_isActive )
	{
		processGenericGetUserDist( _frames, &playerXDist, &playerYDist );
		playerXDistSqr = playerXDist * playerXDist;
		playerYDistSqr = playerYDist * playerYDist;

		if ( m_animPlaying )
		{
			s32 frameCount;

			frameCount = getFrameCount();

			s32 frameShift = ( _frames << 8 ) >> 1;

			if ( ( frameCount << 8 ) - m_frame > frameShift ) //( _frames >> 1 ) )
			{
				//m_frame += _frames >> 1;
				m_frame += frameShift;
			}
			else
			{
				m_frame = ( frameCount - 1 ) << 8;
				m_animPlaying = false;
			}
		}

		switch ( this->m_controlFunc )
		{
			case NPC_CONTROL_NONE:
				return;

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

		if ( m_heading > 1024 && m_heading < 3072 )
		{
			m_reversed = true;
		}
		else
		{
			m_reversed = false;
		}
	}

	processTimer(_frames);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				ATTACK_STATE playerState = player->getAttackState();

				switch( playerState )
				{
					case ATTACK_STATE__NONE:
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

									break;
								}

								case DETECT_ATTACK_COLLISION_GENERIC:
								{
									//if ( m_controlFunc == NPC_CONTROL_CLOSE && m_data[m_type].closeFunc != NPC_CLOSE_NONE )
									if ( m_controlFunc == NPC_CONTROL_CLOSE )
									{
										// only detect collision if in attack mode

										m_oldControlFunc = m_controlFunc;
										m_controlFunc = NPC_CONTROL_COLLISION;
									}

									break;
								}
							}
						}

						break;
					}

					default:
					{
						// player is attacking, respond appropriately

						if ( m_controlFunc != NPC_CONTROL_SHOT )
						{
							m_controlFunc = NPC_CONTROL_SHOT;
							m_state = NPC_GENERIC_HIT_CHECK_HEALTH;
						}

						break;
					}
				}

				break;
			}

			default:
				ASSERT(0);
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEnemy::processSensor()
{
	/*// temporary
	if ( playerXDistSqr + playerYDistSqr < 10000 )
	{
		m_controlFunc = NPC_CONTROL_SHOT;
		m_state = NPC_GENERIC_HIT_CHECK_HEALTH;

		return( true );
	}
	// temporary*/

	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				switch( m_sensorFunc )
				{
					case NPC_SENSOR_OIL_BLOB_USER_CLOSE:
					case NPC_SENSOR_NINJA_STARFISH_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 10000 )
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

					case NPC_SENSOR_GENERIC_USER_VISIBLE:
					{
						s32 xDistWaypoint, yDistWaypoint;

						if ( abs( playerXDist ) < 500 )
						{
							// within range

							// make sure user is closer to npc than next waypoint

							s32 xDistWaypoint, yDistWaypoint;

							m_npcPath.getDistToNextWaypoint( Pos, &xDistWaypoint, &yDistWaypoint );

							if ( abs( playerXDist ) < abs( xDistWaypoint ) )
							{
								s16 headingToPlayer = ratan2( playerYDist, playerXDist );

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
									// check if npc is facing user

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

					case NPC_SENSOR_PARASITIC_WORM_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 40000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_SKULL_STOMPER_USER_CLOSE:
					{
						if ( playerXDistSqr < 10000 && playerYDist >= 0 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extendDir = EXTEND_DOWN;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_BOOGER_MONSTER_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 400 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extendDir = EXTEND_UP;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_IRON_DOGFISH_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_FISH_HOOK_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 400 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_FALLING_ITEM_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 40000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_movementTimer = GameState::getOneSecondInFrames() * 3;

							return( true );
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processMovement(int _frames)
{
	//if ( _frames > 2 )
	//{
		//_frames = 2;
	//}

	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	switch( m_movementFunc )
	{
		case NPC_MOVEMENT_STATIC:
		{
			break;
		}

		case NPC_MOVEMENT_STATIC_CYCLE_ANIM:
		{
			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = m_data[m_type].initAnim;
				m_frame = 0;
			}

			break;
		}

		case NPC_MOVEMENT_FIXED_PATH:
		{
			processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = m_data[m_type].moveAnim;
				m_frame = 0;
			}

			break;
		}

		case NPC_MOVEMENT_FIXED_PATH_WALK:
		{
			processGenericFixedPathWalk( _frames, &moveX, &moveY );

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = m_data[m_type].moveAnim;
				m_frame = 0;
			}

			break;
		}

		case NPC_MOVEMENT_MOTHER_JELLYFISH:
		{
			processMotherJellyfishMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_FLYING_DUTCHMAN:
		{
			processFlyingDutchmanMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_PARASITIC_WORM:
		{
			processParasiticWormMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_SUB_SHARK:
		{
			processSubSharkMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_IRON_DOGFISH:
		{
			processIronDogfishMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_PENDULUM:
		{
			processPendulumMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_FIREBALL:
		{
			processFireballMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_RETURNING_HAZARD:
		{
			processReturningHazardMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_RETURNING_HAZARD_GROUND:
		{
			processReturningHazardMovementGround( _frames );

			break;
		}

		default:

			break;
	}

	processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processMovementModifier(int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange)
{
	Pos.vx += distX;
	Pos.vy += distY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::hasBeenAttacked()
{
	m_controlFunc = NPC_CONTROL_SHOT;
	m_state = NPC_GENERIC_HIT_CHECK_HEALTH;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processShot()
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
					}
					else
					{
						m_state = NPC_GENERIC_HIT_RECOIL;

						m_animPlaying = true;
						m_animNo = m_data[m_type].recoilAnim;
						m_frame = 0;
					}

					break;
				}

				case NPC_GENERIC_HIT_RECOIL:
				{
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

					break;
				}

				case NPC_GENERIC_HIT_DEATH_END:
				{
					if ( !m_animPlaying )
					{
						if ( m_data[m_type].respawning )
						{
							m_isActive = false;

							m_timerFunc = NPC_TIMER_RESPAWN;
							m_timerTimer = 4 * GameState::getOneSecondInFrames();
						}
						else
						{
							shutdown();
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

void CNpcEnemy::processClose(int _frames)
{
	switch( m_data[this->m_type].closeFunc )
	{
		case NPC_CLOSE_GENERIC_USER_SEEK:
		{
			processGenericGotoTarget( _frames, playerXDist, playerYDist, m_data[m_type].speed );

			break;
		}

		case NPC_CLOSE_SKULL_STOMPER_ATTACK:
			processCloseSkullStomperAttack( _frames );

			break;

		case NPC_CLOSE_BOOGER_MONSTER_ATTACK:
			processCloseBoogerMonsterAttack( _frames );

			break;

		case NPC_CLOSE_MOTHER_JELLYFISH_ATTACK:
			processCloseMotherJellyfishAttack( _frames );

			break;

		case NPC_CLOSE_FLYING_DUTCHMAN_ATTACK:
			processCloseFlyingDutchmanAttack( _frames );

			break;

		case NPC_CLOSE_SUB_SHARK_ATTACK:
			processCloseSubSharkAttack( _frames );

			break;

		case NPC_CLOSE_IRON_DOGFISH_ATTACK:
			processCloseIronDogfishAttack( _frames );

			break;

		case NPC_CLOSE_FALLING_ITEM_FALL:
			processCloseFallingItemFall( _frames );

			break;

		case NPC_CLOSE_FISH_HOOK_RISE:
			processCloseFishHookRise( _frames );

			break;

		case NPC_CLOSE_PARASITIC_WORM_ATTACK:
			processCloseParasiticWormAttack( _frames );

			break;

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processCollision()
{
	CPlayer *player = GameScene.getPlayer();

	player->takeDamage( m_data[m_type].damageToUserType );

	m_controlFunc = m_oldControlFunc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processTimer(int _frames)
{
	if ( m_timerTimer > 0 )
	{
		m_timerTimer -= _frames;
	}

	switch( m_timerFunc )
	{
		case NPC_TIMER_NONE:
			{
				break;
			}

		case NPC_TIMER_EVADE_DONE:
		case NPC_TIMER_ATTACK_DONE:
			{
				if ( m_timerTimer <= 0 )
				{
					m_timerFunc = NPC_TIMER_NONE;
					m_sensorFunc = m_data[this->m_type].sensorFunc;
				}

				break;
			}

		case NPC_TIMER_RESPAWN:
			{
				if ( m_timerTimer <= 0 )
				{
					reinit();
				}

				break;
			}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		// Render
		DVECTOR renderPos;
		DVECTOR	offset = CLevel::getCameraPos();

		renderPos.vx = Pos.vx - offset.vx;
		renderPos.vy = Pos.vy - offset.vy;

		if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
		{
			if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
			{
				SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
	switch( evt )
	{
		case USER_REQUEST_TALK_EVENT:
		{
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

				if ( xDiffSqr + yDiffSqr < 10000 )
				{
					if( !CConversation::isActive() )
					{
						CConversation::trigger( SCRIPTS_SPEECHTEST_DAT );
					}
				}
			}

			break;
		}

		case PROJECTILE_RETURNED_TO_SOURCE_EVENT:
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;

			//removeChild( sourceThing );
			//sourceThing->shutdown();
			//delete sourceThing;

			CProjectile *projectile;
			projectile = (CProjectile *) sourceThing;
			projectile->setMovementType( CProjectile::PROJECTILE_FIXED );
			projectile->setPosition( Pos );

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEnemy::canBeCaughtByNet()
{
	return( m_isActive && m_data[m_type].canBeNetted );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::caughtWithNet()
{
	if ( m_data[m_type].respawning )
	{
		if ( m_isActive )
		{
			m_isActive = false;

			m_timerFunc = NPC_TIMER_RESPAWN;
			m_timerTimer = 4 * GameState::getOneSecondInFrames();
		}
	}
	else
	{
		shutdown();
	}
}
