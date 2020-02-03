#pragma once
#include"PE.h"

// CMyReloc 对话框

class CMyReloc : public CDialogEx
{
	DECLARE_DYNAMIC(CMyReloc)

public:
	CMyReloc(WCHAR* fileName,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyReloc();
	WCHAR* m_fileName;
	PE pe;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_firstList;
	CListCtrl m_secondList;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
