/*=========================================================================

	pcart.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCART_H__
#include "platform\pcart.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_carSpeed = m_speed << 8;
	m_isActivated = false;

	sBBox boundingBox = m_modelGfx->GetBBox();
	boundingBox.YMin = boundingBox.YMax - 32;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
	calculateNonRotatedCollisionData();

	m_playerAttached = false;
	m_falling = false;
	m_rebound = false;
	m_reboundTimer = 0;
	m_inJump = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::processMovement( int _frames )
{
	s32 fallSpeed = 2;
	s8 yMovement = fallSpeed * _frames;
	s32 distX, distY, heading;
	s32 groundHeight;
	s32 moveX = 0;
	s32 moveY = 0;

	bool pathComplete;

	if ( m_reboundTimer > 0 )
	{
		m_reboundTimer -= _frames;
	}

	if ( !m_playerAttached && !m_falling )
	{
		m_playerAttached = true;
		CPlayer *player = GameScene.getPlayer();

		DVECTOR newPos = Pos;
		CRECT collisionArea=getCollisionArea();
		newPos.vy = collisionArea.y1;

		player->setPos( newPos );
		player->setPlatform( this );
		m_contact = true;
	}

	if ( m_isActivated )
	{
		if ( m_falling )
		{
			m_vertSpeed += 192;

			if ( m_vertSpeed > ( 8 << 8 ) )
			{
				m_vertSpeed = 8 << 8;
			}

			moveY = ( m_vertSpeed >> 8 ) * _frames;

			Pos.vy += moveY;

			DVECTOR	offset = CLevel::getCameraPos();

			s32 yPos = Pos.vy - offset.vy;

			if ( yPos < 0 || yPos > VidGetScrH() )
			{
				setToShutdown();
			}
		}
		else if ( m_rebound )
		{
			moveX = -4 * _frames;

			m_vertSpeed += 192;

			if ( m_vertSpeed > ( 8 << 8 ) )
			{
				m_vertSpeed = 8 << 8;
			}
			else if ( m_vertSpeed < -( 6 << 8 ) )
			{
				m_vertSpeed = -( 6 << 8 );
			}

			moveY = ( m_vertSpeed >> 8 ) * _frames;

			groundHeight = CGameScene::getCollision()->getHeightFromGroundCart( Pos.vx + moveX, Pos.vy + moveY, 16 );

			if ( groundHeight < 0 )
			{
				// have touched down

				m_rebound = false;
				moveY += groundHeight;
			}

			Pos.vx += moveX;
			Pos.vy += moveY;
		}
		else
		{
			m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &heading );

			if ( !pathComplete )
			{
				moveX = ( m_carSpeed >> 8 ) * _frames;

				if ( heading == 2048 )
				{
					moveX = -moveX;
				}
			}

			if ( m_inJump )
			{
				m_vertSpeed += 192;

				if ( m_vertSpeed > ( 5 << 8 ) )
				{
					m_vertSpeed = 5 << 8;
				}
				else if ( m_vertSpeed < -( 6 << 8 ) )
				{
					m_vertSpeed = -( 6 << 8 );
				}

				moveY = ( m_vertSpeed >> 8 ) * _frames;

				groundHeight = CGameScene::getCollision()->getHeightFromGroundCart( Pos.vx + moveX, Pos.vy, moveY + 16 );

				if ( groundHeight < moveY )
				{
					// have touched down

					m_inJump = false;
					moveY += groundHeight;
				}
			}
			else
			{
				// check for vertical movement

				s32 checkDist = yMovement + 50;

				groundHeight = CGameScene::getCollision()->getHeightFromGroundCart( Pos.vx + moveX, Pos.vy, checkDist );

				if ( groundHeight < checkDist )
				{
					// groundHeight <= yMovement indicates either just above ground or on or below ground

					moveY = groundHeight;
				}
				else
				{
					// fall

					moveY = yMovement;
				}

				if ( moveY < 0 )
				{
					m_carSpeed -= 1;

					if ( m_carSpeed < ( 2 << 8 ) )
					{
						m_carSpeed = ( 2 << 8 );
					}
				}
				else if ( moveY > 0 )
				{
					m_carSpeed += 20;

					if ( m_carSpeed > ( 5 << 8 ) )
					{
						m_carSpeed = ( 5 << 8 );
					}
				}
			}

			Pos.vx += moveX;
			Pos.vy += moveY;
		}

		// sort out draw rotation

		DVECTOR testPos1, testPos2;

		testPos1 = testPos2 = Pos;
		testPos1.vx -= 10;
		testPos2.vx += 10;

		u8 sensorDist = 16;

		s32 yDiff;

		yDiff = CGameScene::getCollision()->getHeightFromGroundCart( testPos1.vx, testPos1.vy, sensorDist + 1 );

		if ( yDiff <= sensorDist )
		{
			// only use if there is ground present

			testPos1.vy += yDiff;
		}

		yDiff = CGameScene::getCollision()->getHeightFromGroundCart( testPos2.vx, testPos2.vy, sensorDist + 1 );

		if ( yDiff <= sensorDist )
		{
			// only use if there is ground present

			testPos2.vy += yDiff;
		}

		s32 xDist = testPos2.vx - testPos1.vx;
		s32 yDist = testPos2.vy - testPos1.vy;

		heading = ratan2( yDist, xDist );

		setCollisionAngle( heading );

		switch ( CGameScene::getCollision()->getCollisionBlock( testPos2.vx, testPos2.vy - 8 ) & COLLISION_TYPE_MASK )
		{
			case COLLISION_TYPE_FLAG_DAMAGE:
			{
				if ( m_reboundTimer <= 0 )
				{
					m_vertSpeed = -8 << 8;
					m_reboundTimer = 2 * GameState::getOneSecondInFrames();
					m_rebound = true;
					Pos.vy -= 8;
				}

				break;
			}

			case COLLISION_TYPE_FLAG_DEATH_FALL:
			{
				m_playerAttached = false;
				m_falling = true;

				break;
			}

			default:
				break;
		}
	}
	else
	{
		groundHeight = CGameScene::getCollision()->getHeightFromGroundCart( Pos.vx, Pos.vy, yMovement + 16 );

		if ( groundHeight <= yMovement )
		{
			moveY = groundHeight;
		}
		else
		{
			// fall

			moveY = yMovement;
		}

		Pos.vy += moveY;

		if ( m_contact )
		{
			m_isActivated = true;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			SVECTOR rotation;

			rotation.vx = 0;
			rotation.vy = 0;
			rotation.vz = getCollisionAngle();

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::jump()
{
	if ( !m_inJump )
	{
		m_inJump = true;
		m_vertSpeed = -8 << 8;
		CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_JUMP);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::collidedWith( CThing *_thisThing )
{
	if ( !m_falling )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player;
				DVECTOR	playerPos;
				CRECT	collisionArea;
				CRECT	playerCollisionArea;

				// Only interested in SBs feet colliding with the box (pkg)
				player=(CPlayer*)_thisThing;
				playerPos=player->getPos();
				playerCollisionArea = player->getCollisionArea();
				collisionArea=getCollisionArea();

				s32 threshold = abs( collisionArea.y2 - collisionArea.y1 );

				if ( threshold > 16 )
				{
					threshold = 16;
				}

				//if( playerPos.vx >= collisionArea.x1 && playerPos.vx <= collisionArea.x2 )
				if( playerCollisionArea.x2 >= collisionArea.x1 && playerCollisionArea.x1 <= collisionArea.x2 )
				{
					if ( checkCollisionDelta( _thisThing, threshold, collisionArea ) )
					{
						player->setPlatform( this );

						m_contact = true;
					}
					else
					{
						if( playerPos.vy >= collisionArea.y1 && playerPos.vy <= collisionArea.y2 )
						{
							if ( m_isActivated || player->getPosDelta().vy >= 0 )
							{
								int height = getHeightFromPlatformAtPosition( playerPos.vx, playerPos.vy );

								if ( height >= -threshold && height < 1 )
								{
									player->setPlatform( this );

									m_contact = true;
								}
							}
						}
					}
				}

				break;
			}

			case TYPE_NPC:
			case TYPE_HAZARD:
				break;

			default:
				ASSERT(0);
				break;
		}
	}
}
