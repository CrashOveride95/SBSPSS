/*******************/
/*** Actor Stuff ***/
/*******************/

#ifndef __ACTOR_HEADER__
#define __ACTOR_HEADER__

#include "system\global.h"
#include <dstructs.h>

#ifndef	__BigLump_H__
#include <biglump.h>
#endif

/*****************************************************************************/
class	CActorGfx
{
public:
		CActorGfx(FileEquate Filename);
virtual	~CActorGfx();

		void		Init(FileEquate _Filename);
		POLY_FT4	*Render(DVECTOR &Pos,int Anim,int Frame,bool FlipX=false,bool FlipY=false,bool Shadow=false);
		void		Dump();

		int			getFrameCount(int Anim)		{return(SpriteBank->AnimList[Anim].FrameCount);}

		FileEquate	GetFilename()	{return(Filename);}
private:
		void		SetUpFT4(POLY_FT4 *Ft4,sSpriteFrame *ThisFrame,int X,int Y,bool XFlip,bool YFlip);

		FileEquate		Filename;
		sSpriteAnimBank	*SpriteBank;
};

/*****************************************************************************/
class CActorPool
{
public:
		enum
		{
			MAX_ACTORS=8,
		};

static	void		Init();

static	void			AddActor(FileEquate Filename)		{GetActor(Filename);}
static	CActorGfx		*GetActor(FileEquate Filename);
static	void			DumpActor(FileEquate Filename);
		
private:
static	int			FindIdx(FileEquate Filename);
static	int			FindFreeIdx();

static	CActorGfx	*ActorList[MAX_ACTORS];

};

/*****************************************************************************/

#endif