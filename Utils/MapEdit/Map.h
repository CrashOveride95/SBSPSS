/******************/
/*** Map Stuph ***/
/*****************/

#ifndef	__MAP_HEADER__
#define	__MAP_HEADER__

#include	<Vector>

struct	sMapElem
{
	int		Set;
	int		Tile;
	int		Flags;
};

enum
{
	MapElemFlagMirrorX=	1<<0,
	MapElemFlagMirrorY=	1<<1,
};

/*****************************************************************************/
class	CMap
{
public:
		CMap(){};
		~CMap(){};

		int			GetWidth();
		int			GetHeight();
		BOOL		IsValid()			{return(GetHeight());}

		void		SetSize(int Width,int Height,BOOL Clear=FALSE);
		void		SetWidth(int Width);
		void		SetHeight(int Height);
		void		Clear();
		void		Delete();
		void		MirrorX(int Flag);
		void		MirrorY(int Flag);

		sMapElem	&Get(int X,int Y);
		void		Set(int X,int Y,sMapElem &Blk);
		void		Set(int X,int Y,CMap &Blk);
		void		Set(CMap &Src,int StartX,int StartY,int Width,int Height);

		void		Resize(int Width,int Height);

		BOOL		DoesContainTile(sMapElem &Tile);

		void		Load(CFile *File,float Version);
		void		Save(CFile *File);


inline	void	operator=(CMap &Src)	
{
int		Width=Src.GetWidth();
int		Height=Src.GetHeight();
		Delete();
		SetSize(Width,Height);
		Set(Src,0,0,Width,Height);
}

protected:
		std::vector< std::vector<sMapElem> > Map;
};


/*****************************************************************************/
#endif
