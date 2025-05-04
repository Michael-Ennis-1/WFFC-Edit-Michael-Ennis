#pragma once
#include "afxdialogex.h"
#include "resource.h"


// CObjectOptions dialog

class CObjectOptions : public CDialog
{
	DECLARE_DYNAMIC(CObjectOptions)

public:
	CObjectOptions(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CObjectOptions();

	void End();
	afx_msg void OnBnClickedOk();

	virtual void OnCancel();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJECT_OPTIONS
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString XCoord;
};
