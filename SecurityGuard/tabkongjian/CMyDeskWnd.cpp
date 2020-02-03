// CMyDeskWnd.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyDeskWnd.h"
#include "afxdialogex.h"


// CMyDeskWnd 对话框

IMPLEMENT_DYNAMIC(CMyDeskWnd, CDialogEx)

CMyDeskWnd::CMyDeskWnd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DeskWnd, pParent)
{

}

CMyDeskWnd::~CMyDeskWnd()
{
}

void CMyDeskWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_wndlist, m_wndList);
}

//枚举子窗口
BOOL CALLBACK WndProc(HWND hWnd, LPARAM lParam)
{
	//强转到列表控件
	CListCtrl* m_list_Window = (CListCtrl*)lParam;
	
	//获得窗口标题信息
	WCHAR buff[200];
	GetWindowText(hWnd, buff, _countof(buff));

	//窗口标题不为空,并且窗口没有隐藏
	if (IsWindowVisible(hWnd) && wcslen(buff))
	{

		//添加列表行信息
		DWORD nIndex = m_list_Window->GetItemCount();
		m_list_Window->InsertItem(nIndex, buff);
	}
	return TRUE;
}

//显示窗口信息
VOID CMyDeskWnd::PrintWindowsList()
{
	//清空窗口控件内容
	m_wndList.DeleteAllItems();

	//枚举子窗口
	EnumWindows(WndProc, (LPARAM)&m_wndList);
}


BEGIN_MESSAGE_MAP(CMyDeskWnd, CDialogEx)
END_MESSAGE_MAP()


// CMyDeskWnd 消息处理程序

BOOL CMyDeskWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//设置窗口列表控件风格
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//插入列表控件列信息
	m_wndList.InsertColumn(0, L"桌面窗口", 0, 400);

	//显示窗口信息
	PrintWindowsList();

	return TRUE; 
}
