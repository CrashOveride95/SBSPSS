; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMapEditDoc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mapedit.h"
LastPage=0

ClassCount=15
Class1=CAddLayerDlg
Class2=CChildFrame
Class3=CGLEnabledView
Class4=CLayerList
Class5=CLayerShadeGUI
Class6=CLayerTileGUI
Class7=CLayerTileToolbar
Class8=CMainFrame
Class9=CMapEditApp
Class10=CAboutDlg
Class11=CMapEditDoc
Class12=CMapEditView
Class13=CMapSizeDlg
Class14=CMultiBar
Class15=CNewMapGUI

ResourceCount=12
Resource1=IDD_ABOUTBOX (English (U.S.))
Resource2=IDD_MULTIBAR (English (U.S.))
Resource3=IDR_MAPEDITYPE (English (U.S.))
Resource4=IDD_LAYER_LIST_DIALOG
Resource5=IDD_MAPSIZE
Resource6=IDD_NEWMAP
Resource7=IDD_ADDLAYER
Resource8=IDD_LAYERTILE_GUI
Resource9=IDD_LAYERTILE_TOOLBAR
Resource10=IDR_TOOLBAR (English (U.S.))
Resource11=IDR_MAINFRAME (English (U.S.))
Resource12=IDD_LAYERSHADE_GUI

[CLS:CAddLayerDlg]
Type=0
BaseClass=CDialog
HeaderFile=AddLayerDlg.h
ImplementationFile=AddLayerDlg.cpp

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CGLEnabledView]
Type=0
BaseClass=CView
HeaderFile=GLEnabledView.h
ImplementationFile=GLEnabledView.cpp

[CLS:CLayerList]
Type=0
HeaderFile=LayerList.h
ImplementationFile=LayerList.cpp

[CLS:CLayerShadeGUI]
Type=0
BaseClass=CDialog
HeaderFile=LayerShadeGUI.h
ImplementationFile=LayerShadeGUI.cpp

[CLS:CLayerTileGUI]
Type=0
BaseClass=CDialog
HeaderFile=LayerTileGUI.h
ImplementationFile=LayerTileGUI.cpp

[CLS:CLayerTileToolbar]
Type=0
BaseClass=CDialog
HeaderFile=LayerTileToolbar.h
ImplementationFile=LayerTileToolbar.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:CMapEditApp]
Type=0
BaseClass=CWinApp
HeaderFile=MapEdit.h
ImplementationFile=MapEdit.cpp
LastObject=CMapEditApp
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MapEdit.cpp
ImplementationFile=MapEdit.cpp
LastObject=CAboutDlg

[CLS:CMapEditDoc]
Type=0
BaseClass=CDocument
HeaderFile=MapEditDoc.h
ImplementationFile=MapEditDoc.cpp
LastObject=CMapEditDoc
Filter=N
VirtualFilter=DC

[CLS:CMapEditView]
Type=0
BaseClass=CGLEnabledView
HeaderFile=MapEditView.h
ImplementationFile=MapEditView.cpp
LastObject=CMapEditView
Filter=C
VirtualFilter=VWC

[CLS:CMapSizeDlg]
Type=0
BaseClass=CDialog
HeaderFile=MapSizeDlg.h
ImplementationFile=MapSizeDlg.cpp

[CLS:CMultiBar]
Type=0
BaseClass=CDialogBar
HeaderFile=MultiBar.h
ImplementationFile=MultiBar.cpp

[CLS:CNewMapGUI]
Type=0
BaseClass=CDialog
HeaderFile=NewMapGUI.h
ImplementationFile=NewMapGUI.cpp

[DLG:IDD_ADDLAYER]
Type=1
Class=CAddLayerDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ADDLAYER_LIST,listbox,1352728833

[DLG:IDD_LAYER_LIST_DIALOG]
Type=1
Class=CLayerList
ControlCount=3
Control1=IDC_LAYER_LIST,listbox,1352728913
Control2=IDC_LAYERLIST_ADD,button,1342242816
Control3=IDC_LAYERLIST_DELETE,button,1342242816

[DLG:IDD_LAYERSHADE_GUI]
Type=1
Class=CLayerShadeGUI
ControlCount=8
Control1=IDC_LAYERSHADE_TEXT0,static,1342308352
Control2=IDC_LAYERSHADE_EDITR0,edit,1350770688
Control3=IDC_LAYERSHADE_EDITG0,edit,1350770816
Control4=IDC_LAYERSHADE_EDITB0,edit,1350770816
Control5=IDC_LAYERSHADE_TEXT1,static,1342308352
Control6=IDC_LAYERSHADE_EDITR1,edit,1350639744
Control7=IDC_LAYERSHADE_EDITG1,edit,1350639744
Control8=IDC_LAYERSHADE_EDITB1,edit,1350639744

