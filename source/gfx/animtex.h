/**************************/
/*** Anim Texture Stuph ***/
/**************************/

#ifndef _ANIMTEX_HEADER_
#define _ANIMTEX_HEADER_

#include "gfx\tpage.h"
#include <DStructs.h>

/*****************************************************************************/
class CAnimTex
{
public:
		CAnimTex();
		~CAnimTex();
		
static	void	AddAnimTex(sFrameHdr *Frame,FileEquate Filename);
static	void	AnimateTex();
static	void	GetTexData();
static	void	DumpThisTPage(FileEquate TexName);

private:
		CAnimTex	*NextTex;
		RECT		Rect;
		s16			Speed,Count;
		s16			PixPerWord;
		u32			*TexData;
		FileEquate	TexName;
		u8			TPage;

};

/*****************************************************************************/
class CMoveTex
{
public:
		enum
		{
			MOVETEX_MAX=16,
		};

		CMoveTex()
		{
//			Src=0;
		}
		
//static	void	Add(sTexInfo &SrcFrame,sTexInfo &DstFrame);
static	void	MoveTex();

private:
//		sTexInfo	*Src,*Dst;

static	CMoveTex	MoveTexList[];
};

/*****************************************************************************/

#endif