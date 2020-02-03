#pragma once
#include"CMyList.h"
#include<TlHelp32.h>
// CMyModule 对话框

class CMyModule : public CDialogEx
{
	DECLARE_DYNAMIC(CMyModule)

public:
	CMyModule(UINT pid,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyModule();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODULE};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//线程id
	UINT m_pid;

	//遍历模块
	void LookModule();

	//显示模块信息到列表控件
	void ShowModule(int i,MODULEENTRY32 mod);

	DECLARE_MESSAGE_MAP()
public:

	//模块列表控件
	CMyList m_moduleList;

	virtual BOOL OnInitDialog();
};
