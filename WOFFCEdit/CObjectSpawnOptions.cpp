// CObjectSpawnOptions.cpp : implementation file
//

#include "afxdialogex.h"
#include "CObjectSpawnOptions.h"
#include "resource.h"


// CObjectSpawnOptions dialog

IMPLEMENT_DYNAMIC(CObjectSpawnOptions, CDialogEx)

CObjectSpawnOptions::CObjectSpawnOptions(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CREATION_OPTIONS, pParent)
	, XCoord(_T("0"))
	, YCoord(_T("0"))
	, ZCoord(_T("0"))
	, XRotation(_T("0"))
	, YRotation(_T("0"))
	, ZRotation(_T("0"))
	, XScale(_T("1"))
	, YScale(_T("1"))
	, ZScale(_T("1"))
{
}

CObjectSpawnOptions::~CObjectSpawnOptions()
{
}

void CObjectSpawnOptions::OnCancel()
{
	DestroyWindow();
}

void CObjectSpawnOptions::OnBnClickedOk()
{
	UpdateData(TRUE);

	DisplayObject newObject;

	newObject.m_position.x = static_cast<float>(_tstof((LPCTSTR)XCoord));
	newObject.m_position.y = static_cast<float>(_tstof((LPCTSTR)YCoord));
	newObject.m_position.z = static_cast<float>(_tstof((LPCTSTR)ZCoord));

	newObject.m_orientation.x = static_cast<float>(_tstof((LPCTSTR)XRotation));
	newObject.m_orientation.y = static_cast<float>(_tstof((LPCTSTR)YRotation));
	newObject.m_orientation.z = static_cast<float>(_tstof((LPCTSTR)ZRotation));

	newObject.m_scale.x = static_cast<float>(_tstof((LPCTSTR)XScale));
	newObject.m_scale.y = static_cast<float>(_tstof((LPCTSTR)YScale));
	newObject.m_scale.z = static_cast<float>(_tstof((LPCTSTR)ZScale));

	m_ToolMain->m_d3dRenderer.SpawnNewDisplayObject(newObject);

	UpdateData(FALSE);
}

void CObjectSpawnOptions::UpdateProperties(ToolMain* toolMain)
{
	UpdateData(TRUE);

	m_ToolMain = toolMain;

	UpdateData(FALSE);
}

void CObjectSpawnOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X_COORD_CREATION, XCoord);
	DDX_Text(pDX, IDC_EDIT_Y_COORD_CREATION, YCoord);
	DDX_Text(pDX, IDC_EDIT_Z_COORD_CREATION, ZCoord);
	DDX_Text(pDX, IDC_EDIT_X_ROTATION_CREATION, XRotation);
	DDX_Text(pDX, IDC_EDIT_Y_ROTATION_CREATION, YRotation);
	DDX_Text(pDX, IDC_EDIT_Z_ROTATION_CREATION, ZRotation);
	DDX_Text(pDX, IDC_EDIT_X_SCALE_CREATION, XScale);
	DDX_Text(pDX, IDC_EDIT_Y_SCALE_CREATION, YScale);
	DDX_Text(pDX, IDC_EDIT_Z_SCALE_CREATION, ZScale);
}


BEGIN_MESSAGE_MAP(CObjectSpawnOptions, CDialogEx)
ON_BN_CLICKED(IDOK, &CObjectSpawnOptions::OnBnClickedOk)
END_MESSAGE_MAP()


// CObjectSpawnOptions message handlers
