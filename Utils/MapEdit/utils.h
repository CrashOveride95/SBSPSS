/*************/
/*** Utils ***/
/*************/

#ifndef	__UTILS_HEADER__
#define	__UTILS_HEADER__

#include	"maths.h"


/**************************************************************************************/
void	DbgMsg(const char * pszFmt,...);

void	BuildGLBox(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax);
void	BuildGLBoxNoNormals(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax);
void	BuildGLQuad(float XMin,float XMax,float YMin,float YMax,float Z);
int		LoadGLTexture(char *FileName, GLuint &Text);

void	TNormalise(TVECTOR &V);
TVECTOR	TCrossProduct(TVECTOR const &V0,TVECTOR const &V1,const TVECTOR &V2 );

CPoint	IDToPoint(int ID,int Width);
int		PointToID(CPoint &Pnt,int Width);

/**************************************************************************************/

#endif