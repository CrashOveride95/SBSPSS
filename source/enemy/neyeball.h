/*=========================================================================

	neyeball.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NEYEBALL_H__
#define	__ENEMY_NEYEBALL_H__

class CNpcEyeballEnemy : public CNpcEnemy
{
public:
	virtual void		render();
	void				postInit();
	void				shutdown();
	virtual int			getFrameCount()									{return( 1 );}
	virtual void		processEvent( GAME_EVENT evt, CThing *sourceThing );
protected:
	virtual void		processEnemyCollision( CThing *thisThing );
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processShotDeathStart( int _frames );
	virtual void		processShotDeathEnd( int _frames );

	s32					m_fireHeading;

	enum
	{
		EYEBALL_DIST = 30,
	};
};

#endif