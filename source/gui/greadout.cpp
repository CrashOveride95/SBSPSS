/*=========================================================================

	greadout.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\greadout.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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
void CGUITextReadout::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObjectWithFont::init(_parent,_id);
	m_target=0;
	m_data=0;
	m_lastValue=-1;
	m_textId=TranslationDatabase::NO_STRING;
	m_textY=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::setReadoutTarget(int *_target)
{
	m_target=_target;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::setReadoutData(TextReadoutData *_data)
{
	m_data=_data;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::render()
{
	if(!isHidden())
	{
		getFontBank()->print((getW()-(BORDERWIDTH*2))/2,m_textY,m_textId);
	}
	CGUIObjectWithFont::render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::think(int _frames)
{
	CGUIObjectWithFont::think(_frames);
	if(*m_target!=m_lastValue)
	{
		recalc();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::recalc()
{
	CGUIObjectWithFont::recalc();

	if(m_data)
	{
		FontBank		*fb;
		char			*string;
		TextReadoutData	*data;
		int				tmp;

		m_lastValue=*m_target;
		fb=getFontBank();
		m_textId=STR__DUMMY;
		data=m_data;
		do
		{
			if(m_lastValue==data->m_value)
			{
				m_textId=data->m_textId;
				break;
			}
			tmp=data->m_value;
			data++;
		}
		while(tmp<data->m_value);
		string=(char*)TranslationDatabase::getString(m_textId);
		
#ifdef __VERSION_DEBUG__
		if(fb->getStringWidth(string)>getW()-(BORDERWIDTH*2)||
		   fb->getStringHeight(string)>getH()-(BORDERHEIGHT*2))
		{
			GUI_DBGMSG("Text overflow in CGUITextReadout!");
		}
#endif
		
		m_textY=(getH()-(BORDERHEIGHT*2)-fb->getStringHeight(string))/2;
	}
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpatCountReadout::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObjectWithFont::init(_parent,_id);
	m_spatsCollected=m_spatsTotal=0;
	m_spriteBank=new ("SpatCountReadoutSprites") SpriteBank();
	m_spriteBank->load(SPRITES_SPRITES_SPR);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpatCountReadout::shutdown()
{
	m_spriteBank->dump();	delete m_spriteBank;
	CGUIObjectWithFont::shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpatCountReadout::setSpatCounts(int _collected,int _total)
{
	m_spatsCollected=_collected;
	m_spatsTotal=_total;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpatCountReadout::render()
{
	if(!isHidden())
	{
		FontBank	*fb;
		sFrameHdr	*fh;
		char		buf[100];
		int			x,y;

		fb=getFontBank();
		fh=m_spriteBank->getFrameHeader(FRM__SPATULA);

		sprintf(buf,"%d/%d",m_spatsCollected,m_spatsTotal);
		x=getW()-fb->getStringWidth(buf);
		y=(getH()/2)-fb->getCharHeight();
		fb->print(x,y,buf);

		x=getX()+getParentX();
		y=getY()+getParentY()+((getH()-fh->H)/2);
		m_spriteBank->printFT4(fh,x,y,0,0,0);
	}
	CGUIObjectWithFont::render();
}









/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObjectWithSpriteBank::init(_parent,_id);
	m_target=0;
	m_data=0;
	m_lastValue=-1;
	m_frame=0;
	m_x=m_y=0;
	setSpriteBank(SPRITES_SPRITES_SPR);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::setReadoutTarget(int *_target)
{
	m_target=_target;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::setReadoutData(SpriteReadoutData *_data)
{
	m_data=_data;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::render()
{
	FontBank	*fb;
	
	if(!isHidden())
	{
		getSpriteBank()->printFT4(m_frame,getParentX()+m_x,getParentY()+m_y,0,0,getOt());
	}
	CGUIObjectWithSpriteBank::render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::think(int _frames)
{
	CGUIObject::think(_frames);
	if(*m_target!=m_lastValue)
	{
		recalc();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::recalc()
{
	CGUIObject::recalc();

	if(m_data)
	{
		char				*string;
		SpriteReadoutData	*data;
		int					tmp;
		sFrameHdr			*fh;
		int					x,y;

		m_lastValue=*m_target;
		m_frame=0;
		data=m_data;
		do
		{
			if(m_lastValue==data->m_value)
			{
				m_frame=data->m_frame;
				break;
			}
			tmp=data->m_value;
			data++;
		}
		while(tmp<data->m_value);
		fh=getSpriteBank()->getFrameHeader(m_frame);

#ifdef __VERSION_DEBUG__
		if(fh->W>getW()-(BORDERWIDTH*2)||
		   fh->H>getH()-(BORDERHEIGHT*2))
		{
			GUI_DBGMSG("Sprite overflow in CGUISpriteReadout!");
		}
#endif

		m_x=getX()+(getW()-fh->W)/2;
		m_y=getY()+(getH()-fh->H)/2;
	}
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBarReadout::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObject::init(_parent,_id);
	setReadoutTarget(0);
	setReadoutRange(0,255);
	m_markerOffset=0;
	m_lastValue=-1;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBarReadout::setReadoutTarget(int *_target)
{
	m_target=_target;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBarReadout::setReadoutRange(int _min,int _max)
{
	m_min=_min;
	m_max=_max;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBarReadout::render()
{
	POLY_G4	*g4;
	int		x,y,w,h;
	int		r,g,b;
	int		ot;

	x=getX()+getParentX();
	y=getY()+getParentY();
	w=getW();
	h=getH();
	r=g=b=isSelected()?245:110;
	ot=getOt();

	DrawLine(x,y+(h/2),x+w,y+(h/2),r,g,b,ot);
	DrawLine(x+m_markerOffset,y,x+m_markerOffset,y+h,r,g,b,ot);

	CGUIObject::render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBarReadout::think(int _frames)
{
	if(m_lastValue!=*m_target)
	{
		recalc();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIBarReadout::recalc()
{
	if(m_target)
	{
		int		w,offset;
		int		scaler;
		
		scaler=(getW()<<8)/(m_max-m_min);
		offset=(scaler*(*m_target-m_min))>>8;

		m_markerOffset=offset;

		m_lastValue=*m_target;
	}
}


/*===========================================================================
 end */