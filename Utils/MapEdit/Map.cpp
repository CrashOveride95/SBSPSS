/******************/
/*** Map Stuph ***/
/*****************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>

#include	"Map.h"


/*****************************************************************************/
int		CMap::GetWidth()
{
	return(Map.size());
}

/*****************************************************************************/
int		CMap::GetHeight()
{
	return(Map[0].size());
}

/*****************************************************************************/
void	CMap::SetSize(int Width,int Height)
{
	Map.resize(Width);
	for (int i=0;i<Width;i++)
	{
		Map[i].resize(Height);
	}

	for (int Y=0;Y<Height;Y++)
		for (int X=0;X<Width;X++)
		{
			Map[X][Y].Bank=0;
			Map[X][Y].Tile=(X+Y)%9;
		}

}

/*****************************************************************************/
void	CMap::SetWidth(int Width)
{
}

/*****************************************************************************/
void	CMap::SetHeight(int Height)
{
}


/*****************************************************************************/

