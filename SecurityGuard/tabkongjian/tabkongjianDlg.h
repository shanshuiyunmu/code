
// tabkongjianDlg.h: 头文件
//

#pragma once
#include"CMytab.h"

// CtabkongjianDlg 对话框
class CtabkongjianDlg : public CDialogEx
{
// 构造
public:
	CtabkongjianDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABKONGJIAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:

	//应用图标
	HICON m_hIcon;

	//响应菜单消息,关机,休眠,锁屏等
	VOID MageComputer(UINT id);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//获得关机权限
	void MyUpdatePrivileges();

	//保存窗口状态
	BOOL m_IsWindowsHide;
	DECLARE_MESSAGE_MAP()
public:
	//tab控件
	CMytab m_mytab;

	//消息预处理,用于响应老板键
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
