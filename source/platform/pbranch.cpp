/*=========================================================================

	pbranch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBRANCH_H__
#include "platform\pbranch.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::postInit()
{
	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ) << 1, PLATFORMCOLLISIONHEIGHT );

	if ( m_reversed )
	{
		setCollisionCentreOffset( boundingBox.XMax, 18 + ( ( boundingBox.YMax + boundingBox.YMin ) >> 1 ) );
	}
	else
	{
		setCollisionCentreOffset( boundingBox.XMin, 18 + ( ( boundingBox.YMax + boundingBox.YMin ) >> 1 ) );
	}

	m_angularVelocity = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 initXPos, newXPos, newYPos;

	initXPos = newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	init( startPos );

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;

		if ( newXPos < initXPos )
		{
			m_reversed = true;
		}
		else
		{
			m_reversed = false;
		}
	}
	else
	{
		m_reversed = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			if ( m_detectCollision && m_isActive )
			{
				CPlayer *player;
				DVECTOR	playerPos;
				CRECT	collisionArea;

				// Only interested in SBs feet colliding with the box (pkg)
				player=(CPlayer*)_thisThing;
				playerPos=player->getPos();
				collisionArea=getCollisionArea();
				if(playerPos.vx>=collisionArea.x1&&playerPos.vx<=collisionArea.x2&&
				   playerPos.vy>=collisionArea.y1&&playerPos.vy<=collisionArea.y2)
				{
					if ( ( m_reversed && playerPos.vx <= Pos.vx ) || ( !m_reversed && playerPos.vx >= Pos.vx ) )
					{
						player->setPlatform( this );

						if(getHeightFromPlatformAtPosition(playerPos.vx,playerPos.vy)==0)
						{
							m_contact = true;
						}
					}
				}
			}

			break;
		}

		case TYPE_HAZARD:
			break;

		default:
			ASSERT(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::processMovement( int _frames )
{
	s16 newAngle = getCollisionAngle();

	if ( m_contact )
	{
		//if ( ( m_reversed && newAngle < -256 ) || newAngle > 256 )
		//{
			// flick player upwards
			//GameScene.getPlayer()->springPlayerUp();
		//}

		CPlayer *player = GameScene.getPlayer();

		if ( m_reversed )
		{
			if ( m_angularVelocity > 8 && newAngle < -20 )
			{
				player->springPlayerUp();
			}
		}
		else
		{
			if ( m_angularVelocity < -8 && newAngle > 20 )
			{
				player->springPlayerUp();
			}
		}

		s16 angularForce = 3 * _frames;

		if ( m_reversed )
		{
			angularForce = -angularForce;
		}

		m_angularVelocity += angularForce;
	}
	else if ( ( getRnd() % 50 ) == 0 )
	{
		s16 angularForce = 6 * _frames;

		if ( m_reversed )
		{
			angularForce = -angularForce;
		}

		m_angularVelocity += angularForce;
	}

	s32 resistance = -( 10 * _frames * newAngle ) >> 8;

	if ( newAngle > 0 && resistance > -2 )
	{
		resistance = -2;
	}
	else if ( newAngle < 0 && resistance < 2 )
	{
		resistance = 2;
	}

	// get direction of resistance

	m_angularVelocity += resistance;

	newAngle += m_angularVelocity;

	if ( m_angularVelocity )
	{
		m_angularVelocity += -m_angularVelocity / abs( m_angularVelocity );

		if ( m_angularVelocity > 40 )
		{
			m_angularVelocity = 40;
		}
		else if ( m_angularVelocity < -40 )
		{
			m_angularVelocity = -40;
		}
	}

	/*if ( newAngle > 320 )
	{
		newAngle = 320;
	}
	else if ( newAngle < -320 )
	{
		newAngle = -320;
	}*/

	setCollisionAngle( newAngle );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int	BX=-16;
int	BY=+18;
void CNpcBranchPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			SVECTOR rotation;
			rotation.vx = 0;
			if ( m_reversed )
			{
				rotation.vy = 0;
				rotation.vz = getCollisionAngle();
				renderPos.vx-= BX;
				renderPos.vy+= BY;
			}
			else
			{
				rotation.vy = 0;
				rotation.vz = getCollisionAngle();
				renderPos.vx+= BX;
				renderPos.vy+= BY;
			}

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		
#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR	offset = CLevel::getCameraPos();
	DVECTOR size;
	DVECTOR	centre;
	int		halfLength;
	int		x1,y1,x2,y2;

	centre=getCollisionCentre();
	size=getCollisionSize();
	halfLength=size.vx>>1;

	x1=0;
	x2=0;
	y1=0;
	y2=0;

	if ( m_reversed )
	{
		x1=-halfLength*mcos(getCollisionAngle()&4095)>>12;
		y1=-halfLength*msin(getCollisionAngle()&4095)>>12;
	}
	else
	{
		x2=+halfLength*mcos(getCollisionAngle()&4095)>>12;
		y2=+halfLength*msin(getCollisionAngle()&4095)>>12;
	}

	centre.vx-=offset.vx;
	centre.vy-=offset.vy;
	x1+=centre.vx;
	y1+=centre.vy;
	x2+=centre.vx;
	y2+=centre.vy;

	DrawLine(x1,y1,x2,y2,0,255,0,0);
#endif

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::calculateBoundingBoxSize()
{
	int		angle;
	DVECTOR	centre;
	int		length;
	int		x1,y1,x2,y2;

	angle=getCollisionAngle();
	centre=getCollisionCentre();

	//halfLength=m_platformWidth/2;
	sBBox boundingBox = m_modelGfx->GetBBox();
	length = ( boundingBox.XMax - boundingBox.XMin );

	x1=-length*mcos(angle&4095)>>12;
	y1=-length*msin(angle&4095)>>12;
	x2=+length*mcos(angle&4095)>>12;
	y2=+length*msin(angle&4095)>>12;

	setCollisionSize(abs(x2-x1),abs(y2-y1)+PLATFORMCOLLISIONHEIGHT);
}
