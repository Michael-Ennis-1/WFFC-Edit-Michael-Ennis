#pragma once
#include "afxdialogex.h"
#include "Camera.h"


// CCameraOptions dialog

class CCameraOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraOptions)

public:
	CCameraOptions(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCameraOptions();

	// Overrides and button logic
	afx_msg void OnBnClickedOk();
	virtual void OnCancel() override;

	// Update properties window
	void UpdateProperties(Camera* CameraRef);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMERA_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	Camera* m_Camera;

	DECLARE_MESSAGE_MAP()
public:
	// Translation
	CString XCoord;
	CString YCoord;
	CString ZCoord;

	// Rotation
	CString YRotation;
	CString ZRotation;

	// Move Speed
	CString SlowSpeed;
	CString NormalSpeed;
	CString SprintSpeed;

	// Sensitivity
	CString QERotation;
	CString MouseSensitivity;
};
