/*=========================================================================

	player.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\player.h"

#ifndef __ANIM_HEADER__
#include "gfx\anim.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __PLAYER__PSSTATES_H__
#include "player\pstates.h"
#endif

#ifndef __PLAYER__PSJUMP_H__
#include "player\psjump.h"
#endif

#ifndef __PLAYER__PSRUN_H__
#include "player\psrun.h"
#endif

#ifndef __PLAYER__PSFALL_H__
#include "player\psfall.h"
#endif

#ifndef __PLAYER__PSIDLE_H__
#include "player\psidle.h"
#endif

#ifndef __PLAYER__PSBUTT_H__
#include "player\psbutt.h"
#endif

#ifndef __PLAYER__PSCHOP_H__
#include "player\pschop.h"
#endif

#ifndef __PLAYER__PSDUCK_H__
#include "player\psduck.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

// to be removed
#include "gfx\tpage.h"


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_PLAYER_ANIM_HEADER__
#include <player_anim.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

CPlayerStateIdle			stateIdle;
CPlayerStateJump			stateJump;
CPlayerStateRun				stateRun;
CPlayerStateFall			stateFall;
CPlayerStateFallFar			stateFallFar;
CPlayerStateButtBounce		stateButtBounce;
CPlayerStateButtBounceFall	stateButtBounceFall;
CPlayerStateButtBounceLand	stateButtBounceLand;
CPlayerStateChop			stateChop;
CPlayerStateRunChop			stateRunChop;
CPlayerStateAirChop			stateAirChop;
CPlayerStateDuck			stateDuck;
CPlayerStateSoakUp			stateSoackUp;
CPlayerStateGetUp			stateGetup;

// Player with karate chop installed
CPlayer::PlayerMode CPlayer::s_modes=
{
	{	{
		8,						// PM__JUMP_VELOCITY
		10,						// PM__MAX_JUMP_FRAMES
		20,						// PM__MAX_SAFE_FALL_FRAMES
		4,						// PM__GRAVITY_VALUE
		8,						// PM__TERMINAL_VELOCITY
		8,						// PM__MAX_RUN_VELOCITY
		4,						// PM__RUN_SPEEDUP
		2,						// PM__RUN_REVERSESLOWDOWN
		1,						// PM__RUN_SLOWDOWN
	}	},
	{
		&stateIdle,				// STATE_IDLE
		&stateJump,				// STATE_JUMP
		&stateRun,				// STATE_RUN
		&stateFall,				// STATE_FALL
		&stateFallFar,			// STATE_FALLFAR
		&stateButtBounce,		// STATE_BUTTBOUNCE
		&stateButtBounceFall,	// STATE_BUTTFALL
		&stateButtBounceLand,	// STATE_BUTTLAND
		&stateChop,				// STATE_ATTACK
		&stateRunChop,			// STATE_RUNATTACK
		&stateAirChop,			// STATE_AIRATTACK
		&stateDuck,				// STATE_DUCK
		&stateSoackUp,			// STATE_SOAKUP
		&stateGetup,			// STATE_GETUP
	}
};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::init()
{
	CThing::init();
	
	sActor3dHdr	*Hdr=m_skel.Load(ACTORS_SPONGEBOB_A3D);		
	m_skel.Init(Hdr);
	TPLoadTex(ACTORS_SPONGEBOB_TEX);
	m_skel.setAnimDatabase(CAnimDB::GetPlayerAnimBank());

m_animNo=0;
m_animFrame=0;
	setState(STATE_IDLE);
	m_moveVel.vx=0;
	m_moveVel.vy=0;
	setFacing(FACING_RIGHT);

	m_lives=CGameSlotManager::getSlotData().m_lives;

	m_invincibleFrameCount=INVIBCIBLE_FRAMES__START;

#ifdef __USER_paul__
	Pos.vx=50;
	Pos.vy=200;
#else
	Pos.vx=10;
	Pos.vy=10;
#endif

	m_cameraOffsetTarget.vx=0;
	m_cameraOffsetTarget.vy=0;
	m_cameraOffset.vx=0;
	m_cameraOffset.vy=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::shutdown()
{
	CThing::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR ofs={0,0};
void	CPlayer::think(int _frames)
{
	int	i;
	
	CThing::think(_frames);

#ifndef __USER_paul__
	int	padInput=PadGetHeld(0);
	int	move=7*_frames;
	if(padInput&PAD_UP)		Pos.vy-=move;
	if(padInput&PAD_DOWN)	Pos.vy+=move;
	if(padInput&PAD_LEFT)	Pos.vx-=move;
	if(padInput&PAD_RIGHT)	Pos.vx+=move;
#else
	if(_frames>=3)_frames=2;

	for(i=0;i<_frames;i++)
	{
		// Think
		m_currentState->think(this);
		Pos.vx+=m_moveVel.vx>>VELOCITY_SHIFT;
		Pos.vy+=m_moveVel.vy>>VELOCITY_SHIFT;

		// Ground collision
		if(isOnSolidGround())
		{
			if(m_moveVel.vy)
			{
				// Was falling.. so we've just hit the ground
				if(m_state==STATE_BUTTFALL)
				{
					setState(STATE_BUTTLAND);
				}
				else if(m_state==STATE_FALLFAR)
				{
					setState(STATE_IDLE);
				}
				else if(m_moveVel.vx)
				{
					setState(STATE_RUN);
					setAnimNo(ANIM_PLAYER_ANIM_RUNJUMPEND);
				}
				else
				{
					setState(STATE_IDLE);
					setAnimNo(ANIM_PLAYER_ANIM_JUMPEND);
				}
				m_moveVel.vy=0;
				m_fallFrames=0;
			}
		}
		else
		{
			if(m_state!=STATE_JUMP&&m_state!=STATE_BUTTBOUNCE)
			{
				// Fall
				const PlayerMetrics	*metrics;
				metrics=getPlayerMetrics();
				m_moveVel.vy+=metrics->m_metric[PM__GRAVITY_VALUE];
				if(m_moveVel.vy>=metrics->m_metric[PM__TERMINAL_VELOCITY]<<VELOCITY_SHIFT)
				{
					m_moveVel.vy=metrics->m_metric[PM__TERMINAL_VELOCITY]<<VELOCITY_SHIFT;
					m_fallFrames++;
					if(m_fallFrames>metrics->m_metric[PM__MAX_SAFE_FALL_FRAMES])
					{
						setState(STATE_FALLFAR);
					}
				}
			}
		}

		// Flashing..
		if(m_invincibleFrameCount)
		{
			m_invincibleFrameCount--;
		}
	}


#endif
m_invincibleFrameCount=0;
	// Move the camera offset
	for(i=0;i<_frames;i++)
	{
		m_cameraOffsetTarget=ofs;		
		m_cameraOffset=ofs;
		/*
		int moveDelta;
		moveDelta=(m_cameraOffset.vx-m_cameraOffsetTarget.vx);
		if(moveDelta)
		{
		if(moveDelta<0)
		{
					moveDelta>>=2;
					if(moveDelta==0)moveDelta=1;
					}
					else if(moveDelta>0)
					{
					moveDelta>>=2;
					if(moveDelta==0)moveDelta=-1;
					}
					m_cameraOffset.vx+=moveDelta;
					}
		*/
	}
	if(Pos.vx<0)Pos.vx=0;
	if(Pos.vy<0)Pos.vy=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int panim=-1;
