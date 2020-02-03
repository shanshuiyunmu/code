#pragma once
#include "PE.h"

// CMyRes 对话框

class CMyRes : public CDialogEx
{
	DECLARE_DYNAMIC(CMyRes)

public:
	CMyRes(WCHAR* fileName,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyRes();
	WCHAR* m_fileName;
	PE pe;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_firstList;
	CListCtrl m_secondList;
	CListCtrl m_thirdList;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
};
