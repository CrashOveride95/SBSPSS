/*=========================================================================

	nclam.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NCLAM_H__
#define	__ENEMY_NCLAM_H__

class CNpcClamEnemy : public CNpcEnemy
{
public:
	virtual u8			canCollideWithEnemy()					{return( false );}
protected:
	virtual void		processUserCollision( CThing *thisThing );
	virtual void		processEnemyCollision( CThing *thisThing );
	virtual bool		processSensor();
};

class CNpcJumpingClamEnemy : public CNpcClamEnemy
{
public:
	virtual void		postInit();
protected:
	virtual void		processClose( int _frames );
};

class CNpcStaticClamEnemy : public CNpcClamEnemy
{
public:
	virtual void		postInit();
	virtual u8			hasBeenAttacked()						{return( false );}
protected:
	virtual void		collidedWith(CThing *_thisThing);
	virtual void		processClose( int _frames );

	u8					m_isStunned;
	u8					m_isAnimating;
};

#endif