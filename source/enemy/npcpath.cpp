/*=========================================================================

	npcpath.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPCPATH_H__
#include "enemy\npcpath.h"
#endif

bool CNpcWaypoint::isPointNear( DVECTOR testPos, s32 *xDist, s32 *yDist )
{
	s32 xDistSqr, yDistSqr;

	*xDist = this->pos.vx - testPos.vx;
	xDistSqr = (*xDist) * (*xDist);

	*yDist = this->pos.vy - testPos.vy;
	yDistSqr = (*yDist) * (*yDist);

	if ( xDistSqr + yDistSqr < 100 )
	{
		return( true );
	}
	else
	{
		return( false );
	}
}

void CNpcPath::initPath()
{
	int loop;

	for ( loop = 0 ; loop < NPC_MAX_WAYPOINTS ; loop++ )
	{
		waypoint[loop].pos.vx = 0;
		waypoint[loop].pos.vy = 0;
	}

	pathType = SINGLE_USE_PATH;
	currentWaypoint = lastWaypoint = 0;
	waypointCount = 0;
	reversePath = false;
}

void CNpcPath::addWaypoint( DVECTOR newPos )
{
	if ( waypointCount < NPC_MAX_WAYPOINTS )
	{
		waypoint[waypointCount].pos = newPos;
		waypointCount++;
	}
}

void CNpcPath::setPathType( NPC_PATH_TYPE newPathType )
{
	pathType = newPathType;
}

bool CNpcPath::incPath()
{
	if ( !reversePath )
	{
		if ( currentWaypoint < waypointCount - 1 )
		{
			currentWaypoint++;
		}
		else
		{
			switch( pathType )
			{
				case SINGLE_USE_PATH:
					// path is completed

					return( true );

				case REPEATING_PATH:
					// go back to start

					currentWaypoint = 0;

					break;

				case PONG_PATH:
					// reverse path

					reversePath = !reversePath;
					currentWaypoint--;

					break;
			}
		}
	}
	else
	{
		// must be pong path if reversed

		if ( currentWaypoint > 0 )
		{
			currentWaypoint--;
		}
		else
		{
			reversePath = !reversePath;
			currentWaypoint++;
		}
	}

	return( false );
}

bool CNpcPath::getDistToNextWaypoint( DVECTOR currentPos, s32 *distX, s32 *distY )
{
	return( waypoint[currentWaypoint].isPointNear( currentPos, distX, distY ) );
}

s32 CNpcPath::think( DVECTOR currentPos, bool *pathComplete, bool *waypointChange )
{
	s32 xDist, yDist;

	*pathComplete = false;
	*waypointChange = false;

	if ( waypoint[currentWaypoint].isPointNear( currentPos, &xDist, &yDist ) )
	{
		*pathComplete = incPath();
		*waypointChange = true;
	}

	s32 headingToTarget = ratan2( yDist, xDist );

	return( headingToTarget );
}