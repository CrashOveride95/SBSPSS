/*=========================================================================

	map.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "map\map.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
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

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

// Level data include files:
#include <CHAPTER01_LEVEL01_INF.h>
#include <CHAPTER01_LEVEL02_INF.h>
#include <CHAPTER01_LEVEL03_INF.h>
#include <CHAPTER01_LEVEL04_INF.h>
#include <CHAPTER02_LEVEL01_INF.h>
#include <CHAPTER02_LEVEL02_INF.h>
#include <CHAPTER02_LEVEL03_INF.h>
#include <CHAPTER02_LEVEL04_INF.h>
#include <CHAPTER03_LEVEL01_INF.h>
#include <CHAPTER03_LEVEL02_INF.h>
#include <CHAPTER03_LEVEL03_INF.h>
#include <CHAPTER03_LEVEL04_INF.h>
#include <CHAPTER04_LEVEL01_INF.h>
#include <CHAPTER04_LEVEL02_INF.h>
#include <CHAPTER04_LEVEL03_INF.h>
#include <CHAPTER04_LEVEL04_INF.h>
#include <CHAPTER05_LEVEL01_INF.h>
#include <CHAPTER05_LEVEL02_INF.h>
#include <CHAPTER05_LEVEL03_INF.h>
#include <CHAPTER05_LEVEL04_INF.h>
#include <CHAPTER06_LEVEL01_INF.h>
#include <CHAPTER06_LEVEL02_INF.h>
#include <CHAPTER06_LEVEL03_INF.h>
#include <CHAPTER06_LEVEL04_INF.h>
#include <CHAPTER06_LEVEL05_INF.h>


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

CMapScene	MapScene;


CMapScene::sMapLevelData	CMapScene::s_mapLevelData[MAP_NUM_CHAPTERS][MAP_NUM_LEVELS_PER_CHAPTER]=
{
	//		mapFile				globalLevelNumber	spatularOrTokenCounts								questItemFrame			kelpWorldLevel
	{
		{	MAP_C1_L1_GFX,		0,					CHAPTER01_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L1_QUEST_ITEM,	false	},
		{	MAP_C1_L2_GFX,		1,					CHAPTER01_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L2_QUEST_ITEM,	false	},
		{	MAP_C1_L3_GFX,		2,					CHAPTER01_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L3_QUEST_ITEM,	false	},
		{	MAP_C1_L4_GFX,		3,					CHAPTER01_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C1_L4_QUEST_ITEM,	false	},
		{	MAP_C1_FAIR_GFX,	20,					CHAPTER06_LEVEL01_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_C2_L1_GFX,		4,					CHAPTER02_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L1_QUEST_ITEM,	false	},
		{	MAP_C2_L2_GFX,		5,					CHAPTER02_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L2_QUEST_ITEM,	false	},
		{	MAP_C2_L3_GFX,		6,					CHAPTER02_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L3_QUEST_ITEM,	false	},
		{	MAP_C2_L4_GFX,		7,					CHAPTER02_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C2_L4_QUEST_ITEM,	false	},
		{	MAP_C2_FAIR_GFX,	21,					CHAPTER06_LEVEL02_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_C3_L1_GFX,		8,					CHAPTER03_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L1_QUEST_ITEM,	false	},
		{	MAP_C3_L2_GFX,		9,					CHAPTER03_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L2_QUEST_ITEM,	false	},
		{	MAP_C3_L3_GFX,		10,					CHAPTER03_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L3_QUEST_ITEM,	false	},
		{	MAP_C3_L4_GFX,		11,					CHAPTER03_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C3_L4_QUEST_ITEM,	false	},
		{	MAP_C3_FAIR_GFX,	22,					CHAPTER06_LEVEL03_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_C4_L1_GFX,		12,					CHAPTER04_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_C4_L2_GFX,		13,					CHAPTER04_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_C4_L3_GFX,		14,					CHAPTER04_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_1,	false	},
		{	MAP_C4_L4_GFX,		15,					CHAPTER04_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C4_QUEST_ITEM_2,	false	},
		{	MAP_C4_FAIR_GFX,	23,					CHAPTER06_LEVEL04_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
	{
		{	MAP_C5_L1_GFX,		16,					CHAPTER05_LEVEL01_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L1_QUEST_ITEM,	false	},
		{	MAP_C5_L2_GFX,		17,					CHAPTER05_LEVEL02_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L2_QUEST_ITEM,	false	},
		{	MAP_C5_L3_GFX,		18,					CHAPTER05_LEVEL03_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L3_QUEST_ITEM,	false	},
		{	MAP_C5_L4_GFX,		19,					CHAPTER05_LEVEL04_INF_TOTAL_ITEM_GOLDEN_SPATULA,	FRM__C5_L4_QUEST_ITEM,	false	},
		{	MAP_C5_FAIR_GFX,	24,					CHAPTER06_LEVEL05_INF_TOTAL_ITEM_KELP_TOKEN,		-1,						true	},
	},
};


DVECTOR						CMapScene::s_mapLevelPositions[MAP_NUM_LEVELS_PER_CHAPTER]=
{
	{	42,49	},
	{	186,49	},
	{	330,49	},
	{	42,113	},
	{	330,113	},
};


extern int s_globalLevelSelectThing;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::init()
{
	m_font=new ("map screen font") FontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_CENTRE);
	m_font->setOt(10);
	m_font->setJustification(FontBank::JUST_LEFT);

	m_currentChapterSelection=0;
	m_currentLevelSelection=0;
	m_screenImage=MemAlloc(512*256*2,"MapScreen");
	generateMapScreenImage();
	SetScreenImage((u8*)m_screenImage);

	m_readyToExit=false;
	CFader::setFadingIn(CFader::BLACK_FADE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::shutdown()
{
	ClearScreenImage();
	MemFree(m_screenImage);
	m_font->dump();				delete m_font;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::render()
{
	sMapLevelData	*level;
	int				i;
	sFrameHdr		*fh;
	DVECTOR			pos;
	char			buf[100];
	
	level=&s_mapLevelData[m_currentChapterSelection][0];

	// Render spatula/kelp counts and quest items
	for(i=0;i<MAP_NUM_LEVELS_PER_CHAPTER;i++)
	{
		pos=s_mapLevelPositions[i];

		if(!level->m_kelpWorldLevel)
		{
			// Normal level
			POLY_FT4		*ft4;
			m_font->setColour(253,251,67);
			sprintf(buf,"00/%d",level->m_spatularOrTokenCounts);
			fh=CGameScene::getSpriteBank()->getFrameHeader(level->m_questItemFrame);
			ft4=CGameScene::getSpriteBank()->printFT4(fh,pos.vx+MAP_LEVEL_WIDTH-fh->W,pos.vy+MAP_LEVEL_HEIGHT-fh->H,0,0,10);
			if(!hasQuestItemBeenCollected(m_currentChapterSelection,i))
			{
				setRGB0(ft4,50,50,50);
			}
		}
		else
		{
			// Bonuse level
			m_font->setColour(67,251,67);
			sprintf(buf,"00/%d",level->m_spatularOrTokenCounts);
		}

		m_font->print(pos.vx,pos.vy,buf);

		level++;
	}

	// Selection cursor
	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__MAPPOINTER);
	CGameScene::getSpriteBank()->printFT4(fh,m_pointerPos.vx-(fh->W/2),m_pointerPos.vy-(fh->H/2),0,0,9);

sprintf(buf,"Chapter %d, Level %d",m_currentChapterSelection+1,m_currentLevelSelection+1);
m_font->setColour(0,255,0);
m_font->print(24,24,buf);
m_font->setColour(0,0,0);
m_font->print(25,25,buf);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CMapScene::think(int _frames)
{
	if(!CFader::isFading()&&!m_readyToExit)
	{
		// Change chapter
		if(PadGetDown(0)&PAD_UP)
		{
			if(++m_currentChapterSelection>=MAP_NUM_CHAPTERS)m_currentChapterSelection=0;
			generateMapScreenImage();
		}
		else if(PadGetDown(0)&PAD_DOWN)
		{
			if(--m_currentChapterSelection<0)m_currentChapterSelection=MAP_NUM_CHAPTERS-1;
			generateMapScreenImage();
		}

		// Move cursor
		if(PadGetDown(0)&PAD_LEFT)
		{
			do
			{
				if(--m_currentLevelSelection<0)m_currentLevelSelection=MAP_NUM_LEVELS_PER_CHAPTER-1;
			}
			while(!isLevelOpen(m_currentChapterSelection,m_currentLevelSelection));
		}
		else if(PadGetDown(0)&PAD_RIGHT)
		{
			do
			{
				if(++m_currentLevelSelection>=MAP_NUM_LEVELS_PER_CHAPTER)m_currentLevelSelection=0;
			}
			while(!isLevelOpen(m_currentChapterSelection,m_currentLevelSelection));
		}
		
		// Calc where the pointer should be
		m_pointerSin=(m_pointerSin+(_frames*70))&4095;
		m_pointerTarget=getPointerTargetPosition();

		// Move the pointer
		for(int i=0;i<_frames;i++)
		{
			int delta;

			delta=m_pointerTarget.vx-m_pointerPos.vx;
			if(delta<0)
			{
				delta/=2;
				if(!delta)delta=-1;
			}
			else if(delta>0)
			{
				delta/=2;
				if(!delta)delta=+1;
			}
			m_pointerPos.vx+=delta;

			delta=m_pointerTarget.vy-m_pointerPos.vy;
			if(delta<0)
			{
				delta/=2;
				if(!delta)delta=-1;
			}
			else if(delta>0)
			{
				delta/=2;
				if(!delta)delta=+1;
			}
			m_pointerPos.vy+=delta;
		}

		if(PadGetDown(0)&(PAD_CROSS|PAD_START))
		{
			s_globalLevelSelectThing=s_mapLevelData[m_currentChapterSelection][m_currentLevelSelection].m_globalLevelNumber;
			m_readyToExit=true;
//			CFader::setFadingOut(CFader::BLACK_FADE);
			GameState::setNextScene(&GameScene);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CMapScene::readyToShutdown()
{
	return m_readyToExit&&!CFader::isFading();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CMapScene::generateMapScreenImage()
{
	int	i;


	m_currentLevelSelection=0;
	memset(m_screenImage,0,512*256*2);
	copyImageToScreen(MAP_MAP_BACKGROUND_GFX,MAP_PARCHMENT_START_X,MAP_PARCHMENT_START_Y,MAP_PARCHMENT_WIDTH,MAP_PARCHMENT_HEIGHT);
	for(i=0;i<MAP_NUM_LEVELS_PER_CHAPTER;i++)
	{
		if(isLevelOpen(m_currentChapterSelection,i))
		{
			copyImageToScreen(s_mapLevelData[m_currentChapterSelection][i].m_mapFile,s_mapLevelPositions[i].vx,s_mapLevelPositions[i].vy,MAP_LEVEL_WIDTH,MAP_LEVEL_HEIGHT);
		}
	}

	m_pointerPos=getPointerTargetPosition();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CMapScene::copyImageToScreen(int _file,int _x,int _y,int _w,int _h)
{
	u8	*image;
	u16	*src,*dst;
	int	x,y;

	image=CFileIO::loadFile((FileEquate)_file);ASSERT(image);
	src=(u16*)image;
	dst=(u16*)m_screenImage+(_x+(_y*512));
	for(y=0;y<_h;y++)
	{
		for(x=0;x<_w;x++)
		{
			*dst++=*src++;
		}
		dst+=512-_w;
	}
	MemFree(image);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CMapScene::getPointerTargetPosition()
{
	DVECTOR	pos;
	
	pos=s_mapLevelPositions[m_currentLevelSelection];
	pos.vx+=(MAP_LEVEL_WIDTH/2);
	pos.vy+=MAP_LEVEL_HEIGHT+(msin(m_pointerSin)*4>>12);

	return pos;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CMapScene::isLevelOpen(unsigned int _chapter,unsigned int _level)
{
	return true;
}

int		CMapScene::getSpatulaCollectedCount(unsigned int _chapter,unsigned int _level)
{
	return CGameSlotManager::getSlotData()->getSpatulaCollectedCount(_chapter,_level);
}

int		CMapScene::getSpatulaAvailableCount(unsigned int _chapter,unsigned int _level)
{
	ASSERT(!s_mapLevelData[_chapter][_level].m_kelpWorldLevel);
	return s_mapLevelData[_chapter][_level].m_spatularOrTokenCounts;
}

int		CMapScene::getKelpTokenAvailableCount(unsigned int _chapter,unsigned int _level)
{
	ASSERT(s_mapLevelData[_chapter][_level].m_kelpWorldLevel);
	return s_mapLevelData[_chapter][_level].m_spatularOrTokenCounts;
}

int		CMapScene::hasQuestItemBeenCollected(unsigned int _chapter,unsigned int _level)
{
	return _level&1;
}


/*===========================================================================
 end */