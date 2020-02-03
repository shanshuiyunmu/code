
// tabkongjianDlg.cpp: 实现文件
//


#include "stdafx.h"
#include "resource.h"
#include "tabkongjian.h"
#include "tabkongjianDlg.h"
#include "afxdialogex.h"
#include "CMytab.h"
#include"CMyMemory.h"
#include"CMyClear.h"
#include"CMyDeskWnd.h"
#include"CMyKillVirus.h"
#include"CMyServer.h"
#include"CMySoftware.h"
#include"peGuiDlg.h"
#include"processDlg.h"
#include"CMyFileInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtabkongjianDlg 对话框

//注入的dll路径
const char* DllName = "C:\\Users\\Administrator\\Desktop\\Hook\\Debug\\HookDll.dll";


CtabkongjianDlg::CtabkongjianDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TABKONGJIAN_DIALOG, pParent)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_IsWindowsHide = TRUE;
}

void CtabkongjianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_mytab);
}

BEGIN_MESSAGE_MAP(CtabkongjianDlg, CDialogEx)
	ON_COMMAND_RANGE(ID_Close, ID_Lock, MageComputer)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CtabkongjianDlg 消息处理程序

//线程,用户保护进程不被任务管理器关闭
DWORD WINAPI PauseThread(LPVOID lpThreadParameter)
{
	while (1)
	{
		HANDLE Semaphore = CreateSemaphoreA(NULL, GetCurrentProcessId(), GetCurrentProcessId() + 1, "MYPID");

		// 1. 通过窗口名称确定目标的 Pid
		DWORD Pid = 0, Size = 0;
		HWND hWnd = FindWindowA(NULL, "任务管理器");
		GetWindowThreadProcessId(hWnd, &Pid);

		// 2. 使用对应的权限打开目标进程
		HANDLE Handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);

		// 3. 申请空间，主要用于存放的是 LoadLibrary 的参数
		LPVOID Addr = VirtualAllocEx(Handle, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		// 4. 写入数据
		WriteProcessMemory(Handle, Addr, DllName, strlen(DllName) + 1, &Size);

		// 5. 创建远程线程
		CreateRemoteThread(Handle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, Addr, NULL, NULL);

		//等待任务管理器进程
		WaitForSingleObject(Handle, -1);

		//关闭句柄
		CloseHandle(Handle);
		CloseHandle(Semaphore);
	}
	return 0;
}
BOOL CtabkongjianDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//在管理员权限下,获得拖拽文件的权限
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	//为tab控件添加列表项
	m_mytab.InsertItemText(9, L"进程管理", L"内存管理", L"桌面窗口", L"垃圾清理", L"服务管理", L"杀毒", L"PE文件解析",L"软件卸载", L"文件信息");

	//为tab控件添加对话框
	m_mytab.InsertDialogs(9, new CprocessDlg, IDD_PROCESS_DIALOG,new CMyMemory,IDD_Memory,new CMyDeskWnd,IDD_DeskWnd,new CMyClear,IDD_Clear,new CMyServer,IDD_Server,new CMyKillVirus,IDD_KillVirus,new CpeGuiDlg, IDD_PEGUI_DIALOG,new CMySoftware,IDD_Software,new CMyFileInfo,IDD_fileInfo);

	//设置菜单
	CMenu tabMenu;
	tabMenu.LoadMenuW(IDR_MENU1);
	SetMenu(&tabMenu);

	//注册老板键 crtl+ shift + H
	::RegisterHotKey(this->GetSafeHwnd(), 0xa001, MOD_CONTROL | MOD_SHIFT, 'H');

	//创建线程,用户保护进程不被任务管理器关闭
	CreateThread(NULL, NULL, PauseThread, (LPVOID)this,NULL,NULL);

	return TRUE;  
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtabkongjianDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtabkongjianDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//获得关机权限
void CtabkongjianDlg::MyUpdatePrivileges()
{
	//得到当前进程句柄
	HANDLE hProcess = GetCurrentProcess();

	//令牌句柄
	HANDLE hToken;

	//打开进程句柄,获取令牌句柄 以查询与修改的权限打开
	OpenProcessToken(hProcess, TOKEN_QUERY| TOKEN_ADJUST_PRIVILEGES, &hToken);

	//令牌的属性
	TOKEN_PRIVILEGES tp = { 0 };

	//将关机权限写入令牌属性结构体
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//修改进程的令牌属性
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}

//设置休眠,睡眠的函数原型
typedef  BOOLEAN(__stdcall *FUN)(_In_ BOOLEAN bHibernate, _In_ BOOLEAN bForce, _In_ BOOLEAN bWakeupEventsDisabled);

//响应菜单消息,关机,休眠,锁屏等
void CtabkongjianDlg::MageComputer(UINT id)
{
	//获得关机权限
	MyUpdatePrivileges();
	
	//获得休眠,睡眠的函数地址
	HMODULE hMod=LoadLibrary(L"powrprof.dll");
	FUN mysetsuspend=(FUN)GetProcAddress(hMod, "SetSuspendState");

	//根据菜单id,执行不同的功能
	switch (id)
	{
	case 32777:
		ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, NULL);//关机
		break;
	case 32778:
		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, NULL);//重启
		break;
	case 32779:
		ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, NULL);//注销
		break;
	case 32780:
		mysetsuspend(TRUE, FALSE, FALSE);//休眠
		break;
	case 32781:
		mysetsuspend(FALSE, FALSE, FALSE);//睡眠
		break;
	case 32782:
		LockWorkStation();//锁屏
		break;
	default:
		break;
	}
}

//消息预处理,用于响应老板键
BOOL CtabkongjianDlg::PreTranslateMessage(MSG* pMsg)
{
	//当消息类型是热键,并且参数id是老板键的id时
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0xa001))
	{
		//判断窗口状态,当为隐藏,就显示,当显示,就隐藏窗口
		if (m_IsWindowsHide == TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowsHide = FALSE;
		}
		else
		{
			ShowWindow(SW_NORMAL);
			m_IsWindowsHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

