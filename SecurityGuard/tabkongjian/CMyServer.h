#pragma once
#include"CMyList.h"
#include <winsvc.h>
// CMyServer 对话框

class CMyServer : public CDialogEx
{
	DECLARE_DYNAMIC(CMyServer)

public:
	CMyServer(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyServer();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Server};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//遍历服务
	void LookSerice();

	//保存服务名
	WCHAR m_sericename[50];

	//响应菜单 开启服务, 停止服务,刷新
	void OnSerice(UINT id);

	//显示服务信息到列表控件
	void ShowSerice(int i, ENUM_SERVICE_STATUS_PROCESS & serice);

	DECLARE_MESSAGE_MAP()
public:

	//服务列表控件
	CMyList m_sericelist;

	virtual BOOL OnInitDialog();

	//响应列表控件右键消息
	afx_msg void OnNMRClickSericelist(NMHDR *pNMHDR, LRESULT *pResult);
};
