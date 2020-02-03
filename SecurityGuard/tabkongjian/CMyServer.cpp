// CMyServer.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyServer.h"
#include "afxdialogex.h"



// CMyServer 对话框

IMPLEMENT_DYNAMIC(CMyServer, CDialogEx)

CMyServer::CMyServer(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Server, pParent)
{

}

CMyServer::~CMyServer()
{
}

void CMyServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SericeList, m_sericelist);
}

//遍历服务
void CMyServer::LookSerice()
{
	//得到服务管理器句柄
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;

	//遍历服务
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO,SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (PBYTE)pEnumSerice, dwSize, &dwSize, &dwServiceNum, NULL, NULL);
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//服务显示到列表控件
		ShowSerice(i,pEnumSerice[i]);
	}
	CloseServiceHandle(hSCM);
}

//响应菜单 开启服务, 停止服务
void CMyServer::OnSerice(UINT id)
{
	if (id == ID_sericeStart)
	{
		//得到服务管理器句柄
		SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		//得到服务句柄
		SC_HANDLE hService = OpenService(hSCM, m_sericename, SERVICE_START);

		//启动服务
		StartService(hService, NULL, NULL);

		//关闭句柄
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
	}
	else if (id == ID_sericeStop)
	{
		SERVICE_STATUS_PROCESS  ssp;
		SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		SC_HANDLE hService = OpenService(hSCM, m_sericename, SERVICE_STOP);

		//停止服务
		ControlService(hService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssp);
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
	}
	else if (ID_flushSerice == id)
	{
		//刷新列表
		m_sericelist.DeleteAllItems();
		LookSerice();
		UpdateData(FALSE);
	}
	UpdateData(FALSE);
}

//显示服务信息到列表控件
void CMyServer::ShowSerice(int i,ENUM_SERVICE_STATUS_PROCESS & serice)
{
	//显示服务名
	m_sericelist.InsertItem(i, serice.lpServiceName);

	//显示服务状态
	CString tempname;
	switch (serice.ServiceStatusProcess.dwCurrentState)
	{
	case SERVICE_CONTINUE_PENDING:
		tempname = L"即将继续";
		break;
	case SERVICE_PAUSE_PENDING:
		tempname = L"即将暂停";
		break;
	case SERVICE_PAUSED:
		tempname = L"已暂停";
		break;
	case SERVICE_RUNNING:
		tempname = L"正在运行";
		break;
	case SERVICE_START_PENDING:
		tempname = L"正在开始";
		break;
	case SERVICE_STOP_PENDING:
		tempname = L"正在停止";
		break;
	case SERVICE_STOPPED:
		tempname = L"已停止";
		break;
	default:
		break;
	}
	m_sericelist.SetItemText(i, 1, tempname);

	//显示服务提示信息
	tempname.Format(L"%s", serice.lpDisplayName);
	m_sericelist.SetItemText(i, 2, tempname);

	//显示服务类型
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCM, serice.lpServiceName, SERVICE_QUERY_CONFIG);
	DWORD dwSize;
	QueryServiceConfig(hService, NULL, 0, &dwSize);
	auto pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
	QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
	switch (pServiceConfig->dwStartType)
	{
	case 2:
		tempname = L"自动";
		break;
	case 3:
		tempname = L"手动";
		break;
	case 4:
		tempname = L"禁用";
		break;
	default:
		tempname.Format(L"%d", pServiceConfig->dwStartType);
		break;
	}
	m_sericelist.SetItemText(i, 3, tempname);

	//显示服务路径
	tempname.Format(L"%s", pServiceConfig->lpBinaryPathName);
	m_sericelist.SetItemText(i, 4, tempname);
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}


BEGIN_MESSAGE_MAP(CMyServer, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_SericeList, &CMyServer::OnNMRClickSericelist)
	ON_COMMAND_RANGE(ID_sericeStart, ID_flushSerice,OnSerice)
END_MESSAGE_MAP()


// CMyServer 消息处理程序


BOOL CMyServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);

	//设置列表列信息
	m_sericelist.MyInsertColumn(5, L"服务名", 0, 200, L"服务状态", 2, 100, L"服务描述", 0, 200, L"启动类型", 2, 100, L"路径", 0, 400);

	//遍历服务
	LookSerice();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//响应列表控件右键消息
void CMyServer::OnNMRClickSericelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//得到选中行的信息
	int hang = pNMItemActivate->iItem;
	CString  Text = m_sericelist.GetItemText(hang, 0);

	//如果内容为空,退出
	if (Text.IsEmpty())
	{
		return;
	}
	//服务名保存到变量中
	wcscpy_s(m_sericename,50, Text.GetBuffer());

	//得到鼠标的坐标
	CPoint point;
	GetCursorPos(&point);

	//弹出菜单
	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu* submenu = menu.GetSubMenu(2);
	submenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}
