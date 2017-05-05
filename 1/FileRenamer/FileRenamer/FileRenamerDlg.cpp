
// FileRenamerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "FileRenamer.h"
#include "FileRenamerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "Ruby.h"

BOOL UTF8toANSI(const char *szUTF8, char *szANSI)
{
	BOOL	bSuccess{ FALSE };
	wchar_t	wszString[MAX_PATH]{ 0, };
	int		nLen{ 0 };

	__try {
		// UTF8 -> UTF16
		nLen = MultiByteToWideChar(CP_UTF8, 0, szUTF8, strlen(szUTF8), NULL, NULL);
		MultiByteToWideChar(CP_UTF8, 0, szUTF8, strlen(szUTF8), wszString, nLen);

		// UTF16 -> ANSI
		nLen = WideCharToMultiByte(CP_ACP, 0, wszString, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, wszString, -1, szANSI, nLen, NULL, NULL);

		bSuccess = TRUE;
	}
	__finally {
		int nErr = GetLastError();
	}

	return bSuccess;
}


BOOL ANSItoUTF8(const char *szANSI, char *szUTF8)
{
	BOOL	bSuccess{ FALSE };
	wchar_t	wszString[MAX_PATH]{ 0, };
	int		nLen{ 0 };

	__try {
		// ANSI -> UTF16
		nLen = MultiByteToWideChar(CP_ACP, 0, szANSI, lstrlen(szANSI), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, szANSI, lstrlen(szANSI), wszString, nLen);

		// UTF16 -> UTF8
		nLen = WideCharToMultiByte(CP_UTF8, 0, wszString, -1, szUTF8, 0, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, wszString, -1, szUTF8, nLen, NULL, NULL);

		bSuccess = TRUE;
	}
	__finally {
		int nErr = GetLastError();
	}

	return bSuccess;
}




// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileRenamerDlg 대화 상자



CFileRenamerDlg::CFileRenamerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILERENAMER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileRenamerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileRenamerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RENAME_BUTTON, &CFileRenamerDlg::OnBnClickedRenameButton)
	ON_BN_CLICKED(IDOK, &CFileRenamerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFileRenamerDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_PLUGIN_COMBO, &CFileRenamerDlg::OnCbnSelchangePluginCombo)
END_MESSAGE_MAP()


// CFileRenamerDlg 메시지 처리기

BOOL CFileRenamerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	::DragAcceptFiles(m_hWnd, true);

	((CListCtrl *)GetDlgItem(IDC_FROM_LIST)) -> InsertColumn(0, "번호", LVCFMT_LEFT, 40, -1);
	((CListCtrl *)GetDlgItem(IDC_FROM_LIST)) -> InsertColumn(1, "파일 이름", LVCFMT_LEFT, 200, -1);

	((CListCtrl *)GetDlgItem(IDC_TO_LIST))->InsertColumn(0, "번호", LVCFMT_LEFT, 40, -1);
	((CListCtrl *)GetDlgItem(IDC_TO_LIST))->InsertColumn(1, "파일 이름", LVCFMT_LEFT, 200, -1);

	LoadPlugInList();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFileRenamerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFileRenamerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFileRenamerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileRenamerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnDropFiles(hDropInfo);

	int nItem{ 0 };
	int nIndex{ 0 };
	int nFiles{ 0 };
	char  szSrcFile[MAX_PATH]{ 0, };
	char szFname[MAX_PATH]{ 0, };
	char	szExt[MAX_PATH]{ 0, };

	char szIndex[MAX_PATH]{ 0 };


	nItem = ((CListCtrl *)GetDlgItem(IDC_FROM_LIST))->GetItemCount();
	nFiles = DragQueryFile(hDropInfo, -1, 0, 0);

	for (nIndex = 0; nIndex < nFiles; nIndex++)
	{
		::DragQueryFile(hDropInfo, nIndex, szSrcFile, sizeof(szSrcFile));

		strcpy_s(m_FileList[nItem].szSrcFile, MAX_PATH, szSrcFile);

		_splitpath(szSrcFile, NULL, NULL, szFname, szExt);
		sprintf_s(szSrcFile, MAX_PATH, "%s%s", szFname, szExt);

		sprintf_s(szIndex, MAX_PATH, "%d", nItem + 1);
		((CListCtrl *)GetDlgItem(IDC_FROM_LIST))->InsertItem(nItem, szIndex, 0);
		((CListCtrl *)GetDlgItem(IDC_FROM_LIST))->SetItem(nItem, 1, LVIF_TEXT, szSrcFile, 0, 0, 0, 0);

		nItem++;
	}

	m_nNumberOfFiles = nItem;
}


