#pragma once
#include"CMyList.h"
#include<TlHelp32.h>
// CMyThread 对话框

class CMyThread : public CDialogEx
{
	DECLARE_DYNAMIC(CMyThread)

public:
	CMyThread(UINT pid,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyThread();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREAD};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	//进程id
	UINT m_pid;

	//线程id
	UINT m_tid;

	//遍历线程
	void LookThread();

	//响应菜单消息,结束线程,挂起线程,恢复线程,刷新
	void OnThreadMenu(UINT id);

	//将线程信息显示到列表控件
	void ShowThread(int i, THREADENTRY32 thread);

	DECLARE_MESSAGE_MAP()
public:

	//线程列表控件
	CMyList m_threadList;


	virtual BOOL OnInitDialog();

	//响应右键列表框消息
	afx_msg void OnNMRClickproclist(NMHDR *pNMHDR, LRESULT *pResult);
};
