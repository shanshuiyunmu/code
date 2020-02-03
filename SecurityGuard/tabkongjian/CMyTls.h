#pragma once
#include"PE.h"

// CMyTls 对话框

class CMyTls : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTls)

public:
	CMyTls(WCHAR* fileName,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyTls();
	WCHAR* m_fileName;
	PE pe;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_start;
	CString m_end;
	CString m_index;
	CString m_callback;
};
