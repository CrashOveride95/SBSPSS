/*=========================================================================

	hcheck.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HCHECK_H__
#include "hazard\hcheck.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::init()
{
	CNpcHazard::init();

	m_triggered = false;
	m_spriteFrame = FRM__CHECKPOINT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::render()
{
	sFrameHdr	*frameHdr;

	if ( m_isActive )
	{
		CHazardThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);

			if ( m_triggered )
			{
				int		x,y;

				DVECTOR offset = CLevel::getCameraPos();

				int		spriteWidth = CGameScene::getSpriteBank()->getFrameWidth(m_spriteFrame);
				int		spriteHeight = CGameScene::getSpriteBank()->getFrameHeight(m_spriteFrame);

				x = Pos.vx - offset.vx - ( spriteWidth >> 1 );
				y = Pos.vy - 100 - offset.vy - ( spriteHeight >> 1 );

				frameHdr = CGameScene::getSpriteBank()->getFrameHeader( m_spriteFrame );
				CGameScene::getSpriteBank()->printFT4( frameHdr, x, y, 0, 0, 10 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCheckpointHazard::collidedWith(CThing *_thisThing)
{
	if ( m_isActive && !m_triggered )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CRECT collisionArea=getCollisionArea();
				DVECTOR respawnPos;
				respawnPos.vx=collisionArea.x1+((collisionArea.x2-collisionArea.x1)/2);
				respawnPos.vy=collisionArea.y2;
				((CPlayer*)_thisThing)->setRespawnPosAndRingTelephone(respawnPos);
				m_triggered = true;

				break;
			}

			default:
				break;
		}
	}
}
