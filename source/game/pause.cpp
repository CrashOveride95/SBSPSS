/*=========================================================================

	pause.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "game\pause.h"

#ifndef	__GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef	__GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static const int	FRAME_WIDTH		=352;
static const int	FRAME_HEIGHT	=160;
static const int	TEXT_BOX_WIDTH	=300;
static const int	TEXT_BOX_HEIGHT	=20;
static const int	OT_POS			=5;
static const int	TEXT_SPACING	=13;
extern int newmode;

extern int invincibleSponge;
int	inv_data[]={0,1,-1};
static CGUITextReadout::TextReadoutData inv_readoutdata[2]={{0,STR__OFF},{1,STR__ON}};

void CPauseMenu::init()
{
	m_guiFrame=new ("Conversation GUI") CGUIControlFrame();
	m_guiFrame->init(0);
	m_guiFrame->setObjectXYWH((512-FRAME_WIDTH)/2,(256-FRAME_HEIGHT)/2,FRAME_WIDTH,FRAME_HEIGHT);
	m_guiFrame->setOt(OT_POS);
	m_guiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);

	int xpos=TEXT_SPACING/2;

#ifdef __USER_paul__
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__CONTINUE,
										&m_exitPauseMenuFlag,true);
	xpos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__QUIT,
										&m_quitGameFlag,true);
	xpos+=TEXT_SPACING+(TEXT_SPACING/2);

	
	CGUIFactory::createCycleButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__INVINCIBILE_SPONGEBOB,
										&invincibleSponge,inv_data,inv_readoutdata);
	xpos+=TEXT_SPACING+(TEXT_SPACING/2);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BASICUNARMED_MODE,
										&newmode,PLAYER_MODE_BASICUNARMED);
	xpos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__FULLUNARMED_MODE,
										&newmode,PLAYER_MODE_FULLUNARMED);
	xpos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BALLOON_MODE,
										&newmode,PLAYER_MODE_BALLOON);
	xpos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BUBBLEMIXTURE_MODE,
										&newmode,PLAYER_MODE_BUBBLE_MIXTURE);
	xpos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__NET_MODE,
										&newmode,PLAYER_MODE_NET);
	xpos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__CORALBLOWER_MODE,
										&newmode,PLAYER_MODE_CORALBLOWER);
	xpos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__JELLYLAUNCHER_MODE,
										&newmode,PLAYER_MODE_JELLY_LAUNCHER);
	xpos+=TEXT_SPACING;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__DEAD_MODE,
										&newmode,PLAYER_MODE_DEAD);
	xpos+=TEXT_SPACING;
#else
	xpos+=TEXT_SPACING*4;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__CONTINUE,
										&m_exitPauseMenuFlag,true);
	xpos+=TEXT_SPACING*2;
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__QUIT,
										&m_quitGameFlag,true);
	xpos+=TEXT_SPACING*2;

	
	CGUIFactory::createCycleButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,xpos,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__INVINCIBILE_SPONGEBOB,
										&invincibleSponge,inv_data,inv_readoutdata);
	xpos+=TEXT_SPACING*2;
#endif

	m_guiSpatReadout=new ("SpatReadout") CGUISpatCountReadout();
	m_guiSpatReadout->init(m_guiFrame);
	m_guiSpatReadout->setObjectXYWH(10,10,120,40);

	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::shutdown()
{
	m_guiFrame->shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::select()
{
	int		chapter,level;
	m_active=true;
	m_exitPauseMenuFlag=false;
	m_quitGameFlag=false;
	m_guiFrame->select();

	chapter=GameScene.getChapterNumber();
	level=GameScene.getLevelNumber();
	m_guiSpatReadout->setSpatCounts(CGameSlotManager::getSlotData()->getSpatulaCollectedCount(chapter,level),
									GameScene.getTotalSpatCountForThisLevel());
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::unselect()
{
	m_active=false;
	m_guiFrame->unselect();
}
/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include "game\game.h"
#include "player\player.h"
void CPauseMenu::think(int _frames)
{
	if(m_active)
	{
		m_guiFrame->think(_frames);
		if(m_exitPauseMenuFlag||
		   m_quitGameFlag||
		   newmode!=-1)
		{
			if(m_quitGameFlag)
			{
				CGameScene::setReadyToExit();
			}
			unselect();
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::render()
{
	if(m_active)
	{
		m_guiFrame->render();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPauseMenu::isActive()
{
	return m_active;
}


/*===========================================================================
 end */
