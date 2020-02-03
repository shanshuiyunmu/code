#pragma once
#include"PE.h"

// CMydelay 对话框

class CMydelay : public CDialogEx
{
	DECLARE_DYNAMIC(CMydelay)

public:
	CMydelay(WCHAR* fileName, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMydelay();
	WCHAR* m_fileName;
	PE pe;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_delayList;
	CListCtrl m_secondList;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
