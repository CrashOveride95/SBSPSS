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

	m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &heading );

	if ( !pathComplete )
	{
		moveX = ( m_carSpeed >> 8 ) * _frames;

		if ( heading == 2048 )
		{
			moveX = -moveX;
		}
	}

	// check for vertical movement

	s32 checkDist = yMovement + 50;

	groundHeight = m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy, checkDist );

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
		m_carSpeed -= 20;

		if ( m_carSpeed < ( 2 << 8 ) )
		{
			m_carSpeed = ( 2 << 8 );
		}
	}
	else if ( moveY > 0 )
	{
		m_carSpeed += 20;

		if ( m_carSpeed > ( 6 << 8 ) )
		{
			m_carSpeed = ( 6 << 8 );
		}
	}

	Pos.vx += moveX;
	Pos.vy += moveY;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	u8 sensorDist = 16;

	s32 yDiff;

	yDiff = m_layerCollision->getHeightFromGround( testPos1.vx, testPos1.vy, sensorDist + 1 );

	if ( yDiff <= sensorDist )
	{
		// only use if there is ground present

		testPos1.vy += yDiff;
	}

	yDiff = m_layerCollision->getHeightFromGround( testPos2.vx, testPos2.vy, sensorDist + 1 );

	if ( yDiff <= sensorDist )
	{
		// only use if there is ground present

		testPos2.vy += yDiff;
	}

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	heading = ratan2( yDist, xDist );

	setCollisionAngle( heading );
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
