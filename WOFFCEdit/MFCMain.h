#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "CObjectOptions.h"
#include "CCameraOptions.h"
#include "CObjectSpawnOptions.h"


class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here

	// Object properties window
	CObjectOptions m_ObjectProperties;

	// Camera properties window
	CCameraOptions m_CameraProperties;

	// Spawn properties window
	CObjectSpawnOptions m_SpawnProperties;

	int m_width;		
	int m_height;
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg	void ToolBarButton1();

	// Display object properties window
	afx_msg void MenuEditObjectProperties();

	// Display camera properties window
	afx_msg void MenuEditCameraProperties();

	// Display object spawn properties windo
	afx_msg void MenuEditSpawnProperties();


	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
