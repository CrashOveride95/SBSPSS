/*=========================================================================

	ppendulm.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PPENDULM_H__
#include "platform\ppendulm.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_extendDir = EXTEND_LEFT;
	m_extension = 0;
	m_heading = 1024;
	m_lineBase.vx = Pos.vx;
	m_lineBase.vy = 0;

	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, boundingBox.YMin + 15 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		DVECTOR pivotPos;
		pivotPos.vx = newXPos << 4;
		pivotPos.vy = newYPos << 4;

		s32 xDist = startPos.vx - pivotPos.vx;
		s32 yDist = startPos.vy - pivotPos.vy;

		m_maxExtension = 1024 - ratan2( abs( yDist ), abs( xDist ) );

		m_length = isqrt2( ( xDist * xDist ) + ( yDist * yDist ) );

		init( pivotPos );
	}
	else
	{
		init( startPos );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::processMovement( int _frames )
{
	if ( m_extendDir == EXTEND_LEFT )
	{
		if ( m_extension > m_maxExtension )
		{
			m_extendDir = EXTEND_RIGHT;
		}
		else
		{
			m_extension += _frames << 3;
		}
	}
	else
	{
		if ( m_extension < -m_maxExtension )
		{
			m_extendDir = EXTEND_LEFT;
		}
		else
		{
			m_extension -= _frames << 3;
		}
	}

	Pos.vx = m_base.vx + ( ( m_length * rcos( m_heading + m_extension ) ) >> 12 );
	Pos.vy = m_base.vy + ( ( m_length * rsin( m_heading + m_extension ) ) >> 12 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::render()
{
	int		x1,y1,x2,y2;
	int		x1Boundary,y1Boundary,x2Boundary,y2Boundary;

	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);

#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR size;
	DVECTOR	centre;
	int		halfLength;

	centre=getCollisionCentre();
	size=getCollisionSize();
	halfLength=size.vx>>1;

	x1=-halfLength*mcos(getCollisionAngle()&4095)>>12;
	y1=-halfLength*msin(getCollisionAngle()&4095)>>12;
	x2=+halfLength*mcos(getCollisionAngle()&4095)>>12;
	y2=+halfLength*msin(getCollisionAngle()&4095)>>12;

	centre.vx-=offset.vx;
	centre.vy-=offset.vy;
	x1+=centre.vx;
	y1+=centre.vy;
	x2+=centre.vx;
	y2+=centre.vy;

	DrawLine(x1,y1,x2,y2,0,255,0,0);
#endif
/*
			x1 = x1Boundary = Pos.vx - offset.vx;
			x2 = x2Boundary = m_lineBase.vx - offset.vx;

			y1 = y1Boundary = Pos.vy - offset.vy;
			y2 = y2Boundary = m_lineBase.vy - offset.vy;

			int angle = ratan2( x1 - x2, y1 - y2 );

			if ( y2 < 0 )
			{
				int yDiff = -y2;
				y2 = y2Boundary = 0;
				int hyp = ( yDiff << 12 ) / rcos( angle );
				x2 += ( hyp * rsin( angle ) ) >> 12;
			}
			
			if ( y1 > VidGetScrH() )
			{
				int yDiff = y1 - VidGetScrH();
				y1 = y1Boundary = VidGetScrH();
				int hyp = ( yDiff << 12 ) / rcos( angle );
				x1 -= ( hyp * rsin( angle ) ) >> 12;
			}

			if ( x1Boundary > x2Boundary )
			{
				int tempX = x1Boundary;
				x1Boundary = x2Boundary;
				x2Boundary = tempX;
			}

			if ( y1Boundary > y2Boundary )
			{
				int tempY = y1Boundary;
				y1Boundary = y2Boundary;
				y2Boundary = tempY;
			}

			if ( x2Boundary >= 0 && x1Boundary <= VidGetScrW() )
			{
				if ( y2Boundary >= 0 && y1Boundary <= VidGetScrH() )
				{
					DrawLine( x1, y1, x2, y2, 0, 0, 0, 0 );
				}
			}
*/
		}
	}
}
