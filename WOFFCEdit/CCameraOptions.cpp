// CCameraOptions.cpp : implementation file
//

#include "afxdialogex.h"
#include "CCameraOptions.h"
#include "resource.h"


// CCameraOptions dialog

IMPLEMENT_DYNAMIC(CCameraOptions, CDialogEx)

CCameraOptions::CCameraOptions(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CAMERA_OPTIONS, pParent)
	, XCoord(_T(""))
	, YCoord(_T(""))
	, ZCoord(_T(""))
	, YRotation(_T(""))
	, SlowSpeed(_T(""))
	, NormalSpeed(_T(""))
	, SprintSpeed(_T(""))
	, QERotation(_T(""))
	, MouseSensitivity(_T(""))
{

}

CCameraOptions::~CCameraOptions()
{
}

void CCameraOptions::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_Camera->m_camPosition.x = static_cast<float>(_tstof((LPCTSTR)XCoord));
	m_Camera->m_camPosition.y = static_cast<float>(_tstof((LPCTSTR)YCoord));
	m_Camera->m_camPosition.z = static_cast<float>(_tstof((LPCTSTR)ZCoord));

	m_Camera->m_camOrientation.y = static_cast<float>(_tstof((LPCTSTR)YRotation));
	m_Camera->m_camOrientation.z = static_cast<float>(_tstof((LPCTSTR)ZRotation));

	m_Camera->m_slowspeed = static_cast<float>(_tstof((LPCTSTR)SlowSpeed));
	m_Camera->m_movespeed = static_cast<float>(_tstof((LPCTSTR)NormalSpeed));
	m_Camera->m_sprintspeed = static_cast<float>(_tstof((LPCTSTR)SprintSpeed));

	m_Camera->m_camRotRate = static_cast<float>(_tstof((LPCTSTR)QERotation));
	m_Camera->m_camRotSensitivity = static_cast<float>(_tstof((LPCTSTR)MouseSensitivity));

	UpdateData(FALSE);
}

void CCameraOptions::OnCancel()
{
	DestroyWindow();
}

void CCameraOptions::UpdateProperties(Camera* CameraRef)
{
	UpdateData(TRUE);

	m_Camera = CameraRef;

	XCoord.Format(_T("%f"), m_Camera->m_camPosition.x);
	YCoord.Format(_T("%f"), m_Camera->m_camPosition.y);
	ZCoord.Format(_T("%f"), m_Camera->m_camPosition.z);

	YRotation.Format(_T("%f"), m_Camera->m_camOrientation.y);
	ZRotation.Format(_T("%f"), m_Camera->m_camOrientation.z);

	SlowSpeed.Format(_T("%f"), m_Camera->m_slowspeed);
	NormalSpeed.Format(_T("%f"), m_Camera->m_movespeed);
	SprintSpeed.Format(_T("%f"), m_Camera->m_sprintspeed);

	QERotation.Format(_T("%f"), m_Camera->m_camRotRate);
	MouseSensitivity.Format(_T("%f"), m_Camera->m_camRotSensitivity);

	UpdateData(FALSE);
}

void CCameraOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X_CAMERA_COORD, XCoord);
	DDX_Text(pDX, IDC_EDIT_Y_CAMERA_COORD, YCoord);
	DDX_Text(pDX, IDC_EDIT_Z_CAMERA_COORD, ZCoord);
	DDX_Text(pDX, IDC_EDIT_Y_CAMERA_ROTATION, YRotation);
	DDX_Text(pDX, IDC_EDIT_Z_CAMERA_ROTATION, ZRotation);
	DDX_Text(pDX, IDC_EDIT_CAMERA_SLOW, SlowSpeed);
	DDX_Text(pDX, IDC_EDIT_CAMERA_NORMAL, NormalSpeed);
	DDX_Text(pDX, IDC_EDIT_CAMERA_SPRINT, SprintSpeed);
	DDX_Text(pDX, IDC_EDIT_QE_ROTATION, QERotation);
	DDX_Text(pDX, IDC_EDIT_MOUSE, MouseSensitivity);
}


BEGIN_MESSAGE_MAP(CCameraOptions, CDialogEx)
ON_BN_CLICKED(IDOK, &CCameraOptions::OnBnClickedOk)
END_MESSAGE_MAP()