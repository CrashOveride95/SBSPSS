/*=========================================================================

	thing.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__THING_THING_H__
#define __THING_THING_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif

#ifndef __GAME_EVENT_H__
#include "game\event.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Collision rectangle definition
typedef struct
{
	int	x1,y1,x2,y2;
}
CRECT;


// Thing manager class
class CThingManager
{
public:
	static void		init();
	static void		shutdown();
	static void		killAllThingsForRespawn();

	static void		thinkAllThings(int _frames);
	static void		renderAllThings();
	static void		processEventAllThings(GAME_EVENT _event,class CThing *_sourceThing);

	static CThing*	checkCollisionAreaAgainstThings(CRECT *_area,int _type,int _continue);

protected:
	static void					addToThingList(class CThing *_this);
	static void					removeFromThingList(CThing *_this);
	friend class CThing;

private:
	static class CThing			*s_thingLists[];
	static int					s_initialised;

};


// Base thing class
class CThing
{
public:
	typedef enum
	{
		TYPE_PICKUP,
		TYPE_PLATFORM,
		TYPE_PLAYER,
		TYPE_PLAYERPROJECTILE,
		TYPE_NPC,
		TYPE_ENEMY,
		TYPE_ENEMYPROJECTILE,
		TYPE_TRIGGER,
		TYPE_HAZARD,
		TYPE_FX,

		MAX_TYPE,
	}
	TYPE;

					CThing()	{;}
	virtual			~CThing()	{;}

	virtual TYPE	getThingType()=0;


	virtual	void	init();
	virtual	void	shutdown();
	virtual	void	think(int _frames);
	virtual	void	render();
	virtual u8		isSetToShutdown()										{return( false );}

// Linkage
	void			addChild(CThing *Child);
	void			removeChild(CThing *Child);
	void			removeAllChild();
	void			deleteAllChild();
	bool			hasChild(CThing *Child);
	int				getNumChildren();


	DVECTOR			getPos()						{return Pos;}
	void			setPos(DVECTOR newPos)			{Pos=newPos;}
	DVECTOR			getPosDelta()					{return PosDelta;}
	CThing			*getNext()						{return Next;}


	virtual void	processEvent(GAME_EVENT _event,CThing *_sourceThing);


protected:

	
// Linkage
		CThing		*Parent,*Next;
// Count
		int			m_numChildren;
// Pos
		DVECTOR		Pos, PosLast, PosDelta;

public:
		class CThing			*m_nextThing;


		
		

	// -- Collision --
public:
	DVECTOR			getCollisionCentre()						{return m_collisionCentre;}
	DVECTOR			getCollisionCentreOffset()					{return m_collisionCentreOffset;}
	int				getCollisionRadius()						{return m_collisionRadius;}
	CRECT			getCollisionArea()							{return m_collisionArea;}
	s16				getCollisionAngle()							{return m_collisionAngle;}		// pkg - move to CNpcPlatform?
	DVECTOR			getNewCollidedPos()							{return m_newCollidedPos;}		// pkg - to be removed?
	DVECTOR			getCollisionSize()							{return m_collisionSize;}

	virtual int		canCollide()								{return true;}
	virtual int		checkCollisionAgainst(CThing *_thisThing, int _frames);
	int				checkCollisionAgainstArea(CRECT *_rect);
	void			updateCollisionArea();
	virtual void	collidedWith(CThing *_thisThing)			{;}
	virtual void	setHasPlatformCollided( bool newVal )		{;}
	virtual bool	getHasPlatformCollided()					{return false;}
	virtual s32		getNewYPos( CThing *_thisThing );
	void			setNewCollidedPos(DVECTOR newPos)			{m_newCollidedPos = newPos;}	// pkg - to be removed?

	bool			IsOnScreen()	{return(m_OnScreenFlag);/* Put check code here */}
	bool			getOnScreenFlag()							{return(m_OnScreenFlag);}
	void			setOnScreenFlag(bool f)						{m_OnScreenFlag=f;}

protected:
	virtual void	setCollisionSize(int _w,int _h);
	virtual void	setCollisionCentreOffset(int _x,int _y)		{m_collisionCentreOffset.vx=_x;m_collisionCentreOffset.vy=_y;}
	virtual void	setCollisionCentreOffset(DVECTOR xy)		{m_collisionCentreOffset=xy;}
	virtual void	setCollisionAngle(int newAngle)				{m_collisionAngle = newAngle;}	// pkg - move to CNpcPlatform?
private:
	DVECTOR			m_collisionSize;
	DVECTOR			m_collisionCentreOffset;
	int				m_collisionRadius;
	CRECT			m_collisionArea;
	DVECTOR			m_collisionCentre;
	s16				m_collisionAngle;															// pkg - move to CNpcPlatform?
	DVECTOR			m_newCollidedPos;															// pkg - to be removed?
	bool			m_OnScreenFlag;

};

/* These are the individual base classes for each of the seperate thing types */
class CPickupThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_PICKUP;}
};
class CPlayerThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_PLAYER;}
};
class CPlayerProjectileThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_PLAYERPROJECTILE;}
};
class CNpcThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_NPC;}
};
class CEnemyThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_ENEMY;}
};
class CEnemyProjectileThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_ENEMYPROJECTILE;}
};
class CPlatformThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_PLATFORM;}
};
class CTriggerThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_TRIGGER;}
	virtual void	setPositionAndSize(int _x,int _y,int _w,int _h);	// Wonder if this might be better in CThing? (pkg)
	virtual void	setTargetBox(int _x,int _y,int _w,int _h);			// Wonder if this might be better in CThing? (pkg)
protected:
	int		m_boxX1,m_boxY1,m_boxX2,m_boxY2;
};
class CHazardThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_HAZARD;}
};
class CFXThing : public CThing
{
public:
	virtual TYPE	getThingType()					{return TYPE_FX;}
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __THING_THING_H__ */

/*===========================================================================
 end */
