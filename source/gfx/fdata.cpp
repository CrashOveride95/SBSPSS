/*=========================================================================

	fdata.cpp

	Author:		PKG
	Created: 
	Project:	PRLSR
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gfx\fdata.h"

#ifndef		__FILE_EQUATES_H__
#include	"biglump.h"
#endif

#include	<UiGfx.h>


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

s16	standardFontTab[]=
{
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/*   ! " # $ % & ' */	           -1,FRM_FONT_0037,FRM_FONT_0038,           -1,FRM_FONT_0052,FRM_FONT_0039,FRM_FONT_0040,FRM_FONT_0036,
	/* ( ) * + , - . / */	FRM_FONT_0042,FRM_FONT_0043,FRM_FONT_0041,FRM_FONT_0152,FRM_FONT_0047,FRM_FONT_0044,FRM_FONT_0053,FRM_FONT_0050,
	/* 0 1 2 3 4 5 6 7 */	FRM_FONT_0026,FRM_FONT_0027,FRM_FONT_0028,FRM_FONT_0029,FRM_FONT_0030,FRM_FONT_0031,FRM_FONT_0032,FRM_FONT_0033,
	/* 8 9 : ; < = > ? */	FRM_FONT_0034,FRM_FONT_0035,FRM_FONT_0045,FRM_FONT_0046,FRM_FONT_0149,FRM_FONT_0153,FRM_FONT_0150,FRM_FONT_0048,
	/* @ A B C D E F G */	           -1,FRM_FONT_0000,FRM_FONT_0001,FRM_FONT_0002,FRM_FONT_0003,FRM_FONT_0004,FRM_FONT_0005,FRM_FONT_0006,
	/* H I J K L M N O */	FRM_FONT_0007,FRM_FONT_0008,FRM_FONT_0009,FRM_FONT_0010,FRM_FONT_0011,FRM_FONT_0012,FRM_FONT_0013,FRM_FONT_0014,
	/* P Q R S T U V W */	FRM_FONT_0015,FRM_FONT_0016,FRM_FONT_0017,FRM_FONT_0018,FRM_FONT_0019,FRM_FONT_0020,FRM_FONT_0021,FRM_FONT_0022,
	/* X Y Z [ \ ] ^ _ */	FRM_FONT_0023,FRM_FONT_0024,FRM_FONT_0025,FRM_FONT_0054,FRM_FONT_0049,FRM_FONT_0055,           -1,FRM_FONT_0151,
	/* ` a b c d e f g */	           -1,FRM_FONT_0056,FRM_FONT_0057,FRM_FONT_0058,FRM_FONT_0059,FRM_FONT_0060,FRM_FONT_0061,FRM_FONT_0062,
	/* h i j k l m n o */	FRM_FONT_0063,FRM_FONT_0064,FRM_FONT_0065,FRM_FONT_0066,FRM_FONT_0067,FRM_FONT_0068,FRM_FONT_0069,FRM_FONT_0070,
	/* p q r s t u v w */	FRM_FONT_0071,FRM_FONT_0072,FRM_FONT_0073,FRM_FONT_0074,FRM_FONT_0075,FRM_FONT_0076,FRM_FONT_0077,FRM_FONT_0078,
	/* x y z { | } ~ . */	FRM_FONT_0079,FRM_FONT_0080,FRM_FONT_0081,           -1,           -1,           -1,           -1,           -1,
	/* � � � � � � � � */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* � � � � � � � � */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* � � � � � � � � */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* � � � � � � � � */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1
	/* � � � � � � � � */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* � � � � � � � � */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* � � � � � � � � */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* � � � � � � � � */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							-1,-1,
	/* � � � � � � � � */	FRM_FONT_0084,FRM_FONT_0085,FRM_FONT_0086,FRM_FONT_0087,FRM_FONT_0088,FRM_FONT_0089,FRM_FONT_0090,FRM_FONT_0091,
	/* � � � � � � � � */	FRM_FONT_0092,FRM_FONT_0093,FRM_FONT_0094,FRM_FONT_0095,FRM_FONT_0096,FRM_FONT_0097,FRM_FONT_0098,FRM_FONT_0099,
	/* � � � � � � � � */	FRM_FONT_0100,FRM_FONT_0101,FRM_FONT_0102,FRM_FONT_0103,FRM_FONT_0104,FRM_FONT_0105,FRM_FONT_0106,FRM_FONT_0107,
	/* � � � � � � � � */	FRM_FONT_0108,FRM_FONT_0109,FRM_FONT_0110,FRM_FONT_0111,FRM_FONT_0112,FRM_FONT_0113,FRM_FONT_0114,FRM_FONT_0115,
	/* � � � � � � � � */	FRM_FONT_0116,FRM_FONT_0117,FRM_FONT_0118,FRM_FONT_0119,FRM_FONT_0120,FRM_FONT_0121,FRM_FONT_0122,FRM_FONT_0123,
	/* � � � � � � � � */	FRM_FONT_0124,FRM_FONT_0125,FRM_FONT_0126,FRM_FONT_0127,FRM_FONT_0128,FRM_FONT_0129,FRM_FONT_0130,FRM_FONT_0131,
	/* � � � � � � � � */	FRM_FONT_0132,FRM_FONT_0133,FRM_FONT_0134,FRM_FONT_0135,FRM_FONT_0136,FRM_FONT_0137,FRM_FONT_0138,FRM_FONT_0139,
	/* � � � � � � � � */	FRM_FONT_0140,FRM_FONT_0141,FRM_FONT_0142,FRM_FONT_0143,FRM_FONT_0144,FRM_FONT_0145,FRM_FONT_0146,FRM_FONT_0147,
};
FontData	standardFont( UI_UIGFX_SPR,	standardFontTab,	9,	1,1, 4 );



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
FontData::FontData( FileEquate _fontFileId, s16 *_fontTab, s16 _charHeight, s16 _charGapX, s16 _charGapY, s16 _spaceWidth )
{
	fontFileId=_fontFileId;
	fontTab=_fontTab;
	charHeight=_charHeight;
	charGapX=_charGapX;
	charGapY=_charGapY;
	spaceWidth=_spaceWidth;
}


/*===========================================================================
 end */