#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "DisplayObject.h"
#include "ToolMain.h"


// CObjectOptions dialog

class CObjectOptions : public CDialog
{
	DECLARE_DYNAMIC(CObjectOptions)

public:
	CObjectOptions(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CObjectOptions();

	void End();

	// Overrides and Button logic
	virtual void OnCancel() override;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedDuplicate();

	// Update properties window
	void UpdateProperties(DisplayObject* SelectedObject, ToolMain* toolMain);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJECT_OPTIONS
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Stores selected ID to determine next object to update textboxes
	int m_SelectedID = -1;

	// Stores object to display info in textboxes, stores tools main to inform Game
	DisplayObject* m_SelectedObject = nullptr;
	ToolMain* m_ToolMain = nullptr;

	// Translation
	CString XCoord;
	CString YCoord;
	CString ZCoord;

	// Rotation
	CString XRotation;
	CString YRotation;
	CString ZRotation;

	// Scale
	CString XScale;
	CString YScale;
	CString ZScale;
};
