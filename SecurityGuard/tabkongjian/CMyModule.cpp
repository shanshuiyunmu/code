// CMyModule.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyModule.h"
#include "afxdialogex.h"


// CMyModule 对话框

IMPLEMENT_DYNAMIC(CMyModule, CDialogEx)

CMyModule::CMyModule(UINT pid, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODULE, pParent),
	m_pid(pid)
{

}

CMyModule::~CMyModule()
{
}

void CMyModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_proclist, m_moduleList);
}

//遍历模块
void CMyModule::LookModule()
{
	//创建快照
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_pid);

	//判断创建快照是否成功
	if (Snapshot == INVALID_HANDLE_VALUE)
	{
		MessageBox( L"模块快照创建失败");
		return;
	}

	//创建模块信息结构体
	MODULEENTRY32 ModuleInfo = { sizeof(MODULEENTRY32) };
	int i = 0;

	//第一次遍历模块快照,如果成功,进行下一次遍历
	if (Module32First(Snapshot, &ModuleInfo))
	{
		do {

			//将模块信息显示到列表控件中
			ShowModule(i, ModuleInfo);
			i++;
		} while (Module32Next(Snapshot, &ModuleInfo));
	}

	//管理快照句柄
	CloseHandle(Snapshot);
}

//显示模块信息到列表控件
void CMyModule::ShowModule(int i, MODULEENTRY32 mod)
{
	//显示模块路径
	m_moduleList.InsertItem(i, mod.szExePath);

	//显示模块基地址
	CString temp;
	temp.Format(L"0x%08X", mod.modBaseAddr);
	m_moduleList.SetItemText(i, 1, temp);

	//显示模块大小
	temp.Format(L"0x%08X", mod.modBaseSize);
	m_moduleList.SetItemText(i, 2, temp);

}


BEGIN_MESSAGE_MAP(CMyModule, CDialogEx)
END_MESSAGE_MAP()


// CMyModule 消息处理程序


BOOL CMyModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//设置列表控件列信息
	m_moduleList.MyInsertColumn(3, L"模块路径", 0, 500, L"模块基址", 2, 150, L"模块大小", 2, 150);

	//遍历模块
	LookModule();

	return TRUE;  
}