[DLG:IDD_LAYERTILE_GUI]
Type=1
Class=CLayerTileGUI
ControlCount=4
Control1=IDD_LAYERTILE_LIST,combobox,1342242819
Control2=IDD_LAYERTILE_BTN_UPDATE,button,1342242816
Control3=IDD_LAYERTILE_BTN_LOAD,button,1342242816
Control4=IDD_LAYERTILE_BTN_DELETE,button,1342242816

[DLG:IDD_LAYERTILE_TOOLBAR]
Type=1
Class=CLayerTileToolbar
ControlCount=2
Control1=IDD_LAYERTILE_BTN_PAINT,button,1342177344
Control2=IDD_LAYERTILE_BTN_SELECT,button,1342177344

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_MAPSIZE]
Type=1
Class=CMapSizeDlg
ControlCount=7
Control1=IDOK,button,1342177281
Control2=IDCANCEL,button,1342177280
Control3=IDC_MAPSIZE_WIDTH_TEXT,static,1342308866
Control4=IDC_MAPSIZE_WIDTH,edit,1350639744
Control5=IDC_MAPSIZE_HEIGHT_TEXT,static,1342308866
Control6=IDC_MAPSIZE_HEIGHT,edit,1350639744
Control7=IDC_MAPSIZE_WARNING,static,1342308352

[DLG:IDD_MULTIBAR]
Type=1
Class=CMultiBar

[DLG:IDD_NEWMAP]
Type=1
Class=CNewMapGUI
ControlCount=6
Control1=IDOK,button,1342177281
Control2=IDCANCEL,button,1342177280
Control3=IDC_MAPSIZE_WIDTH_TEXT,static,1342308866
Control4=IDC_MAPSIZE_WIDTH,edit,1350639744
Control5=IDC_MAPSIZE_HEIGHT_TEXT,static,1342308866
Control6=IDC_MAPSIZE_HEIGHT,edit,1350639744

[TB:IDR_TOOLBAR (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
CommandCount=6

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_APP_ABOUT
CommandCount=7

[MNU:IDR_MAPEDITYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_EXPORT
Command7=ID_FILE_MRU_FILE1
Command8=ID_APP_EXIT
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_VIEW_TOOLBAR
Command14=ID_VIEW_STATUS_BAR
Command15=ID_MAP_SETSIZE
Command16=ID_TOGGLE_GRID
Command17=ID_ZOOM_IN
Command18=ID_ZOOM_OUT
Command19=ID_MIRRORX
Command20=ID_MIRRORY
Command21=ID_ACTIVEBRUSH_LEFT
Command22=ID_ACTIVEBRUSH_RIGHT
Command23=ID_TOGGLE_TILEVIEW
Command24=ID_2D_3D_TOGGLE
Command25=ID_WINDOW_NEW
Command26=ID_WINDOW_CASCADE
Command27=ID_WINDOW_TILE_HORZ
Command28=ID_WINDOW_ARRANGE
Command29=ID_APP_ABOUT
CommandCount=29

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_ZOOM_IN
Command2=ID_ZOOM_OUT
Command3=ID_ACTIVEBRUSH_LEFT
Command4=ID_ACTIVEBRUSH_RIGHT
Command5=ID_2D_3D_TOGGLE
Command6=ID_EDIT_COPY
Command7=ID_TOGGLE_GRID
Command8=ID_FILE_NEW
Command9=ID_FILE_OPEN
Command10=ID_FILE_SAVE
Command11=ID_EDIT_PASTE
Command12=ID_ZOOM_IN
Command13=ID_EDIT_UNDO
Command14=ID_EDIT_CUT
Command15=ID_EDIT_COPY
Command16=ID_EDIT_PASTE
Command17=ID_TOGGLE_TILEVIEW
Command18=ID_ZOOM_OUT
Command19=ID_NEXT_PANE
Command20=ID_PREV_PANE
Command21=ID_MIRRORX
Command22=ID_EDIT_CUT
Command23=ID_MIRRORY
Command24=ID_EDIT_UNDO
CommandCount=24

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDOK,button,1342373889

[DLG:IDD_MULTIBAR (English (U.S.))]
Type=1
Class=?
ControlCount=0

