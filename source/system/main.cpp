/****************/
/*** PSX Main ***/
/****************/

#include <libmcrd.h>

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"pad\pads.h"
#include 	"system\vid.h"
#include 	"gfx\prim.h"
#include 	"gfx\tpage.h"
#include	"utils\utils.h"

#include 	"system\gp.h"

// scenes
#include "game\game.h"

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifdef __USER_paul__
#include "paul\paul.h"
CPaulScene s_paulScene;
#endif

#ifndef __SYSTEM_EXCEPT_H__
#include "system\except.h"
#endif



#define	SCREEN_GRAB


/*****************************************************************************/

void	SaveScreen(RECT R);

/*****************************************************************************/

void	InitSystem()	// reordered to reduce black screen (hope all is well
{
	ResetCallback();
	SaveGP();
	SetSp(GetSp()|0x807f0000);
//	SetDispMask(0);

	MemInit();
//	MemCardInit( 1 );
//	MemCardStart();
	PadsInit();
//	MemCardStop();

	CFileIO::Init();
	TranslationDatabase::initialise(false);
	TranslationDatabase::loadLanguage(ENGLISH);
	PrimInit();
	TPInit();
	VidInit();

#ifdef __USER_paul__
	installExceptionHandler();			// Where is the earliest we can do this?
#endif

	setRndSeed( VidGetTickCount() );

	SetDispMask(1);

	GameState::initialise();

	CSoundMediator::initialise();

#ifdef __USER_paul__
s_paulScene.init();
#endif
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int thing=50;
int	bug=0;
static int	s_time = 0;
void dumpDebugMem();

void	MainLoop()
{
	while (1)
	{
		int	frames;

		frames=GameState::getFramesSinceLast();
		
		GameState::think();
		GameState::render();

		CSoundMediator::think(frames);
		
#ifdef __USER_paul__
		s_paulScene.think(frames);
		s_paulScene.render();
#endif

		while(DrawSync(1));

		VSync(0);

		VidSwapDraw();
		PrimDisplay();
		PadUpdate();

		DbgPollHost();

#ifdef __USER_paul__
		bug=100/thing;
		thing--;
#endif

#if defined(__VERSION_DEBUG__)

	#if defined(__DEBUG_MEM__)
		dumpDebugMem();
	#endif

	#if		defined(SCREEN_GRAB)
		if (PadGetHeld(0) & PAD_L2)
			if (PadGetDown(0) & PAD_START) SaveScreen(VidGetScreen()->Draw.clip);
	#endif

		if (PadGetDown(0) & PAD_SELECT) VRamViewer();
#endif
	}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int 	main()
{
	CFileIO::GetAllFilePos();
	InitSystem();
	
	GameState::setNextScene( &GameScene );

//	CXAStream::Init();			// PKG - Stuck here so that it doesn't affect any startup stuff (7/8/00)
	MainLoop();

	return(0);

}

/*****************************************************************************/
#if	defined(SCREEN_GRAB)
#if defined(__VERSION_DEBUG__)
struct	sTgaHdr
{
	char	id;			   // 0
	char	colmaptype;	   // 1
	char	imagetype;	   // 2
	char	fei[2];		   // 3
	char	cml[2];		   // 5
	char	cmes;		   // 7
	short	xorig;		   // 8
	short	yorig;		   // 10
	short	width;		   // 12
	short	height;		   // 14
	char	depth;		   // 15
	char	imagedesc;	   // 16
};

bool FileExists(char const * Name)
{
	int		FileHnd;

	FileHnd=PCopen((char *)Name,0,0);

	if (FileHnd!=-1)
		{
		PCclose(FileHnd);
		return true;
		}
	else
		return false;
}

void SaveScreen(RECT SR)
{
int				FileHnd;
static	int		ScreenNo=0;
sTgaHdr			FileHdr;
int				W=SR.w;
int				H=SR.h;
char			Filename[32];

		sprintf( Filename, "PRLSR%04d.tga", ScreenNo );
		while (FileExists( Filename ) )
			{
			ScreenNo++;
			sprintf( Filename, "PRLSR%04d.tga", ScreenNo );
			}

		FileHnd=PCcreat((char *)Filename,0);
		ASSERT(FileHnd != -1);
		
//---------------------------------------------------------------------------
// Header
		memset(&FileHdr,0 ,sizeof(sTgaHdr));
	
		FileHdr.imagetype= 2;  //imagetype
		FileHdr.width = W;
		FileHdr.height= H;
		FileHdr.depth=24;
//		FileHdr.imagedesc=24;

		PCwrite(FileHnd,(char *)&FileHdr,sizeof(sTgaHdr));

//---------------------------------------------------------------------------
// Data
int		x,y;
u16		InBuffer[1024];
u8		OutBuffer[1024*3];

		SR.y+=SR.h;
		SR.h=1;
		for (y=0; y<H; y++)
			{
			SR.y--;
			StoreImage(&SR,(u32*)InBuffer);

			for (x=0; x<W; x++)
				{
				u16	Col;
				u8	R,G,B;
				Col=InBuffer[x];

				R=Col&0x1f;
				G=(Col>>5)&0x1f;
				B=(Col>>10)&0x1f;

				R=R*255/31;
				G=G*255/31;
				B=B*255/31;

				OutBuffer[(x*3)+0]=B;
				OutBuffer[(x*3)+1]=G;
				OutBuffer[(x*3)+2]=R;
				}
			PCwrite(FileHnd,(char *)OutBuffer,W*3);
			}

//---------------------------------------------------------------------------
		PCclose(FileHnd);


}
#endif
#endif

