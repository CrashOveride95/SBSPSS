/*=========================================================================

	ndogfish.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NDOGFISH_H__
#define	__ENEMY_NDOGFISH_H__

class CNpcIronDogfishEnemy : public CNpcEnemy
{
public:
	virtual void		postInit();
protected:
	virtual void		processTimer( int _frames );
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	void				processStandardIronDogfishAttack( int _frames );
	void				processWalkToUser( int _frames, int speed );
	virtual void		processAttackCollision();
	virtual void		hasBeenSteamed( DVECTOR &steamPos );

	enum NPC_IRON_DOGFISH_STATE
	{
		IRON_DOGFISH_THUMP_1 = 0,
		IRON_DOGFISH_LASER_EYE_1 = 1,
		IRON_DOGFISH_THUMP_2,
		IRON_DOGFISH_ROLL,
		IRON_DOGFISH_LASER_EYE_2,
	};

	s32					m_steamTimer;
};

#endif