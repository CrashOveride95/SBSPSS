/**********************/
/*** Main Game File ***/
/**********************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"pad\pads.h"
#include 	"system\vid.h"
#include 	"gfx\prim.h"
#include	"utils\utils.h"
#include	"level\level.h"
#include	"game\game.h"
#include	"system\gstate.h"
#include	"gfx\font.h"
#include	"gfx\fdata.h"

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif
#include "player\demoplay.h"

#ifndef	__ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NPLATFRM_H__
#include "enemy\nplatfrm.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __GFX_BUBICLES__
#include "gfx\bubicles.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef _ANIMTEX_HEADER_
#include "gfx\animtex.h"
#endif

#ifndef	__GAME_PAUSE_H__
#include "game\pause.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef __PICKUPS_PICKUP_H__
#include "pickups\pickup.h"
#endif


#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#include "gfx\actor.h"

int			RenderZ=256;

/*****************************************************************************/

FontBank	*CGameScene::s_genericFont;
MATRIX		CGameScene::CamMtx;

/*****************************************************************************/

int s_globalLevelSelectThing=0;
int CGameScene::s_readyToExit;
int CGameScene::s_levelFinished;

/*****************************************************************************/

CGameScene	GameScene;

/*****************************************************************************/
void	CGameScene::AspectCorrectCamera()
{
const s32 Scale = (512<<12)/(256);

		CamMtx.m[0][0] = ((s32)CamMtx.m[0][0] * Scale)>>12;
		CamMtx.m[0][1] = ((s32)CamMtx.m[0][1] * Scale)>>12;
		CamMtx.m[0][2] = ((s32)CamMtx.m[0][2] * Scale)>>12;

		CamMtx.t[0]  = (CamMtx.t[0] * Scale)>>12;
}

/*****************************************************************************/
void 	CGameScene::init()
{
// Setup Constant Camera Matrix
//		SetIdentNoTrans(&CamMtx);
//		CamMtx.t[2]=ZPos;
//		AspectCorrectCamera();
//		SetRotMatrix(&CamMtx);
//		SetTransMatrix(&CamMtx);

		SetGeomScreen(RenderZ);
		CamMtx.t[2]=RenderZ;
		SetTransMatrix(&CamMtx);		

		s_genericFont=new ("CGameScene::Init") FontBank();
		s_genericFont->initialise( &standardFont );
		s_genericFont->setColour( 255, 255 , 0 );
		VidSetClearScreen(0);



		m_pauseMenu=new ("Pause Menu") CPauseMenu();
		m_pauseMenu->init();

		s_readyToExit=false;

		CFader::setFadingIn();
		initLevel();
}

/*****************************************************************************/
// This is a seperate funtion ( and virtual ) so that we can overload it for
// the demo mode (pkg)
// NB: This function should *only* include the new function - Don't add anything else here!
void	CGameScene::createPlayer()
{
		m_player=new ("player") CPlayer();
}

// This is also to be overloaded for demomode.. to stop the pause menu appearing
int		CGameScene::canPause()
{
		return true;
}


/*****************************************************************************/

void	CGameScene::shutdown()
{
		shutdownLevel();
		CSoundMediator::dumpSong();

		m_pauseMenu->shutdown();	delete m_pauseMenu;
		s_genericFont->dump();		delete s_genericFont;
}

/*****************************************************************************/
void 	CGameScene::render()
{
//		CamMtx.t[2]=ZPos;	// Temp

		m_pauseMenu->render();
		CConversation::render();
		CThingManager::renderAllThings();
		Level.render();
}

