/*=========================================================================

	bubicles.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "bubicles.h"

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
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

int				CBubicleFactory::s_initialised=false;
CBubicleEmitter	*CBubicleFactory::s_emitters;
CBubicle		*CBubicleFactory::s_bubicles;
SpriteBank		*CBubicleFactory::s_sprites;




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::create()
{
	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::init(BubicleEmitterData *_init)
{
	m_data=*_init;
	m_frameCount=0;
	m_spawnFrameCount=0;
	m_active=true;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::think(int _frames)
{
	m_frameCount+=_frames;
	m_spawnFrameCount+=_frames;

	if(m_spawnFrameCount>=m_data.m_birthRate)
	{
		m_spawnFrameCount=0;
		for(int i=0;i<m_data.m_birthAmount;i++)
		{
			if(CBubicleFactory::spawnParticle(&m_data)==NULL)
				break;	// Ran out of bubicles :(
		}
	}

	if(m_data.m_life!=-1&&m_frameCount>m_data.m_life)
		m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#ifdef SHOW_BUBICLE_EMITTERS
void CBubicleEmitter::render()
{
	POLY_F4		*f4;
	f4=GetPrimF4();
	setPolyF4(f4);
	setXYWH(f4,m_data.m_x,m_data.m_y,m_data.m_w,m_data.m_h);
	setSemiTrans(f4,1);
	setShadeTex(f4,0);
	setRGB0(f4,30,40,50);
	AddPrimToList(f4,m_data.m_bubicleBase.m_ot+1);
}
#endif


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::kill()
{
	ASSERT(m_active);
	
	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::setPos(int _x,int _y)
{
	ASSERT(m_active);
	
	m_data.m_x=_x;
	m_data.m_y=_y;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::setSize(int _w,int _h)
{
	ASSERT(m_active);
	
	m_data.m_w=_w;
	m_data.m_h=_h;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::create()
{
	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::init(BubicleData *_init,int _x,int _y)
{
	static int	frameType=0;

	m_data=*_init;

	m_x=(_x-(m_data.m_w>>1))<<ACCURACY_SHIFT;
	m_y=(_y-(m_data.m_h>>1))<<ACCURACY_SHIFT;

	m_data.m_vxmax;
	m_data.m_vymax;
	m_data.m_w<<=ACCURACY_SHIFT;
	m_data.m_h<<=ACCURACY_SHIFT;

	m_typeSizeChange=getRnd()&1;
	m_vSizeChange=0;	
	m_frameCount=0;

	m_fhBub=CBubicleFactory::getSprites()->getFrameHeader((frameType++)&0x31?FRM__BUBBLE_SMALL:FRM__BUBBLE_FLOWER);

	m_active=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::think(int _frames)
{
	ASSERT(isActive());

	for(int i=0;i<_frames;i++)		// Good or bad way to do this? Alternative is a bunch of multiplies..
	{
		if(m_x!=m_data.m_vxmax)
		{
			m_x+=m_data.m_vx;
			if(m_data.m_vxmax<0)
			{
				m_data.m_vx+=m_data.m_vdx;
				if(m_data.m_vx<m_data.m_vxmax)
					m_data.m_vx=m_data.m_vxmax;
			}
			else if(m_data.m_vxmax>0)
			{
				m_data.m_vx+=m_data.m_vdx;
				if(m_data.m_vx>m_data.m_vxmax)
					m_data.m_vx=m_data.m_vxmax;
			}
		}

		if(m_y!=m_data.m_vymax)
		{
			m_y+=m_data.m_vy;
			if(m_data.m_vymax<0)
			{
				m_data.m_vy+=m_data.m_vdy;
				if(m_data.m_vy<m_data.m_vymax)
					m_data.m_vy=m_data.m_vymax;
			}
			else if(m_data.m_vymax>0)
			{
				m_data.m_vy+=m_data.m_vdy;
				if(m_data.m_vy>m_data.m_vymax)
					m_data.m_vy=m_data.m_vymax;
			}
		}

		m_data.m_theta=(m_data.m_theta+m_data.m_vtheta)&4095;

		m_data.m_wobbleWidth+=m_data.m_vwobbleWidth;
		m_data.m_vwobbleWidth+=m_data.m_vdwobbleWidth;


		if(m_typeSizeChange)
		{
			if(m_data.m_w<m_data.m_h)
				m_vSizeChange+=m_data.m_dvSizeChange;
			else if(m_data.m_w>m_data.m_h)
				m_vSizeChange-=m_data.m_dvSizeChange;
			m_data.m_w+=m_vSizeChange;
		}
		else
		{
			if(m_data.m_h<m_data.m_w)
				m_vSizeChange+=m_data.m_dvSizeChange;
			else if(m_data.m_h>m_data.m_w)
				m_vSizeChange-=m_data.m_dvSizeChange;
			m_data.m_h+=m_vSizeChange;
		}
	}
	
	m_frameCount+=_frames;

	if(m_frameCount>m_data.m_life)
		m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::render()
{
	ASSERT(isActive());

	POLY_FT4	*ft4;
	int			x,y,w,h;

	x=m_x>>ACCURACY_SHIFT;
	y=m_y>>ACCURACY_SHIFT;
	w=m_data.m_w>>ACCURACY_SHIFT;
	h=m_data.m_h>>ACCURACY_SHIFT;

	x+=(msin(m_data.m_theta)*(m_data.m_wobbleWidth>>ACCURACY_SHIFT))>>12;

	ft4=CBubicleFactory::getSprites()->printFT4(m_fhBub,0,0,0,0,m_data.m_ot);
	setXYWH(ft4,x,y,w,h);
	setSemiTrans(ft4,1);
	setRGB0(ft4,m_data.m_colour.m_r,m_data.m_colour.m_g,m_data.m_colour.m_b);
}





/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::init()
{
	int				i;
	CBubicleEmitter	*emt;
	CBubicle		*bub;

	s_sprites=new ("Bubble Sprites") SpriteBank();
	s_sprites->load(INGAMEFX_INGAMEFX_SPR);

	emt=s_emitters=(CBubicleEmitter*)MemAlloc(sizeof(CBubicleEmitter)*NUM_EMITTERS,"BubicleEmitters");
	for(i=0;i<NUM_EMITTERS;i++,emt++)
		emt->create();
	
	bub=s_bubicles=(CBubicle*)MemAlloc(sizeof(CBubicle)*NUM_BUBICLES,"Bubicles");
	for(i=0;i<NUM_BUBICLES;i++,bub++)
		bub->create();

	SYSTEM_DBGMSG("CBubicleFactory::init ( allocated %d bytes )",(sizeof(CBubicleEmitter)*NUM_EMITTERS)+(sizeof(CBubicle)*NUM_BUBICLES));

	s_initialised=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::shutdown()
{
	ASSERT(s_initialised);
	
	MemFree(s_emitters);
	MemFree(s_bubicles);
	s_sprites->dump();			delete 	s_sprites;

	s_initialised=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::think(int _frames)
{
	ASSERT(s_initialised);

	int				frames;
	int				i;
	CBubicleEmitter	*emt;
	CBubicle		*bub;

	emt=s_emitters;
	for(i=0;i<NUM_EMITTERS;i++,emt++)
	{
		if(emt->isActive())
			emt->think(_frames);
	}

	bub=s_bubicles;
	for(i=0;i<NUM_BUBICLES;i++,bub++)
	{
		if(bub->isActive())
			bub->think(_frames);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::render()
{
	ASSERT(s_initialised);

	int				i;
	CBubicleEmitter	*emt;
	CBubicle		*bub;

#ifdef SHOW_BUBICLE_EMITTERS
	emt=s_emitters;
	for(i=0;i<NUM_EMITTERS;i++,emt++)
	{
		if(emt->isActive())
			emt->render();
	}
#endif
	
	bub=s_bubicles;
	for(i=0;i<NUM_BUBICLES;i++,bub++)
	{
		if(bub->isActive())
			bub->render();
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CBubicleEmitter	*CBubicleFactory::spawnEmitter(BubicleEmitterData *_init)
{
	ASSERT(s_initialised);

	int				i;
	CBubicleEmitter	*emt;
	
	emt=s_emitters;
	for(i=0;i<NUM_EMITTERS;i++,emt++)
	{
		if(!emt->isActive())
		{
			emt->init(_init);
			return emt;
		}
	}

	SYSTEM_DBGMSG("Out of CBubicleEmitters!");
	return NULL;
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CBubicle *CBubicleFactory::spawnParticle(BubicleEmitterData *_init)
{
	ASSERT(s_initialised);

	int				i;
	CBubicle		*bub;

	bub=s_bubicles;
	for(i=0;i<NUM_BUBICLES;i++,bub++)
	{
		if(!bub->isActive())
		{
			BubicleData		newBubData;
			int				x,y;

			x=_init->m_x+getRndRange(_init->m_w);
			y=_init->m_y+getRndRange(_init->m_h);

			newBubData.m_life=_init->m_bubicleBase.m_life+getRndRange(_init->m_bubicleRange.m_life);
			newBubData.m_vx=_init->m_bubicleBase.m_vx+getRndRange(_init->m_bubicleRange.m_vx);
			newBubData.m_vdx=_init->m_bubicleBase.m_vdx+getRndRange(_init->m_bubicleRange.m_vdx);
			newBubData.m_vxmax=_init->m_bubicleBase.m_vxmax+getRndRange(_init->m_bubicleRange.m_vxmax);
			newBubData.m_vy=_init->m_bubicleBase.m_vy+getRndRange(_init->m_bubicleRange.m_vy);
			newBubData.m_vdy=_init->m_bubicleBase.m_vdy+getRndRange(_init->m_bubicleRange.m_vdy);
			newBubData.m_vymax=_init->m_bubicleBase.m_vymax+getRndRange(_init->m_bubicleRange.m_vymax);
			newBubData.m_w=_init->m_bubicleBase.m_w+getRndRange(_init->m_bubicleRange.m_w);
			newBubData.m_h=_init->m_bubicleBase.m_h+getRndRange(_init->m_bubicleRange.m_h);
			newBubData.m_dvSizeChange=_init->m_bubicleBase.m_dvSizeChange+getRndRange(_init->m_bubicleRange.m_dvSizeChange);
			newBubData.m_theta=_init->m_bubicleBase.m_theta+getRndRange(_init->m_bubicleRange.m_theta);
			newBubData.m_vtheta=_init->m_bubicleBase.m_vtheta+getRndRange(_init->m_bubicleRange.m_vtheta);
			newBubData.m_wobbleWidth=_init->m_bubicleBase.m_wobbleWidth+getRndRange(_init->m_bubicleRange.m_wobbleWidth);
			newBubData.m_vwobbleWidth=_init->m_bubicleBase.m_vwobbleWidth+getRndRange(_init->m_bubicleRange.m_vwobbleWidth);
			newBubData.m_vdwobbleWidth=_init->m_bubicleBase.m_vdwobbleWidth+getRndRange(_init->m_bubicleRange.m_vdwobbleWidth);
			newBubData.m_ot=_init->m_bubicleBase.m_ot+getRndRange(_init->m_bubicleRange.m_ot);
			newBubData.m_colour.m_r=_init->m_bubicleBase.m_colour.m_r+getRndRange(_init->m_bubicleRange.m_colour.m_r);
			newBubData.m_colour.m_g=_init->m_bubicleBase.m_colour.m_r+getRndRange(_init->m_bubicleRange.m_colour.m_g);
			newBubData.m_colour.m_b=_init->m_bubicleBase.m_colour.m_r+getRndRange(_init->m_bubicleRange.m_colour.m_b);

			bub->init(&newBubData,x,y);

			return bub;
		}
	}

	SYSTEM_DBGMSG("Out of CBubicles!");
	return NULL;
}


/*===========================================================================
 end */