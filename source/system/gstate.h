/*=========================================================================

	gstate.h

	Author:		PKG
	Created:
	Project:	PRLSR
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __SYSTEM_GSTATE_H__
#define __SYSTEM_GSTATE_H__







/*****************************************************************************/
class CScene
{
public:
			CScene()	{;}
	virtual ~CScene()	{;}

	virtual void	init()=0;
	virtual void	shutdown()=0;
	virtual void	render()=0;
	virtual void	think(int _frames)=0;
	virtual char	*getSceneName()=0;

protected:
};


/*****************************************************************************/
class GameState
{
public:
	static void		initialise();
	static void		think();
	static void		render();

	static void		setNextScene( CScene *_nextScene );

	inline static long int	getTimeSinceLast()		{return s_timeSinceLast;}
	inline static long int	getFramesSinceLast()	{return (s_timeSinceLast>>12)+1;}

	static void		setTimeSpeed( int speed );

	static CScene *	getCurrentScene();

#if defined(__TERRITORY_USA__) || defined(__TERRITORY_JAP__)
	static int		getOneSecondInFrames()			{return 60;}
#else
	static int		getOneSecondInFrames()			{return 50;}
#endif

private:
	// Try and instantiate a GameState and you will fail miserably :)
			GameState();

	static void		updateTimer();
	static	int		s_timeSinceLast;

};


#endif	/* __SYSTEM_GSTATE_H__ */

/*===========================================================================
 end */
