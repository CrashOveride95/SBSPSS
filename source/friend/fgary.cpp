/*=========================================================================

	fgary.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FGARY_H__
#include "friend\fgary.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::postInit()
{
	CNpcFriend::postInit();

	m_started = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::think( int _frames )
{
	CNpcFriend::think(_frames);

	s8 multiplier = -1 + ( 2 * m_extension );
	s32 maxHeight = 20;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s8 groundHeight;

	// check vertical collision

	groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

	if ( m_platform )
	{
		s32 platformHeight = m_platform->getHeightFromPlatformAtPosition( Pos.vx, Pos.vy );

		if ( platformHeight < groundHeight )
		{
			groundHeight = platformHeight;
		}

		//Pos.vy += platformHeight;
		//return;
	}

	if ( groundHeight <= 0 )
	{
		// groundHeight <= 0  indicates either on ground or below ground

		// check horizontal collision

		if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( multiplier * _frames ), Pos.vy ) < -maxHeight )
		{
			// reverse direction

			m_extension = !m_extension;
			m_reversed = !m_reversed;
		}
		else
		{
			// make sure we are on the ground, not below it

			Pos.vy += groundHeight;

			if ( m_started )
			{
				CSoundMediator::playSfx( CSoundMediator::SFX_GARY_DE_SNAIL );

				Pos.vx += multiplier * _frames;
			}
		}
	}
	else
	{
		// above ground

		if ( groundHeight < yMovement )
		{
			// colliding with ground

			Pos.vy += groundHeight;

			if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( multiplier * _frames ), Pos.vy ) < -maxHeight )
			{
				// reverse direction

				m_extension = !m_extension;
			}
			else
			{
				if ( m_started )
				{
					CSoundMediator::playSfx( CSoundMediator::SFX_GARY_DE_SNAIL );

					Pos.vx += multiplier * _frames;
				}
			}
		}
		else
		{
			Pos.vy += yMovement;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::setupWaypoints( sThingActor *ThisActor )
{
	if ( ThisActor->PointCount > 1 )
	{
		u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

		u16 newXPos, newYPos;

		// skip first waypoint
		
		PntList++;
		PntList++;

		// get trigger position

		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		m_triggerPos.vx = newXPos;
		m_triggerPos.vy = newYPos;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::render()
{
	CNpcThing::render();

// Render

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		m_actorGfx->Render(renderPos,m_animNo,(m_frame>>8),m_reversed);

		sBBox boundingBox = m_actorGfx->GetBBox();
		boundingBox.YMax = 0;
		setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
		setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::startLeft()
{
	start();

	m_extension = EXTEND_LEFT;
	m_reversed = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGaryFriend::startRight()
{
	start();

	m_extension = EXTEND_RIGHT;
	m_reversed = false;
}