/*****************************************************************************/
void	CGameScene::think(int _frames)
{
//#ifdef __USER_paul__
//	if(!CConversation::isActive()&&PadGetDown(0)&PAD_START)
//	{
//		CConversation::trigger(SCRIPTS_SPEECHTEST_DAT);
//	}
//#endif

	if(s_readyToExit)
	{
		// Temporarily.. exiting game scene always goes back to the front end (pkg)
		GameState::setNextScene(&FrontEndScene);
	}
	else if(s_levelFinished)
	{
		s_globalLevelSelectThing++;
		shutdownLevel();
		initLevel();
	s_levelFinished=false;
	}


	if(!m_pauseMenu->isActive()&&PadGetDown(0)&PAD_START&&canPause())
	{
		m_pauseMenu->select();
	}

/*	if (!s_levelFinished) */CConversation::think(_frames);
/*	if (!s_levelFinished) */m_pauseMenu->think(_frames);
	if(!CConversation::isActive()&& !m_pauseMenu->isActive())
	{
		DVECTOR	camPos;
/*		if (!s_levelFinished) */CThingManager::thinkAllThings(_frames);
/*		if (!s_levelFinished) */camPos=m_player->getCameraPos();
/*		if (!s_levelFinished) */CBubicleFactory::setMapOffset(&camPos);
/*		if (!s_levelFinished) */Level.setCameraCentre(camPos);
/*		if (!s_levelFinished) */Level.think(_frames);

		if(PadGetDown(0)&PAD_R2)
		{
			levelFinished();
		}
	}
//	s_levelFinished=false;
}


/*****************************************************************************/
int		CGameScene::readyToShutdown()
{
	return s_readyToExit;
}

/*****************************************************************************/
CPlayer	* CGameScene::getPlayer()
{
	return( m_player );
}

/*****************************************************************************/
void CGameScene::sendEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CThingManager::processEventAllThings(evt, sourceThing);
}



/*****************************************************************************/
void	CGameScene::initLevel()
{
	printf("InitLevel\n");
	CThingManager::init();

	CConversation::init();
	CConversation::registerConversationScript(SCRIPTS_SPEECHTEST_DAT);	// Register one script for testing..

	s_levelFinished=false;
	Level.init(s_globalLevelSelectThing);

	createPlayer();
	m_player->setRespawnPos(Level.getPlayerSpawnPos());
	m_player->init();
	m_player->setLayerCollision(Level.getCollisionLayer());
	m_player->setMapSize(Level.getMapSize());

// Init actors (needs moving and tidying
	int pointNum;
	int actorNum;
	int platformNum;

	sThingActor **actorList = Level.getActorList();
	if (actorList)
	{
		for ( actorNum = 0 ; actorNum < Level.getActorCount() ; actorNum++ )
		{
			sThingActor	*ThisActor=actorList[actorNum];
			CActorPool::ACTOR_TYPE actorType = CActorPool::getActorType( actorList[actorNum]->Type );
			switch ( actorType )
			{
				case CActorPool::ACTOR_ENEMY_NPC:
					{
						CNpcEnemy *enemy;
						enemy=CNpcEnemy::Create(ThisActor);
						enemy->setLayerCollision( Level.getCollisionLayer() );
						enemy->setupWaypoints( ThisActor );
						enemy->postInit();
					}
					break;

				default:
					break;
			}
		}
	}

	sThingPlatform **platformList = Level.getPlatformList();
	if (platformList)
	{
		for ( platformNum = 0 ; platformNum < Level.getPlatformCount() ; platformNum++ )
		{
			CNpcPlatform *platform;
			platform = new ("platform") CNpcPlatform;
			ASSERT(platform);
			platform->setTypeFromMapEdit( platformList[platformNum]->Type );

			u16	*PntList=(u16*)MakePtr(platformList[platformNum],sizeof(sThingPlatform));

			u16 newXPos, newYPos;

			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			DVECTOR startPos;
			startPos.vx = newXPos << 4;
			startPos.vy = newYPos << 4;

			platform->init( startPos );
			platform->setLayerCollision( Level.getCollisionLayer() );
			platform->setTiltable( false );

			platform->addWaypoint( newXPos, newYPos );

			if ( platformList[platformNum]->PointCount > 1 )
			{
				for ( pointNum = 1 ; pointNum < platformList[platformNum]->PointCount ; pointNum++ )
				{
					newXPos = (u16) *PntList;
					PntList++;
					newYPos = (u16) *PntList;
					PntList++;

					platform->addWaypoint( newXPos, newYPos );
				}
			}

		}
	}

	// Song is loaded/dumped by the level, and played from here. This just gives some
	// better timing over when it starts (pkg)
	CSoundMediator::playSong();
	printf("InitLevelDone\n");

}


/*****************************************************************************/
void	CGameScene::shutdownLevel()
{
	CConversation::shutdown();
	CThingManager::shutdown();
	Level.shutdown();
}

/*****************************************************************************/