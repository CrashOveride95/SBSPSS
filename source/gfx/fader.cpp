/*=========================================================================

	fader.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "fader.h"

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __GFX_BUBICLES_H__
#include "gfx\bubicles.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

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

CFader::FADE_MODE	CFader::s_fadeMode=FADED_IN;
CFader::FADE_STYLE	CFader::s_fadeStyle;
int					CFader::s_fadeValue=0;	
int					CFader::s_waitFrames=0;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFader::render()
{
	switch(s_fadeMode)
	{
		case FADED_IN:
		case PAUSE_BEFORE_FADING_IN:
		case PAUSE_BEFORE_FADING_OUT:
			return;

		case FADING_IN:
			{
				switch(s_fadeStyle)
				{
					case BLACK_FADE:
					case WHITE_FADE:
					{
						POLY_F4		*f4;
						POLY_FT3	*ft3;
						f4=GetPrimF4();
						setXYWH(f4,0,0,511,255);
						setRGB0(f4,s_fadeValue,s_fadeValue,s_fadeValue);
						setShadeTex(f4,0);
						setSemiTrans(f4,1);
						AddPrimToList(f4,0);
						ft3=GetPrimFT3();
						setPolyFT3(ft3);
						setShadeTex(ft3,1);
						setSemiTrans(ft3,1);
						ft3->tpage=(s_fadeStyle==BLACK_FADE?2:1)<<5;
						setXY3(ft3,0,0,0,512,512,0);
						AddPrimToList(ft3,0);
						break;
					}
				}
			}
			break;

		case FADING_OUT:
		case PAUSE_AFTER_FADING_OUT:
			{
				switch(s_fadeStyle)
				{
					case BLACK_FADE:
					case WHITE_FADE:
					{
						POLY_F4		*f4;
						POLY_FT3	*ft3;
						int			col;
						col=255-s_fadeValue;
						f4=GetPrimF4();
						setXYWH(f4,0,0,511,255);
						setRGB0(f4,col,col,col);
						setShadeTex(f4,0);
						setSemiTrans(f4,1);
						AddPrimToList(f4,0);
						ft3=GetPrimFT3();
						setPolyFT3(ft3);
						setShadeTex(ft3,1);
						setSemiTrans(ft3,1);
						ft3->tpage=(s_fadeStyle==BLACK_FADE?2:1)<<5;
						setXY3(ft3,512,512,512,512,512,512);
						AddPrimToList(ft3,0);
						break;
					}
				}
			}
			break;

		case FADED_OUT:
			{
				POLY_F4		*f4;
				
				f4=GetPrimF4();
				setXYWH(f4,0,0,512,256);
				switch(s_fadeStyle)
				{
					case BLACK_FADE:
						setRGB0(f4,0,0,0);
						break;
					case WHITE_FADE:
						setRGB0(f4,255,255,255);
						break;
				}
				AddPrimToList(f4,0);
				return;
			}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFader::think(int _frames)
{
	if(s_waitFrames)
	{
		s_waitFrames--;
		return;
	}

	switch(s_fadeMode)
	{
		case FADING_IN:
		case FADING_OUT:
			s_fadeValue-=_frames*FADE_SPEED;
			switch(s_fadeStyle)
			{
				case BLACK_FADE:
				case WHITE_FADE:
				{
					if(s_fadeValue<0)
					{
						s_fadeValue=0;
						if(s_fadeMode==FADING_OUT)
						{
							s_fadeMode=FADED_OUT;
							CSoundMediator::setVolume(CSoundMediator::VOL_FADE,0);
							s_waitFrames=FRAMES_TO_WAIT;
						}
						else
						{
							s_fadeMode=FADED_IN;
							CSoundMediator::setVolume(CSoundMediator::VOL_FADE,255);
						}
					}
					else
					{
						CSoundMediator::setVolume(CSoundMediator::VOL_FADE,s_fadeMode==FADING_OUT?s_fadeValue:255-s_fadeValue);
					}
					break;
				}
			}
			break;

		case PAUSE_BEFORE_FADING_IN:
			if(--s_waitFrames==0)
			{
				s_fadeMode==FADING_IN;
			}
			break;

		case PAUSE_BEFORE_FADING_OUT:
			if(--s_waitFrames==0)
			{
				s_fadeMode==FADING_OUT;
			}
			break;

		case PAUSE_AFTER_FADING_OUT:
			if(--s_waitFrames==0)
			{
				s_fadeMode==FADED_OUT;
			}
			break;

		default:
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFader::setFadingOut(FADE_STYLE _style)
{
	switch(_style)
	{
		case BLACK_FADE:
		case WHITE_FADE:
			s_fadeValue=255;
			break;
	}

	s_fadeMode=FADING_OUT;
	s_fadeStyle=_style;
	s_waitFrames=FRAMES_TO_WAIT;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFader::setFadingIn(FADE_STYLE _style)
{
	switch(_style)
	{
		case BLACK_FADE:
		case WHITE_FADE:
			s_fadeValue=255;
			break;
	}
	s_fadeMode=FADING_IN;
	s_fadeStyle=_style;
	s_waitFrames=FRAMES_TO_WAIT;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFader::isFading()
{
	return s_fadeMode==FADING_IN||s_fadeMode==FADING_OUT;
}


/*===========================================================================
 end */