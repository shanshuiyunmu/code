#pragma once
#include<vector>
#include<afxwin.h>
#include "CMyList.h"
using namespace std;
// CMySoftware 对话框

class CMySoftware : public CDialogEx
{
	DECLARE_DYNAMIC(CMySoftware)

public:
	CMySoftware(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMySoftware();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Software};
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//遍历可卸载软件
	void LookSoftWare();

	//显示软件信息到列表
	void ShowSoftware(DWORD index);
	
	//遍历启动项
	void LookBoots();

	//显示启动项
	void ShowBoot();

	//创建软件信息结构体
	typedef struct _SOFTINFO
	{
		WCHAR szSoftName[50];
		WCHAR szSoftVer[50];
		WCHAR szSoftDate[20];
		WCHAR szSfotSize[MAX_PATH];
		WCHAR strSoftInspath[MAX_PATH];
		WCHAR strSoftUniPath[MAX_PATH];
		WCHAR strSoftVenRel[50];
		WCHAR strSoftIco[MAX_PATH];
	}SOFTINFO, *PSOFTINFO;

	//主键
	HKEY RootKey = HKEY_LOCAL_MACHINE;

	//子键
	LPCTSTR lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	HKEY hkResult = 0;
	CHAR m_strSoftInspath[MAX_PATH];
	CString m_strSoftpath;
	SOFTINFO m_SoftInfo = {};
	vector<SOFTINFO> m_vectSoftInfo; 
	WCHAR RegBootStartItemInHKLM[1][MAX_PATH] = {L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"};
	DWORD index = 0;
	WCHAR szKeyValueName[MAX_PATH] = {};
	WCHAR szValue[100] = {};
	WCHAR m_strMidReg[MAX_PATH] = {};
	CString lpBootSubKey;
	HKEY hKey;
	DECLARE_MESSAGE_MAP()
public:

	//软件卸载列表
	CMyList m_softwareUninstall;

	//启动项列表
	CMyList m_bootlist;

	virtual BOOL OnInitDialog();

	//响应 右键软件卸载列表 消息
	afx_msg void OnNMRClicksoftwareunload(NMHDR *pNMHDR, LRESULT *pResult);

	//卸载软件
	afx_msg void Onunload();

	//刷新卸载列表
	afx_msg void Onflushsoft();

	//删除启动项
	afx_msg void Ondeleteboot();

	//刷新启动项
	afx_msg void Onflushboot();

	//响应 右键启动项列表 消息
	afx_msg void OnNMRClickBoot(NMHDR *pNMHDR, LRESULT *pResult);
};
