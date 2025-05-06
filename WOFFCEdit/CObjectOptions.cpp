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
	, YCoord(_T(""))
	, ZCoord(_T(""))
	, XRotation(_T(""))
	, YRotation(_T(""))
	, ZRotation(_T(""))
	, XScale(_T(""))
	, YScale(_T(""))
	, ZScale(_T(""))
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

	m_SelectedObject->m_position.x = static_cast<float>(_tstof((LPCTSTR)XCoord));
	m_SelectedObject->m_position.y = static_cast<float>(_tstof((LPCTSTR)YCoord));
	m_SelectedObject->m_position.z = static_cast<float>(_tstof((LPCTSTR)ZCoord));

	m_SelectedObject->m_orientation.x = static_cast<float>(_tstof((LPCTSTR)XRotation));
	m_SelectedObject->m_orientation.y = static_cast<float>(_tstof((LPCTSTR)YRotation));
	m_SelectedObject->m_orientation.z = static_cast<float>(_tstof((LPCTSTR)ZRotation));

	m_SelectedObject->m_scale.x = static_cast<float>(_tstof((LPCTSTR)XScale));
	m_SelectedObject->m_scale.y = static_cast<float>(_tstof((LPCTSTR)YScale));
	m_SelectedObject->m_scale.z = static_cast<float>(_tstof((LPCTSTR)ZScale));

	UpdateData(FALSE);
}

void CObjectOptions::OnBnClickedDelete()
{
	if (m_SelectedObject)
	{
		m_ToolMain->m_d3dRenderer.DeleteSelectedDisplayObject();
	}

	DestroyWindow();
}

void CObjectOptions::OnBnClickedDuplicate()
{
	if (m_SelectedObject)
	{
		DisplayObject newObject = *m_SelectedObject;
		newObject.m_position.x += 0.5;
		newObject.m_position.z += 0.5;

		m_ToolMain->m_d3dRenderer.SpawnNewDisplayObject(newObject);
	}

	DestroyWindow();
}



void CObjectOptions::UpdateProperties(DisplayObject* SelectedObject, ToolMain* toolMain)
{
	if (!SelectedObject)
	{
		return;
	}

	m_SelectedObject = SelectedObject;
	m_ToolMain = toolMain;

	UpdateData(TRUE);

	XCoord.Format(_T("%f"), SelectedObject->m_position.x);
	YCoord.Format(_T("%f"), SelectedObject->m_position.y);
	ZCoord.Format(_T("%f"), SelectedObject->m_position.z);

	XRotation.Format(_T("%f"), SelectedObject->m_orientation.x);
	YRotation.Format(_T("%f"), SelectedObject->m_orientation.y);
	ZRotation.Format(_T("%f"), SelectedObject->m_orientation.z);

	XScale.Format(_T("%f"), SelectedObject->m_scale.x);
	YScale.Format(_T("%f"), SelectedObject->m_scale.y);
	ZScale.Format(_T("%f"), SelectedObject->m_scale.z);

	UpdateData(FALSE);
}

void CObjectOptions::OnCancel()
{
	End();
}

void CObjectOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X_COORD, XCoord);
	DDX_Text(pDX, IDC_EDIT_Y_COORD, YCoord);
	DDX_Text(pDX, IDC_EDIT_Z_COORD, ZCoord);
	DDX_Text(pDX, IDC_EDIT_X_ROTATION, XRotation);
	DDX_Text(pDX, IDC_EDIT_Y_ROTATION, YRotation);
	DDX_Text(pDX, IDC_EDIT_Z_ROTATION, ZRotation);
	DDX_Text(pDX, IDC_EDIT_X_SCALE, XScale);
	DDX_Text(pDX, IDC_EDIT_Y_SCALE, YScale);
	DDX_Text(pDX, IDC_EDIT_Z_SCALE, ZScale);
}


BEGIN_MESSAGE_MAP(CObjectOptions, CDialog)
	ON_BN_CLICKED(IDOK, &CObjectOptions::OnBnClickedOk)
	ON_BN_CLICKED(IDDELETE, &CObjectOptions::OnBnClickedDelete)
	ON_BN_CLICKED(IDDUPLICATE, &CObjectOptions::OnBnClickedDuplicate)
END_MESSAGE_MAP()


// CObjectOptions message handlers
