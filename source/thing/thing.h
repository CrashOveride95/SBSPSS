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

// Thing manager class
class CThingManager
{
public:
	static void		init();
	static void		shutdown();

	static void		thinkAllThings(int _frames);
	static void		renderAllThings();
	static void		processEventAllThings(GAME_EVENT _event,class CThing *_sourceThing);

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

// Linkage
	void			addChild(CThing *Child);
	void			removeChild(CThing *Child);
	void			removeAllChild();
	bool			hasChild(CThing *Child);


	DVECTOR			getPos()						{return Pos;}
	virtual void	shove(DVECTOR move);
	CThing			*getNext()						{return Next;}


	virtual void	processEvent(GAME_EVENT _event,CThing *_sourceThing);


protected:

	
// Linkage
		CThing		*Parent,*Next;

// Pos
		DVECTOR		Pos, PosDelta;

public:
		class CThing			*m_nextThing;


		
		

	// -- Collision --
public:
	virtual int		canCollide()								{return true;}
	virtual int		checkCollisionAgainst(CThing *_thisThing);
	void			updateCollisionArea();
	virtual void	collidedWith(CThing *_thisThing)			{;}
	s32				getNewYPos( CThing *_thisThing );
	void			setNewCollidedPos(DVECTOR newPos)			{m_newCollidedPos = newPos;}
protected:
	typedef struct
	{
		int	x1,y1,x2,y2;
	}
	CRECT;

	void			setCollisionSize(int _w,int _h);
	void			setCollisionCentreOffset(int _x,int _y)		{m_collisionCentreOffset.vx=_x;m_collisionCentreOffset.vy=_y;}
	void			setCollisionCentreOffset(DVECTOR xy)		{m_collisionCentreOffset=xy;}
	void			setCentreCollision(bool newCentreCollision)	{m_centreCollision = newCentreCollision;}
	void			setCollisionAngle(int newAngle)				{m_collisionAngle = newAngle;}
	int				getCollisionRadius()						{return m_collisionRadius;}
	CRECT			getCollisionArea()							{return m_collisionArea;}
	DVECTOR			getCollisionCentre()						{return m_collisionCentre;}
	s16				getCollisionAngle()							{return m_collisionAngle;}
	bool			getCentreCollision()						{return m_centreCollision;}
	DVECTOR			getNewCollidedPos()							{return m_newCollidedPos;}
private:
	DVECTOR			m_collisionSize;
	DVECTOR			m_collisionCentreOffset;
	int				m_collisionRadius;
	CRECT			m_collisionArea;
	DVECTOR			m_collisionCentre;
	s16				m_collisionAngle;
	bool			m_centreCollision;
	DVECTOR			m_newCollidedPos;

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