VALUE DbgString(VALUE self, VALUE String)
{
	int nType = 0;
	char *szDbgString{ NULL };

	szDbgString = rb_string_value_cstr(&String);

	OutputDebugStringA(szDbgString);

	return Qnil;
}

void CFileRenamerDlg::OnBnClickedRenameButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	BOOL	bSuccess{ FALSE };
	VALUE   ScriptFile{ NULL };
	ID		Rename{ 0 };
	char	szScriptPath[MAX_PATH]{ 0, };

	VALUE	ReturnValue{ NULL };
	VALUE	RetString{ NULL };
	char	*szUTF8{ NULL };

	int				nIndex{ 0 };
	int				nItem{ 0 };
	char			szIndex[MAX_PATH]{ 0, };
	char			szFname[MAX_PATH]{ 0, };
	char			szExt[MAX_PATH]{ 0, };
	char			szDstFile[MAX_PATH]{ 0, };

	__try {
		int nPlugIn = ((CComboBox *)GetDlgItem(IDC_PLUGIN_COMBO))->GetCurSel();
		((CListCtrl *)GetDlgItem(IDC_TO_LIST))->DeleteAllItems();

		if (-1 == nPlugIn) __leave;

		bSuccess = InitRuby();
		if (FALSE == bSuccess) __leave;

		rb_define_global_function("DbgString", (VALUE(*)(ANYARGS))DbgString, 1);

		for (nIndex = 0; nIndex < m_nNumberOfFiles; nIndex++)
		{
			ANSItoUTF8(m_PlugIns[nPlugIn].szPath, szScriptPath);
			ScriptFile = rb_str_new_cstr(szScriptPath);

			rb_load(ScriptFile, 0);

			Rename = rb_intern("Rename");
			RetString = rb_funcall(0, Rename, 1, rb_str_new_cstr(m_FileList[nIndex].szSrcFile));

			szUTF8 = rb_string_value_ptr(&RetString);
			UTF8toANSI(szUTF8, m_FileList[nIndex].szDstFile);

			sprintf_s(szIndex, MAX_PATH, "%d", nItem + 1);
			((CListCtrl *)GetDlgItem(IDC_TO_LIST))->InsertItem(nItem, szIndex, 0);

			_splitpath(m_FileList[nIndex].szDstFile, NULL, NULL, szFname, szExt);
			sprintf_s(szDstFile, MAX_PATH, "%s%s", szFname, szExt);

			((CListCtrl *)GetDlgItem(IDC_TO_LIST))->SetItem(nItem, 1, LVIF_TEXT, szDstFile, 0, 0, 0, 0);

			nItem++;
		}
	}
	__finally {

	}
}


void CFileRenamerDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OutputDebugString("\n===========================================\n");
	for (int nIndex = 0; nIndex < m_nNumberOfFiles; nIndex++)
	{
		OutputDebugString(m_FileList[nIndex].szSrcFile);
		OutputDebugString("\n");
		OutputDebugString(m_FileList[nIndex].szDstFile);
		OutputDebugString("\n===========================================\n");
		rename(m_FileList[nIndex].szSrcFile, m_FileList[nIndex].szDstFile);
	}

	//CDialogEx::OnOK();
}


void CFileRenamerDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CFileRenamerDlg::OnCbnSelchangePluginCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


// PlugIn 목록을 얻어 온다...
void CFileRenamerDlg::LoadPlugInList()
{
	// 1. PlugIn 폴더에 있는 특정 확장자(js)를 가진 파일들을 목록을 구한다.
	CFileFind	PlugInFinder;
	BOOL		bFound{ FALSE };
	BOOL		bSuccess{ FALSE };
	PLUGIN		PlugIn{ 0, };
	UINT		nIndex{ 0 };

	bFound = PlugInFinder.FindFile("./PlugIn/*.rb");

	while (bFound)
	{
		bFound = PlugInFinder.FindNextFile();

		// 2. 얻어온 파일들에서 특정 함수 (GetInfo)를 호출한다. 
		// 이름, 설명, 버전, 날짜, 만든이
		ZeroMemory(&PlugIn, sizeof(PlugIn));
		bSuccess = GetPlugInInfo((LPCSTR)PlugInFinder.GetFilePath(), &PlugIn);
	
		if (TRUE == bSuccess)
		{
			// 이 함수가 리턴한 문자열을 콤보 박스에 추가한다.....
			((CComboBox *)GetDlgItem(IDC_PLUGIN_COMBO))->AddString(PlugIn.szName);

			// 3. PlugIn 리스트에 보관한다.
			memcpy(&(m_PlugIns[nIndex]), &PlugIn, sizeof(PLUGIN));

			nIndex++;
		}
		
		if (nIndex >= MAX_PLUGINS) break;
	}



}



