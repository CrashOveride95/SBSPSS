/*=========================================================================

	projectl.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PROJECTL_H__
#define	__PROJECTL_PROJECTL_H__

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

/*****************************************************************************/

class	CProjectile : public CEnemyProjectileThing
{
public:
	enum PROJECTILE_MOVEMENT_TYPE
	{
		PROJECTILE_FIXED = 0,
		PROJECTILE_DUMBFIRE = 1,
		PROJECTILE_USER_SEEK,
		PROJECTILE_GAS_CLOUD,
	};

	enum PROJECTILE_LIFETIME_TYPE
	{
		PROJECTILE_FINITE_LIFE = 0,
		PROJECTILE_INFINITE_LIFE = 1,
	};

	enum PROJECTILE_STATE
	{
		PROJECTILE_ATTACK = 0,
		PROJECTILE_RETURN = 1,
	};

	void				init();
	void				init( DVECTOR initPos, s16 initHeading );
	void				init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType );
	void				init( DVECTOR initPos, s16 initHeading, PROJECTILE_MOVEMENT_TYPE initMoveType, PROJECTILE_LIFETIME_TYPE initLifeType, s32 initLifetime );
	void				shutdown();
	void				setToShutdown();
	u8					isSetToShutdown()										{return( m_isShuttingDown );}
	void				think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );
	void				setMovementType( PROJECTILE_MOVEMENT_TYPE moveType );
	PROJECTILE_MOVEMENT_TYPE	getMovementType();
	void				setState( PROJECTILE_STATE newState );
	void				setLifeTime( PROJECTILE_LIFETIME_TYPE lifeType );
	void				setPosition( DVECTOR newPos );
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}

private:
	class CLayerCollision	*m_layerCollision;

protected:
	DVECTOR				getScreenOffset();
	bool				processTargetSeek( int _frames, DVECTOR targetPos );
	void				collidedWith( CThing *_thisThing );

	class SpriteBank							*m_spriteBank;
	DVECTOR										m_initPos;
	s16											m_heading;
	s32											m_lifetime;
	s32											m_extension;
	PROJECTILE_MOVEMENT_TYPE					m_movementType;
	PROJECTILE_LIFETIME_TYPE					m_lifetimeType;
	PROJECTILE_STATE							m_state;
	u16											m_turnSpeed;
	u8											m_isShuttingDown;
};

/*****************************************************************************/

class	CPlayerProjectile : public CPlayerProjectileThing
{
public:
	enum PLAYER_PROJECTILE_MOVEMENT_TYPE
	{
		PLAYER_PROJECTILE_DUMBFIRE = 0,
	};

	enum PLAYER_PROJECTILE_LIFETIME_TYPE
	{
		PLAYER_PROJECTILE_FINITE_LIFE = 0,
		PLAYER_PROJECTILE_INFINITE_LIFE = 1,
	};

	void				init();
	void				init( DVECTOR initPos, s16 initHeading );
	void				init( DVECTOR initPos, s16 initHeading, PLAYER_PROJECTILE_MOVEMENT_TYPE initMoveType, PLAYER_PROJECTILE_LIFETIME_TYPE initLifeType );
	void				init( DVECTOR initPos, s16 initHeading, PLAYER_PROJECTILE_MOVEMENT_TYPE initMoveType, PLAYER_PROJECTILE_LIFETIME_TYPE initLifeType, s32 initLifetime );
	void				shutdown();
	void				setToShutdown();
	u8					isSetToShutdown()										{return( m_isShuttingDown );}
	virtual void		think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );
	void				setMovementType( PLAYER_PROJECTILE_MOVEMENT_TYPE moveType );
	PLAYER_PROJECTILE_MOVEMENT_TYPE		getMovementType();
	void				setLifeTime( PLAYER_PROJECTILE_LIFETIME_TYPE lifeType );
	void				setPosition( DVECTOR newPos );
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}

private:
	class CLayerCollision	*m_layerCollision;

protected:
	DVECTOR				getScreenOffset();
	void				collidedWith( CThing *_thisThing );

	class SpriteBank							*m_spriteBank;
	DVECTOR										m_initPos;
	s16											m_heading;
	s32											m_lifetime;
	s32											m_extension;
	PLAYER_PROJECTILE_MOVEMENT_TYPE				m_movementType;
	PLAYER_PROJECTILE_LIFETIME_TYPE				m_lifetimeType;
	u16											m_turnSpeed;
	int											m_frame;
	POLY_FT4									*SprFrame;
	u8											m_reversed;
	u8											m_isShuttingDown;
};


/*****************************************************************************/
#endif
