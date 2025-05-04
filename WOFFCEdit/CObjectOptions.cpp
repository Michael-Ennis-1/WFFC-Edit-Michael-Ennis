// CObjectOptions.cpp : implementation file
//

#include "afxdialogex.h"
#include "pch.h"
#include "CObjectOptions.h"


// CObjectOptions dialog

IMPLEMENT_DYNAMIC(CObjectOptions, CDialog)

CObjectOptions::CObjectOptions(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_OBJECT_OPTIONS, pParent)
	, XCoord(_T(""))
{

}

CObjectOptions::~CObjectOptions()
{
}

void CObjectOptions::End()
{
	DestroyWindow();
}

void CObjectOptions::OnBnClickedOk()
{
	UpdateData(TRUE);
	UpdateData(FALSE);

	// Temporarily just close the window
	//End();
}

void CObjectOptions::OnCancel()
{
	End();
}

void CObjectOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X_COORD, XCoord);
}


BEGIN_MESSAGE_MAP(CObjectOptions, CDialog)
ON_BN_CLICKED(IDOK, &CObjectOptions::OnBnClickedOk)
END_MESSAGE_MAP()


// CObjectOptions message handlers