SVECTOR ppos={0,1024,5000};
void	CPlayer::render()
{
	CThing::render();
	
	// Render
	if(m_invincibleFrameCount==0||m_invincibleFrameCount&2)
	{
		m_skel.setPos(&ppos);
		m_skel.setFrame(m_animFrame);
		if(panim!=-1)
			m_skel.setAnimNo(panim);
		else
			m_skel.setAnimNo(m_animNo);
		m_skel.Animate(this);
		m_skel.Render(this);
	}
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR CPlayer::getCameraPos()
{
	DVECTOR	cameraPos;
	cameraPos.vx=Pos.vx;//+m_cameraOffset.vx;
	cameraPos.vy=Pos.vy;//+m_cameraOffset.vy;
	return cameraPos;
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const PlayerMetrics *CPlayer::getPlayerMetrics()
{
	return &s_modes.m_metrics;
}





/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::setState(PLAYER_STATE _state)
{
	m_currentState=s_modes.m_states[_state];
	m_currentState->enter(this);
	m_state=_state;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getFacing()
{
	  return m_facing;
}
void CPlayer::setFacing(int _facing)
{
	if(m_facing!=_facing)
	{
		switch(_facing)
		{
			case FACING_LEFT:
				m_facing=FACING_LEFT;
				m_skel.setAng(1024);
				break;
			case FACING_RIGHT:
				m_facing=FACING_RIGHT;
				m_skel.setAng(-1024);
				break;
			default:
				ASSERT(0);
				break;
		}
	}
}
int CPlayer::getAnimFrame()
{
	return m_animFrame;
}
void CPlayer::setAnimFrame(int _animFrame)
{
	m_animFrame=_animFrame;
}
int CPlayer::getAnimFrameCount()
{
	return m_skel.getFrameCount();
}
int CPlayer::getAnimNo()
{
	return m_animNo;
}
void CPlayer::setAnimNo(int _animNo)
{
	m_animNo=_animNo;
	m_animFrame=0;
}
DVECTOR CPlayer::getMoveVelocity()
{
	return m_moveVel;
}
void CPlayer::setMoveVelocity(DVECTOR *_moveVel)
{
	m_moveVel=*_moveVel;
}
int CPlayer::getPadInput()
{
	return PadGetHeld(0);
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */







int CPlayer::isOnSolidGround()
{
	return Pos.vy>16*15;
}




void CPlayer::moveLeft()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();

	setFacing(FACING_LEFT);
	if(m_moveVel.vx<=0)
	{
		m_moveVel.vx-=metrics->m_metric[PM__RUN_SPEEDUP];
		if(m_moveVel.vx<-metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT)
		{
			m_moveVel.vx=-metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT;
		}
	}
	else
	{
		m_moveVel.vx-=metrics->m_metric[PM__RUN_REVERSESLOWDOWN];
	}
}
void CPlayer::moveRight()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();
	
	setFacing(FACING_RIGHT);
	if(m_moveVel.vx>=0)
	{
		m_moveVel.vx+=metrics->m_metric[PM__RUN_SPEEDUP];
		if(m_moveVel.vx>metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT)
		{
			m_moveVel.vx=metrics->m_metric[PM__MAX_RUN_VELOCITY]<<VELOCITY_SHIFT;
		}
	}
	else
	{
		m_moveVel.vx+=metrics->m_metric[PM__RUN_REVERSESLOWDOWN];
	}
}
void CPlayer::slowdown()
{
	const PlayerMetrics	*metrics;
	metrics=getPlayerMetrics();
	
	if(m_moveVel.vx<0)
	{
		m_moveVel.vx+=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(m_moveVel.vx>0)m_moveVel.vx=0;
	}
	else if(m_moveVel.vx>0)
	{
		m_moveVel.vx-=metrics->m_metric[PM__RUN_SLOWDOWN];
		if(m_moveVel.vx<0)m_moveVel.vx=0;
	}
}

void CPlayer::jump()
{
}
void CPlayer::fall()
{
}


/*===========================================================================
end */
