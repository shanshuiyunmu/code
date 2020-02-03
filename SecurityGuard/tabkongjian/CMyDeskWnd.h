#pragma once


// CMyDeskWnd 对话框

class CMyDeskWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDeskWnd)

public:
	CMyDeskWnd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyDeskWnd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DeskWnd};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:

	//窗口列表控件
	CListCtrl m_wndList;

	//显示窗口信息
	VOID PrintWindowsList();

	virtual BOOL OnInitDialog();
};
