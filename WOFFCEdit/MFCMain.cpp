#include "MFCMain.h"
#include "resource.h"
#include <Effects.h>


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT,	&MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_OBJECTPROPERTIES, &MFCMain::MenuEditObjectProperties)
	ON_COMMAND(ID_EDIT_CAMERAPROPERTIES, &MFCMain::MenuEditCameraProperties)
	ON_COMMAND(ID_EDIT_SPAWNPROPERTIES, &MFCMain::MenuEditSpawnProperties)
	ON_COMMAND(ID_BUTTON40001,	&MFCMain::ToolBarButton1)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{	
			int ID = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString = L"Selected Object: " + std::to_wstring(ID);
			m_ToolSystem.Tick(&msg);

			// Update MFC object properties window if ID changes to another object; otherwise, keep it selected on an object regardless
			if (m_ObjectProperties)
			{
				if (ID != m_ObjectProperties.m_SelectedID && ID > -1)
				{
					Game& d3dRenderer = m_ToolSystem.m_d3dRenderer;
					DisplayObject* selectedObject = &d3dRenderer.m_displayList[ID];

					m_ObjectProperties.m_SelectedID = ID;
					m_ObjectProperties.UpdateProperties(selectedObject, &m_ToolSystem);
				}
			}
			else
			{
				// If object properties window closed, stop highlighting object
				Game& d3dRenderer = m_ToolSystem.m_d3dRenderer;
				if (d3dRenderer.m_ObjectPropertiesOpen)
				{
					d3dRenderer.m_ObjectPropertiesOpen = false;
				}
			}

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
		}
	}

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
	m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObject, &m_ToolSystem);
}

void MFCMain::ToolBarButton1()
{
	m_ToolSystem.onActionSave();
}

void MFCMain::MenuEditObjectProperties()
{
	m_ObjectProperties.Create(IDD_DIALOG_OBJECT_OPTIONS);
	m_ObjectProperties.ShowWindow(SW_SHOW);

	Game& d3dRenderer = m_ToolSystem.m_d3dRenderer;

	// View selected object data in property window
	int SelectedObjectID = m_ToolSystem.getCurrentSelectionID();
	if (SelectedObjectID > -1)
	{
		DisplayObject* selectedObject = &d3dRenderer.m_displayList[SelectedObjectID];
		m_ObjectProperties.UpdateProperties(selectedObject, &m_ToolSystem);
	}

	// Forces selection to remain when property window open
	d3dRenderer.m_ObjectPropertiesOpen = true;
}

void MFCMain::MenuEditCameraProperties()
{
	m_CameraProperties.Create(IDD_DIALOG_CAMERA_OPTIONS);
	m_CameraProperties.ShowWindow(SW_SHOW);

	Game& d3dRenderer = m_ToolSystem.m_d3dRenderer;
	m_CameraProperties.UpdateProperties(d3dRenderer.m_Camera.get());
}

void MFCMain::MenuEditSpawnProperties()
{
	m_SpawnProperties.Create(IDD_DIALOG_CREATION_OPTIONS);
	m_SpawnProperties.ShowWindow(SW_SHOW);

	m_SpawnProperties.UpdateProperties(&m_ToolSystem);
}

MFCMain::MFCMain()
{
}

MFCMain::~MFCMain()
{
}
