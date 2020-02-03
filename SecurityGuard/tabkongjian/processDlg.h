
// processDlg.h: 头文件
//

#pragma once
#include"CMyList.h"
#include <TlHelp32.h> 
// CprocessDlg 对话框
class CprocessDlg : public CDialogEx
{
// 构造
public:
	CprocessDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	//保存列表右键选中的进程id
	UINT m_pid;

	//获得debug的权限
	void MyUpdatePrivileges();
	
	//在列表控件上显示进程信息
	void ShowProcess(int i,PROCESSENTRY32 pro);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	//遍历进程
	void lookProcess();

	//响应进程子菜单 结束进程,打开线程,打开模块,刷新
	void OnProcessMenu(UINT id);

	DECLARE_MESSAGE_MAP()
public:

	//进程列表控件
	CMyList m_process;

	//响应右键列表框的消息
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);

	//提权,使进程以管理员权限打开
	afx_msg void OnBnClickedtiquan();

	//查询进程权限
	BOOL QueryPrivileges();
};
