/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>
#include	"gl3d.h"
#include	"Layer.h"
#include	"LayerBack.h"

#include	"TexCache.h"
#include	"TileSet.h"


/*****************************************************************************/
enum MOUSE_MODE
{
	MOUSE_MODE_NONE=0,
	MOUSE_MODE_LMB_EDIT,
	MOUSE_MODE_MMB_EDIT,
	MOUSE_MODE_RMB_EDIT,
	
};


/*****************************************************************************/
class	CMapEditView;
class	CCore
{
public:
		CCore();
		~CCore();
// Control
		void				LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void				MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void				RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void				MouseWheel(UINT nFlags, short zDelta, CPoint &pt);
		void				MouseMove(UINT nFlags, CPoint &point);

// Blah
		void				Init(CMapEditView *Wnd);
		void				Render();
		void				UpdateView(float XOfs,float YOfs,float ZOfs);
		void				SetMouseMode(MOUSE_MODE CurrentMode,MOUSE_MODE NewMode);		

		Vec					&GetCamPos()	{return(CamPos);}

		void				SetTileView(BOOL f);
		BOOL				GetTileView()				{return(TileViewFlag);}
		void				ToggleTileView();

		void				SetLayerPalette(BOOL f);
		BOOL				GetLayerPalette()				{return(LayerPaletteFlag);}
		void				ToggleLayerPalette();

// Layers
		void				LayerSetActive(int Layer);
		int					LayerGetActive();
		CLayer				*LayerGet(int i);

// Tex Cache
		CTexCache			&GetTexCache()	{return(TexCache);}


private:
		CMapEditView			*ParentWindow;	
		MOUSE_MODE				MouseMode;
		CPoint					CurrentMousePos,LastMousePos;
		Vec						CamPos;

		CLayer					*Layers[LAYER_TYPE_MAX];
		int						ActiveLayer;

		std::vector<CTileSet>	TileSet;
		CTexCache				TexCache;

		BOOL					TileViewFlag;
		BOOL					LayerPaletteFlag;


};

/*****************************************************************************/
#endif
