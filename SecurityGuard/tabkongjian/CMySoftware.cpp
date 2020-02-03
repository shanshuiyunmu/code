// CMySoftware.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMySoftware.h"
#include "afxdialogex.h"


// CMySoftware 对话框

IMPLEMENT_DYNAMIC(CMySoftware, CDialogEx)

CMySoftware::CMySoftware(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Software, pParent)
{

}

CMySoftware::~CMySoftware()
{
}

void CMySoftware::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_softwareUnload, m_softwareUninstall);
	DDX_Control(pDX, IDC_Boot, m_bootlist);
}

//遍历可卸载软件
void CMySoftware::LookSoftWare()
{
	LONG lReturn = RegOpenKeyEx(RootKey, lpSubKey, 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &hkResult);
	DWORD dwIndex = 0;
	DWORD i = 0;
	while (lReturn==0)
	{
 		DWORD dwType = 0;	
		DWORD dwKeyLen = 255;
		WCHAR szNewKeyName[MAX_PATH] = {};
		lReturn = RegEnumKeyEx(hkResult, dwIndex, szNewKeyName, &dwKeyLen, 0, NULL, NULL, NULL);
		WCHAR strMidReg[MAX_PATH] = {};
		swprintf_s(strMidReg, L"%s%s%s", lpSubKey, L"\\", szNewKeyName);
		HKEY hkValueKey = 0;
		RegOpenKeyEx(RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);
		CString name(m_SoftInfo.strSoftUniPath);
		DWORD dwNameLen = 255;
		RegQueryValueEx(hkValueKey, L"DisplayName", 0, &dwType, (LPBYTE)m_SoftInfo.szSoftName,&dwNameLen);
		dwNameLen = 255;
		
		RegQueryValueEx(hkValueKey, L"UninstallString", 0, &dwType, (LPBYTE)m_SoftInfo.strSoftUniPath, &dwNameLen);
		dwNameLen = 255;
		
		if (wcscmp(name.GetBuffer(), m_SoftInfo.strSoftUniPath))
		{	
			ShowSoftware(i);
			i++;
		}		
		dwIndex++;
	}
}

//显示软件信息到列表
void CMySoftware::ShowSoftware(DWORD index)
{
	CString temp;
	temp=m_SoftInfo.szSoftName;
	m_softwareUninstall.InsertItem(index, temp);
	temp=m_SoftInfo.strSoftUniPath;
	m_softwareUninstall.SetItemText(index, 1, temp);	
	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CMySoftware, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_softwareUnload, &CMySoftware::OnNMRClicksoftwareunload)
	ON_COMMAND(ID_unload, &CMySoftware::Onunload)
	ON_COMMAND(ID_flushsoft, &CMySoftware::Onflushsoft)
	ON_COMMAND(ID_deleteboot, &CMySoftware::Ondeleteboot)
	ON_COMMAND(ID_flushboot, &CMySoftware::Onflushboot)
	ON_NOTIFY(NM_RCLICK, IDC_Boot, &CMySoftware::OnNMRClickBoot)
END_MESSAGE_MAP()


// CMySoftware 消息处理程序

//遍历启动项
void CMySoftware::LookBoots()
{
	DWORD dwIndex = 0;
	for (int i = 0; i < 1; i++)
	{
		LONG lReturn = RegOpenKeyEx(HKEY_LOCAL_MACHINE, RegBootStartItemInHKLM[i], 0, KEY_ALL_ACCESS, &hkResult);
		DWORD Index = 0;
		while (lReturn==0)
		{
			DWORD dwType = 0;
			DWORD dwKeyLen = 255;
			WCHAR szNewKeyName[MAX_PATH] = {};
			lReturn = RegEnumKeyEx(hkResult, Index, szNewKeyName, &dwKeyLen, 0, NULL, NULL, NULL);
			WCHAR strMidReg[MAX_PATH] = {};
			swprintf_s(strMidReg, L"%s%s%s", RegBootStartItemInHKLM[i], L"\\", szNewKeyName);
			HKEY hkValueKey = 0;
			RegOpenKeyEx(RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);
			DWORD dwIndex = 0;
			LONG llReturn = 0;
			while (llReturn == 0)
			{
				DWORD dwNameLen = 255;
				DWORD dwValueLen = 255;
				DWORD dwValueNameLen = 255;
				CString name(szKeyValueName);
				hKey = hkValueKey;
				llReturn=RegEnumValue(hkValueKey, dwIndex, szKeyValueName, &dwValueNameLen, 0, NULL, (PBYTE)szValue, &dwValueLen);
				if (wcscmp(name.GetBuffer(), szKeyValueName))
				{	
					ShowBoot();
				}
				dwIndex++;
			}
			Index++;
			}
	}
}

//显示启动项
void CMySoftware::ShowBoot()
{
	CString temp;
	temp = szKeyValueName;
	m_bootlist.InsertItem(index, temp);
	temp = szValue;
	m_bootlist.SetItemText(index, 1, temp);
	UpdateData(FALSE);
	index++;
}

BOOL CMySoftware::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_softwareUninstall.MyInsertColumn(2, L"软件名称", 0, 200, L"卸载路径", 0, 400);
	LookSoftWare();

	m_bootlist.MyInsertColumn(2, L"启动项", 0, 200, L"键值", 0, 400);
	LookBoots();
	return TRUE;  
}

//响应 右键软件卸载列表 消息
void CMySoftware::OnNMRClicksoftwareunload(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int hang = pNMItemActivate->iItem;
	CString  Text = m_softwareUninstall.GetItemText(hang, 1);
	if (Text.IsEmpty())
	{
		return;
	}
	m_strSoftpath=Text;

	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu* submenu = menu.GetSubMenu(3);
	submenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}

//卸载软件
void CMySoftware::Onunload()
{
	HINSTANCE num=ShellExecute(NULL, L"open", m_strSoftpath, NULL, NULL, SW_SHOW);
	if (num == (HINSTANCE)2)
	{
		WCHAR* name = m_strSoftpath.GetBuffer();
		CHAR he[MAX_PATH];
		WCHAR_TO_CHAR(name, he)
		system(he);
	}
}

//刷新卸载列表
void CMySoftware::Onflushsoft()
{
	m_softwareUninstall.DeleteAllItems();
	LookSoftWare();
}

//删除启动项
void CMySoftware::Ondeleteboot()
{
	CString RegBootStart("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run");
	HKEY hSubKey;
	RegOpenKeyEx(RootKey, RegBootStart, 0, KEY_SET_VALUE, &hSubKey);
	RegDeleteValue(hSubKey, szValue);
}

//刷新启动项
void CMySoftware::Onflushboot()
{
	index = 0;
	m_bootlist.DeleteAllItems();
	LookBoots();
	UpdateData(FALSE);
}

//响应 右键启动项列表 消息
void CMySoftware::OnNMRClickBoot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int hang = pNMItemActivate->iItem;
	CString  Text = m_bootlist.GetItemText(hang, 1);
	if (Text.IsEmpty())
	{
		return;
	}
	lpBootSubKey= m_bootlist.GetItemText(hang, 0);
	wcscpy_s(szValue, 100, Text.GetBuffer());
	
	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu* submenu = menu.GetSubMenu(5);
	submenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}
