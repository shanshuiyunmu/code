#pragma once
#include"PE.h"

// CImport 对话框

class CImport : public CDialogEx
{
	DECLARE_DYNAMIC(CImport)

public:
	CImport(WCHAR* fileName,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_dllList;
	CListCtrl m_funcLIst;
	WCHAR* m_fileName;
	PE pe;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
