/*=========================================================================

	gfactory.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	A factory to simplify the creation of actual UI objects. Rather than
				creating the constituent parts of an item yourself, just call the
				member functions here.

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GFACTORY_H__
#define	__GUI_GFACTORY_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGUIFactory
{
public:
	static void		createValueButtonFrame(class CGUIObject *_parent,
										   int _x,int _y,int _w, int _h,
										   int _textId,
										   int *_target,int _value);
	static void		createSliderButtonFrame(class CGUIObject *_parent,
											int _x,int _y,int _w, int _h,
											int _textId,
											int *_target,int _min, int _max);
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GFACTORY_H__ */

/*===========================================================================
 end */
