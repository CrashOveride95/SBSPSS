/*=========================================================================

	fdata.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef	__ANIM_BARNACLEBOY_HEADER__
#include <ACTOR_BARNACLEBOY_Anim.h>
#endif

#ifndef	__ANIM_KRUSTY_HEADER__
#include <ACTOR_KRUSTY_Anim.h>
#endif

#ifndef	__ANIM_SQUIDWARD_HEADER__
#include <ACTOR_SQUIDWARD_Anim.h>
#endif

CNpcFriend::NPC_FRIEND_DATA CNpcFriend::m_data[NPC_FRIEND_UNIT_TYPE_MAX] =
{
	{	// NPC_FRIEND_BARNACLE_BOY
		ACTORS_CLAM_SBK,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		ANIM_BARNACLEBOY_IDLE,
	},

	{	// NPC_FRIEND_GARY
		ACTORS_CLAM_SBK,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,
	},

	{	// NPC_FRIEND_KRUSTY
		ACTORS_CLAM_SBK,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		ANIM_KRUSTY_IDLEJIG,
	},

	{	// NPC_FRIEND_MERMAID_MAN
		ACTORS_CLAM_SBK,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,
	},

	{	// NPC_FRIEND_PATRICK
		ACTORS_CLAM_SBK,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,
	},

	{	// NPC_FRIEND_SANDY_CHEEKS
		ACTORS_CLAM_SBK,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		0,
	},

	{	// NPC_FRIEND_SQUIDWARD
		ACTORS_SQUIDWARD_SBK,
		true,
		3,
		128,
		false,
		DAMAGE__HIT_ENEMY,
		ANIM_SQUIDWARD_IDLEBREATHE,
	},
};

CNpcFriend::NPC_FRIEND_UNIT_TYPE CNpcFriend::mapEditConvertTable[NPC_FRIEND_UNIT_TYPE_MAX] =
{
	NPC_FRIEND_BARNACLE_BOY,
	NPC_FRIEND_GARY,
	NPC_FRIEND_KRUSTY,
	NPC_FRIEND_MERMAID_MAN,
	NPC_FRIEND_PATRICK,
	NPC_FRIEND_SANDY_CHEEKS,
	NPC_FRIEND_SQUIDWARD,
};

