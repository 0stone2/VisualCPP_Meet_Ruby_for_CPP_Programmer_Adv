
// FileRenamer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CFileRenamerApp:
// �� Ŭ������ ������ ���ؼ��� FileRenamer.cpp�� �����Ͻʽÿ�.
//

class CFileRenamerApp : public CWinApp
{
public:
	CFileRenamerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CFileRenamerApp theApp;