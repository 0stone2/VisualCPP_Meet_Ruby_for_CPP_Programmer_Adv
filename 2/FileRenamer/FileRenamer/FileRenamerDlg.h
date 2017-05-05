
// FileRenamerDlg.h : ��� ����
//

#pragma once


#define MAX_PLUGINS		10
typedef struct {
	char szPath[MAX_PATH];
	char szName[MAX_PATH];
	char szDesc[MAX_PATH];
	char szVersion[MAX_PATH];
	char szDate[MAX_PATH];
	char szAuthor[MAX_PATH];
	char szEmail[MAX_PATH];
} PLUGIN, *PPLUGIN;


#define MAX_FILES		256
typedef struct {
	char szSrcFile[MAX_PATH];
	char szDstFile[MAX_PATH];
} FILE_RENAMER, * PFILE_RENAMER;




// CFileRenamerDlg ��ȭ ����
class CFileRenamerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CFileRenamerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILERENAMER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedRenameButton();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	afx_msg void OnCbnSelchangePluginCombo();
	
private:
	UINT			m_nNumberOfPlugIns;
	PLUGIN			m_PlugIns[MAX_PLUGINS];

	UINT			m_nNumberOfFiles;
	FILE_RENAMER	m_FileList[MAX_FILES];

	void LoadPlugInList();
	BOOL GetPlugInInfo(LPCSTR szPlugIn, PPLUGIN pPlugIn);
public:
	void addFileToRightPane(LPCSTR pszFile);
};