BOOL CFileRenamerDlg::GetPlugInInfo(LPCSTR szPlugIn, PPLUGIN pPlugIn)
{
	BOOL bSuccess{ FALSE };

	VALUE   ScriptFile{ NULL };
	ID		GetInfo{ 0 };
	char	szScriptPath[ MAX_PATH ] {0, };

	VALUE	ReturnValue{ NULL };
	VALUE	RetString{ NULL };
	char	*szUTF8{ NULL };

	__try {
		bSuccess = InitRuby();
		if (FALSE == bSuccess) __leave;

		ANSItoUTF8(szPlugIn, szScriptPath);
		ScriptFile = rb_str_new_cstr(szScriptPath);

		char szDbgString[MAX_PATH]{ 0, };
		VALUE load_path = rb_gv_get("$LOAD_PATH");
		
		for (int nIndex = 0; nIndex < 8; nIndex++)
		{
			RetString = rb_ary_shift(load_path);
			szUTF8 = rb_string_value_ptr(&RetString);
			UTF8toANSI(szUTF8, szDbgString);
			OutputDebugStringA(szDbgString);
		}

		rb_ary_push(load_path, rb_str_new_cstr("C:\\Script\\Ruby\\lib"));
		rb_ary_push(load_path, rb_str_new_cstr("C:/Script/Ruby/lib/ruby/site_ruby"));
		rb_ary_push(load_path, rb_str_new_cstr("C:/Script/Ruby/lib/ruby/site_ruby/2.2.0/"));
		rb_ary_push(load_path, rb_str_new_cstr("C:/Script/Ruby/lib/ruby/site_ruby/2.2.0/i386-msvcrt"));
		rb_ary_push(load_path, rb_str_new_cstr("C:/Script/Ruby/lib/ruby/vendor_ruby"));
		rb_ary_push(load_path, rb_str_new_cstr("C:/Script/Ruby/lib/ruby/vendor_ruby/2.2.0"));
		rb_ary_push(load_path, rb_str_new_cstr("C:/Script/Ruby/lib/ruby/vendor_ruby/2.2.0/i386-msvcrt"));
		rb_ary_push(load_path, rb_str_new_cstr("C:/Script/Ruby/lib/ruby/2.2.0"));
		rb_ary_push(load_path, rb_str_new_cstr("C:/Script/Ruby/lib/ruby/2.2.0/i386-mingw32"));

		load_path = rb_gv_get("$LOAD_PATH");

		for (int nIndex = 0; nIndex < 9; nIndex++)
		{
			RetString = rb_ary_entry(load_path, nIndex);
			szUTF8 = rb_string_value_ptr(&RetString);
			UTF8toANSI(szUTF8, szDbgString);
			OutputDebugStringA(szDbgString);
			OutputDebugStringA("\n");
			//rb_ary_push(load_path, rb_str_new2("any expanded directory"));
		}

		rb_load(ScriptFile, 0);

		GetInfo = rb_intern("GetInfo");
		ReturnValue = rb_funcall(0, GetInfo, 0);

		RetString = rb_ary_entry(ReturnValue, 0);
		szUTF8 = rb_string_value_ptr(&RetString);
		UTF8toANSI(szUTF8, pPlugIn->szName);

		RetString = rb_ary_entry(ReturnValue, 1);
		szUTF8 = rb_string_value_ptr(&RetString);
		UTF8toANSI(szUTF8, pPlugIn->szDesc);

		RetString = rb_ary_entry(ReturnValue, 2);
		szUTF8 = rb_string_value_ptr(&RetString);
		UTF8toANSI(szUTF8, pPlugIn->szVersion);

		RetString = rb_ary_entry(ReturnValue, 3);
		szUTF8 = rb_string_value_ptr(&RetString);
		UTF8toANSI(szUTF8, pPlugIn->szDate);

		RetString = rb_ary_entry(ReturnValue, 4);
		szUTF8 = rb_string_value_ptr(&RetString);
		UTF8toANSI(szUTF8, pPlugIn->szAuthor);

		RetString = rb_ary_entry(ReturnValue, 5);
		szUTF8 = rb_string_value_ptr(&RetString);
		UTF8toANSI(szUTF8, pPlugIn->szEmail);

		strcpy_s(pPlugIn->szPath, MAX_PATH, szPlugIn);
	}
	__finally {

	}

	return bSuccess;
}