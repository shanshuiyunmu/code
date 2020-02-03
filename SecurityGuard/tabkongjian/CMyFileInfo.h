#pragma once
#include"CMyList.h"

// CMyFileInfo 对话框

class CMyFileInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CMyFileInfo)

public:
	CMyFileInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyFileInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_fileInfo };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//上一层文件路径
	CString m_lastMulu;

	DECLARE_MESSAGE_MAP()
public:

	//文件信息列表控件
	CMyList m_fileNameList;

	//响应双击列表控件消息
	afx_msg void OnNMDblclkfilename(NMHDR *pNMHDR, LRESULT *pResult);

	//显示信息到列表控件
	VOID PrintList(CString & FileName);

	virtual BOOL OnInitDialog();

	//响应返回一层按钮消息
	afx_msg void OnBnClickedreturn();

	//响应回到磁盘目录消息
	afx_msg void OnBnClickedgengmulu();

	//响应右键列表框消息
	afx_msg void OnNMRClickfilename(NMHDR *pNMHDR, LRESULT *pResult);

	//响应菜单 得到md5 消息
	afx_msg void OnGetmd5();

	//响应菜单 复制md5 消息
	afx_msg void OnCopymd5();
};
