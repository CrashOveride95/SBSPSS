/*******************/
/*** Layer Thing ***/
/*******************/

#ifndef	__LAYER_THING_HEADER__
#define	__LAYER_THING_HEADER__

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUIToolbar.h"
#include	"Elem.h"

/*****************************************************************************/
struct	sLayerThingData
{
	bool					WaypointFlag;

	int						Speed;
	int						TurnRate;
	int						Health;
	int						AttackStrength;
	bool					CollisionFlag;
	bool					PlayerFlag;
	int						Spare[8];

};
struct	sLayerThing
{
	GString					Name;
	CList<CPoint>			XY;
	int						ElemID;
	sLayerThingData			Data;

bool	operator==(const char *Name1)	{return (Name==Name1);}
};

/*****************************************************************************/
class	CIni;
class	CLayerThing : public CLayer
{
public:
		enum	MouseMode
		{
			MouseModeNormal=0,
			MouseModeNew,
			MouseModePoints,
		};

		CLayerThing(){};
		CLayerThing(sLayerDef &Def);			// New Layer
		CLayerThing(CFile *File,int Version)	{Load(File,Version);}
		~CLayerThing();

		void			InitLayer(sLayerDef &Def);

virtual	void			InitSubView(CCore *Core);

virtual	void			Render(CCore *Core,Vector3 &CamPos,bool Is3d);
		void			RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d);

virtual	void			GUIInit(CCore *Core);
virtual	void			GUIKill(CCore *Core);
virtual	void			GUIUpdate(CCore *Core);
virtual	void			GUIChanged(CCore *Core);

virtual	void			GUIThingDefClear(){};
virtual	void			GUIThingUpdate(bool OnlySel=false){};
virtual	void			GUIThingUpdateList(CComboBox &List,bool OnlySel=false);
virtual	void			GUIThingPointUpdate(bool OnlySel=false){};
virtual	void			GUIThingPointUpdateList(CListBox &List,bool OnlySel=false);

virtual	void			Load(CFile *File,int Version);
virtual	void			LoadThing(CFile *File,int Version,sLayerThing &ThisThing);
virtual	void			LoadThingNames(CFile *File,int Version);
virtual	void			Save(CFile *File);
virtual	void			SaveThing(CFile *File,sLayerThing &ThisThing);
virtual	void			SaveThingNames(CFile *File);
virtual	void			LoadThingScript(const char *Filename);

virtual	void			Export(CCore *Core,CExport &Exp);
virtual	void			ExportThing(CExport &Exp,sLayerThing &ThisThing)=0;
virtual	void			ExportThingNames(CExport &Exp);

// Functions
virtual	bool			LButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
virtual	bool			RButtonControl(CCore *Core,UINT nFlags, CPoint &point,bool DownFlag);
virtual	bool			MouseMove(CCore *Core,UINT nFlags, CPoint &point);
virtual	bool			Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0);

protected:
virtual	int				FindDefThing(const char *Name);
virtual	void			SetCursor(const char *Name);

		void			RenderThing(CCore *Core,Vector3 &CamPos,sLayerThing	&ThisThing,bool Render3d,bool Selected);
		int				CheckThing(CPoint &Pos);
		void			AddThing(CPoint &Pos);
		int				SelectThing(CPoint &Pos);
		int				SelectThing(int Idx);
		void			DeleteThing();
		int				CheckThingPoint(CPoint &Pos);
		void			AddThingPoint(CPoint &Pos);
		int				SelectThingPoint(CPoint &Pos);

		void			UpdatePos(CPoint &Pos,int Thing,int PosNo,bool Recurs=false);
		void			MovePoint(int Dir);
		void			DeletePoint();
		void			Cancel();

		CIni				ThingScript;
		CElemBank			*ThingBank;
		CList<sLayerThing>	DefList;
		CList<sLayerThing>	ThingList;
		int					CurrentDefThing;
		int					CurrentThing,CurrentThingPoint;
		MouseMode			Mode;
		sLayerThing			Cursor;
		bool				DrawPoints;
};

/*****************************************************************************/
#endif
