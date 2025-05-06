#pragma once
#include "afxdialogex.h"
#include "ToolMain.h"

// CObjectSpawnOptions dialog

class CObjectSpawnOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectSpawnOptions)

public:
	CObjectSpawnOptions(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CObjectSpawnOptions();

	// Overrides and Button logic
	virtual void OnCancel() override;
	afx_msg void OnBnClickedOk();

	// Update spawn properties
	void UpdateProperties(ToolMain* toolMain);

	// Stores Tool Main to call function within game to instantiate object
	ToolMain* m_ToolMain;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATION_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString XCoord;
	CString YCoord;
	CString ZCoord;
	CString XRotation;
	CString YRotation;
	CString ZRotation;
	CString XScale;
	CString YScale;
	CString ZScale;
};
