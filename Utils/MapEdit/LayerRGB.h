/*******************/
/*** Layer RGB ***/
/*******************/

#ifndef	__LAYER_RGB_HEADER__
#define	__LAYER_RGB_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUILayerRGB.h"
#include	"Elem.h"
#include	"ExportHdr.h"

struct sRGBElem
{
	u8	R,G,B;
};

/*****************************************************************************/
class	CCore;
class	CLayerRGB : public CLayer
{

public:
		enum
		{
			GUI_MODE_PAINT=0,
			GUI_MODE_TINT,
			GUI_MODE_LIGHTEN,
			GUI_MODE_DARKEN,

			GUI_MODE_MAX
		};
		enum
		{
			RGB_BRUSH_MAX=8,
		};

		struct	sRGBBrush
		{
				int	WH;
				int	XYOfs;
				u8	*Gfx;
		};


		CLayerRGB(sLayerDef &Def);
		CLayerRGB(CFile *File,int Version)	{Load(File,Version);}
		~CLayerRGB();

		void			InitLayer(sLayerDef &Def);

		void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			RenderGrid(CCore *Core,Vector3 &CamPos,bool Active){};
		void			RenderSelection(CCore *Core,Vector3 &ThisCam){};

		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);

		void			GUIInit(CCore *Core);
		void			GUIKill(CCore *Core);
		void			GUIUpdate(CCore *Core);
		void			GUIChanged(CCore *Core);

		void			CheckLayerSize(int Width,int Height);
		bool			Resize(int Width,int Height);

		void			Load(CFile *File,int Version);
		void			Save(CFile *File);

		void			Export(CCore *Core,CExport &Exp);
virtual	void			LoadGfx(CCore *Core){}

		void			SetSize(int Width,int Height,BOOL ClearFlag);
		void			Clear();


// Functions
		bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
		bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag);
		bool			MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos);

protected:
		void			Render(CCore *Core,Vector3 &CamPos,CMap &ThisMap,bool Render3d,float Alpha=1.0f,Vector3 *Ofs=0);
		void			Paint(CCore *Core,CPoint &CursorPos);
		void			Grab(CCore *Core,CPoint &CursorPos);
		sRGBElem const	&GetRGB(int X,int Y)		{return(Map[X][Y]);}

		CGUILayerRGB		GUIRGB;
		
		sRGBElem		CurrentRGB;
		int				CurrentBrush;
		int				CurrentMode;
		bool			ShadeFlag;

		int				MapWidth,MapHeight;
		std::vector< std::vector<sRGBElem> > Map;

		CPoint			LastCursPos;
static	char			*RGBModeName[GUI_MODE_MAX];
static	sRGBBrush		RGBBrushTable[CLayerRGB::RGB_BRUSH_MAX];

		
};

/*****************************************************************************/
#endif
