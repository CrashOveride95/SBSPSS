/*=========================================================================

	nsstomp.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif


void CNpc::processCloseSkullStomperAttack( int _frames )
{
	if ( m_timerTimer > 0 )
	{
		// wait
	}
	else
	{
		if ( m_extendDir == EXTEND_DOWN )
		{
			if ( m_layerCollision->Get( Pos.vx >> 4, ( Pos.vy + ( m_data[m_type].speed * _frames ) ) >> 4 ) )
			{
				// colliding with ground

				s32 distY;
				s32 lastPointY = 0;

				for ( distY = 1 ; distY <= ( m_data[m_type].speed * _frames ) ; distY++ )
				{
					if ( m_layerCollision->Get( Pos.vx >> 4, ( Pos.vy + distY ) >> 4 ) )
					{
						break;
					}
					else
					{
						lastPointY++;
					}
				}

				Pos.vy += lastPointY;

				// pause and change direction

				m_timerTimer = GameState::getOneSecondInFrames();
				m_extendDir = EXTEND_UP;
			}
			else
			{
				// drop down

				Pos.vy += m_data[m_type].speed * _frames;
			}
		}
		else
		{
			if ( m_base.vx - Pos.vx == 0 && m_base.vy - Pos.vy == 0 )
			{
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;
				m_npcPath.resetPath();
			}
			else
			{
				// return to original position

				processGenericGotoTarget( _frames, m_base.vx - Pos.vx, m_base.vy - Pos.vy, 1 );
			}
		}
	}
}