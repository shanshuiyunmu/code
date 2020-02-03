
// processDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "processDlg.h"
#include "afxdialogex.h"
#include "CMyThread.h"
#include "CMyModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CprocessDlg 对话框



CprocessDlg::CprocessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_DIALOG, pParent)
{
	
}

void CprocessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_proclist, m_process);
}

BEGIN_MESSAGE_MAP(CprocessDlg, CDialogEx)
	ON_COMMAND_RANGE(ID_proflush, ID_closePro,OnProcessMenu)
	ON_NOTIFY(NM_RCLICK, IDC_proclist, &CprocessDlg::OnNMRClickList1)
	ON_BN_CLICKED(IDC_tiquan, &CprocessDlg::OnBnClickedtiquan)
END_MESSAGE_MAP()


// CprocessDlg 消息处理程序

//获得debug的权限
void CprocessDlg::MyUpdatePrivileges()
{
	//得到当前进程句柄
	HANDLE hProcess = GetCurrentProcess();

	//令牌句柄
	HANDLE hToken;

	//得到当前进程的令牌句柄,以查询,修改的权限打开
	OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken);

	//令牌的属性结构体
	TOKEN_PRIVILEGES tp = { 0 };

	//写入debug的权限到令牌属性
	LookupPrivilegeValue(0, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//让令牌得到debug的权限
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}

//在列表控件上显示进程信息
void CprocessDlg::ShowProcess(int i,PROCESSENTRY32 pro)
{
	//列表控件 插入行信息,进程名
	m_process.InsertItem(i, pro.szExeFile);
	CString temp;

	//设置行信息,进程id
	temp.Format(L"%d", pro.th32ProcessID);
	m_process.SetItemText(i, 1, temp);

	//设置行信息,父进程id
	temp.Format(L"%d", pro.th32ParentProcessID);
	m_process.SetItemText(i, 2, temp);

	//设置行信息,线程数量
	temp.Format(L"%d", pro.cntThreads);
	m_process.SetItemText(i, 3, temp);

	//提权,得到debug权限
	MyUpdatePrivileges();

	//得到进程句柄
	HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION,
		FALSE, pro.th32ProcessID);

	//设置进程路径变量
	DWORD PathSize = MAX_PATH;
	WCHAR Path[MAX_PATH] = { 0 };

	//获得进程路径
	QueryFullProcessImageName(Handle, 0, Path, &PathSize);

	//设置行信息,进程路径
	m_process.SetItemText(i, 4, Path);

	//关闭句柄
	CloseHandle(Handle);
}


BOOL CprocessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//查询进程权限,设置提权按键是否隐藏
	QueryPrivileges();

	//设置列表 风格与插入列信息
	m_process.MyInsertColumn(5, L"进程名",0,200, L"PID", 2, 100 ,L"父进程ID", 2, 100, L"线程数量", 2, 100, L"进程路径",0, 500);

	//遍历进程
	lookProcess();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//遍历进程
void CprocessDlg::lookProcess()
{
	//拍摄进程快照
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//判断快照拍摄是否成功
	if (Snapshot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"进程快照创建失败");
		return;
	}

	//创建进程信息结构体
	PROCESSENTRY32 ProcessInfo = { sizeof(PROCESSENTRY32) };
	int i = 0;

	//第一次遍历快照,如果成功进行下一次遍历
	if (Process32First(Snapshot, &ProcessInfo))
	{
		do {

			//显示进程信息到列表控件
			ShowProcess(i,ProcessInfo);
			i++;
		} while (Process32Next(Snapshot, &ProcessInfo));
	}

	//关闭快照句柄
	CloseHandle(Snapshot);
}

//响应进程子菜单 结束进程,打开线程,打开模块,刷新
void CprocessDlg::OnProcessMenu(UINT id)
{
	//根据菜单id,进行不同的操作
	switch (id)
	{
		//刷新
	case ID_proflush:
		m_process.DeleteAllItems();
		lookProcess();
		break;

		//遍历进程线程
	case ID_openThread:
	{
		CMyThread mythread(m_pid);
		mythread.DoModal();
	}
		break;

		//遍历进程模块
	case ID_openModule:
	{
		CMyModule mymodule(m_pid);
		mymodule.DoModal();
	}
		break;

		//关闭进程
	case ID_closePro:
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, m_pid);
		TerminateProcess(hProcess, 1);
		CloseHandle(hProcess);
	}
		break;
	default:
		break;
	}
}



//响应右键列表框的消息
void CprocessDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//获得当前选中行
	int hang = pNMItemActivate->iItem;

	//得到选中行的第一列信息(PID)
	CString  Text=m_process.GetItemText(hang, 1);

	//如果选中行为空,退出
	if (Text.IsEmpty())
	{
		return;
	}

	//将字符串转换成DWORD
	WCHAR* name = Text.GetBuffer();
	CHAR he[9];
	WCHAR_TO_CHAR(name, he)

	//得到选中进程的id,以便创建线程快照,模块快照时,传入进程id
	m_pid = strtol(he, nullptr, 10);

	//得到当前鼠标坐标
	CPoint point;
	GetCursorPos(&point);

	//弹出菜单
	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu* submenu=menu.GetSubMenu(0);
	submenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y,this);
	*pResult = 0;
}

//提权
void CprocessDlg::OnBnClickedtiquan()
{
	// 1. 隐藏当前窗口
	this->ShowWindow(SW_HIDE);
	// 2. 获取当前程序路径
	WCHAR szApplication[MAX_PATH] = { 0 };
	DWORD cchLength = _countof(szApplication);
	QueryFullProcessImageName(GetCurrentProcess(), 0,
		szApplication, &cchLength);
	// 3. 以管理员权限重新打开进程
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = L"runas";      // 请求提升权限
	sei.lpFile = szApplication; // 可执行文件路径
	sei.lpParameters = NULL;          // 不需要参数
	sei.nShow = SW_SHOWNORMAL; // 正常显示窗口
	if (ShellExecuteEx(&sei))
	{

	}
	else {
		exit(0);
	}
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
}

//查询进程权限
BOOL CprocessDlg::QueryPrivileges()
{
	// 1. 获得本进程的令牌
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return false;
	// 2. 获取提升类型
	TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;
	BOOL                 bIsAdmin = false;
	DWORD                dwSize = 0;


	if (GetTokenInformation(hToken, TokenElevationType, &ElevationType,
		sizeof(TOKEN_ELEVATION_TYPE), &dwSize)) {
		// 2.1 创建管理员组的对应SID
		BYTE adminSID[SECURITY_MAX_SID_SIZE];
		dwSize = sizeof(adminSID);
		CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);


		// 2.2 判断当前进程运行用户角色是否为管理员
		if (ElevationType == TokenElevationTypeLimited) {

			// a. 获取连接令牌的句柄
			HANDLE hUnfilteredToken = NULL;
			GetTokenInformation(hToken, TokenLinkedToken, (PVOID)&hUnfilteredToken,
				sizeof(HANDLE), &dwSize);

			// b. 检查这个原始的令牌是否包含管理员的SID
			if (!CheckTokenMembership(hUnfilteredToken, &adminSID, &bIsAdmin))
				return false;
			CloseHandle(hUnfilteredToken);
		}
		else {
			bIsAdmin = IsUserAnAdmin();
		}
		CloseHandle(hToken);
	}
	// 3. 判断具体的权限状况
	BOOL bFullToken = false;
	switch (ElevationType) {
	case TokenElevationTypeDefault: /* 默认的用户或UAC被禁用 */
		if (IsUserAnAdmin())  bFullToken = true; // 默认用户有管理员权限
		else                  bFullToken = false;// 默认用户不是管理员组
		break;
	case TokenElevationTypeFull:    /* 已经成功提高进程权限 */
		if (IsUserAnAdmin())  bFullToken = true; //当前以管理员权限运行
		else                  bFullToken = false;//当前未以管理员权限运行
		break;
	case TokenElevationTypeLimited: /* 进程在以有限的权限运行 */
		if (bIsAdmin)  bFullToken = false;//用户有管理员权限，但进程权限有限
		else           bFullToken = false;//用户不是管理员组，且进程权限有限
	}

	// 4. 根据权限的不同控制按钮的显示
	if (!bFullToken)
		Button_SetElevationRequiredState(::GetDlgItem(m_hWnd, IDC_tiquan), !bFullToken);
	else
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_tiquan), SW_HIDE);
	
	return TRUE;
}