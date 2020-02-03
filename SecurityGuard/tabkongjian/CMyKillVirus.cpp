// CMyKillVirus.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyKillVirus.h"
#include "afxdialogex.h"
#include"md5file.h"
#include<string>
#include<TlHelp32.h>
// CMyKillVirus 对话框

IMPLEMENT_DYNAMIC(CMyKillVirus, CDialogEx)

CMyKillVirus::CMyKillVirus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KillVirus, pParent)
	, m_editmd5(_T("D:\\Document\\15pb\\windows原理\\Project1\\Debug"))
	, m_virtusName(_T(""))
	, m_virtusMd5(_T(""))
{

}

CMyKillVirus::~CMyKillVirus()
{
}

void CMyKillVirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITmd5, m_editmd5);
	DDX_Control(pDX, IDC_LISTmd5, m_listmdt);
	DDX_Text(pDX, IDC_EDIT1virtusname, m_virtusName);
	DDX_Text(pDX, IDC_EDITvirtusmd5, m_virtusMd5);
}


BEGIN_MESSAGE_MAP(CMyKillVirus, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONmd5, &CMyKillVirus::OnBnClickedButtonmd5)
	ON_BN_CLICKED(IDC_BUTTONmd5path, &CMyKillVirus::OnBnClickedButtonmd5path)
	ON_BN_CLICKED(IDC_BUTTONProcess, &CMyKillVirus::OnBnClickedButtonprocess)
	ON_BN_CLICKED(IDC_BUTTONconnect, &CMyKillVirus::OnBnClickedButtonconnect)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyKillVirus::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTONcommit, &CMyKillVirus::OnBnClickedButtoncommit)
	ON_MESSAGE(UM_RECV_MSG, &CMyKillVirus::OnUmRecvMsg)
	ON_MESSAGE(UM_SEND_MSG, &CMyKillVirus::OnUmSendMsg)
	ON_MESSAGE(UM_RECV_KILLVIRTUS, &CMyKillVirus::OnUmRecvKillvirtus)
	ON_MESSAGE(UM_RECV_COMMITVIRTUS, &CMyKillVirus::OnUmRecvCommitvirtus)
END_MESSAGE_MAP()

//黑名单杀进程
void CMyKillVirus::ProcessKill()
{
	//读取文件
	HANDLE hFile = CreateFile(L"..\\Debug\\黑名单进程.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwReads;
	ReadFile(hFile, m_processName, 200, &dwReads, NULL);
	CloseHandle(hFile);

	//遍历进程 进行查杀
	lookProcess();
	MessageBox(L"查杀完成");
}

//遍历进程 进行查杀
void CMyKillVirus::lookProcess()
{
	//创建快照
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//判断快照是否创建成功
	if (Snapshot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"进程快照创建失败");
		ExitProcess(-1);
	}
	
	//创建进程信息结构体
	PROCESSENTRY32 ProcessInfo = { sizeof(PROCESSENTRY32) };
	int i = 0;

	//第一次遍历快照,如果成功进行下一次遍历
	if (Process32First(Snapshot, &ProcessInfo))
	{
		do {
			//进程名转 string
			CHAR he[MAX_PATH];
			WCHAR_TO_CHAR(ProcessInfo.szExeFile, he)
			std::string name2 = he;

			//查找子串,如果进程名在文件中,就处理
			if (strstr(m_processName, name2.c_str()))
			{
				m_listmdt.InsertItem(i, ProcessInfo.szExeFile);

				//打开进程
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE,ProcessInfo.th32ProcessID);

				//结束进程
				if (TerminateProcess(hProcess, 1))
				{
					m_listmdt.SetItemText(i, 1, L"成功清除");
				}
				else 
				{
					m_listmdt.SetItemText(i, 1, L"清除失败");
				}

				//关闭句柄
				CloseHandle(hProcess);
				i++;
			}	
		} while (Process32Next(Snapshot, &ProcessInfo));
	}

	//关闭句柄
	CloseHandle(Snapshot);
}

// CMyKillVirus 消息处理程序

//md5杀毒
void CMyKillVirus::md5KillVirtus()
{
	//打开文件得到md5值
	HANDLE hFile = CreateFile(L"..\\Debug\\md5.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwReads;
	ReadFile(hFile, m_md5, 1000, &dwReads, NULL);
	CloseHandle(hFile);

	//遍历文件,比对md5值
	File(m_editmd5);
	UpdateData(FALSE);	
}

//全路径杀毒
void CMyKillVirus::md5pathKill()
{
	DWORD dwReads;
	HANDLE hFile1 = CreateFile(L"..\\Debug\\pathmd5.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = 0;
	GetFileSize(hFile1, &fileSize);
	m_file = new char[fileSize] {0};
	while (1)
	{
		PATHMD5 temp;
		ReadFile(hFile1, &temp, sizeof(PATHMD5), &dwReads, NULL);
		if (dwReads != sizeof(PATHMD5))
		{
			break;
		}
		m_vectorfile.push_back(temp);
	} 
	CloseHandle(hFile1);
	UpdateData(TRUE);
	Filemd5path(m_editmd5);
	UpdateData(FALSE);
}

//遍历文件,比对md5值
VOID CMyKillVirus::File(CString &FilePath)
{
	// 1. 拼接想要遍历的路径
	CString FindPath = FilePath + L"\\*";

	// 提供结构体用于保存遍历到的信息
	WIN32_FIND_DATA FileInfo = { 0 };

	// 尝试遍历第一个文件\文件夹
	HANDLE FindHande = FindFirstFileW(FindPath, &FileInfo);
	int index = 0;

	// 如果遍历成功，则输出信息并继续遍历
	if (FindHande != INVALID_HANDLE_VALUE)
	{
		do {
			// 判断是否是一个目录
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 判断文件夹是否是 . 和 ..，排除掉进行递归
				if (wcscmp(L".", FileInfo.cFileName) &&
					wcscmp(L"..", FileInfo.cFileName))
				{
					// 拼接新的路径，下一层需要查找的路径
					FilePath = FilePath + L"\\" + FileInfo.cFileName;

					// 递归调用函数
					File(FilePath);
				}
			}
			else
			{
				//列表控件显示进程名
				CString Text = FilePath + L"\\" + FileInfo.cFileName;
				m_listmdt.InsertItem(index, FileInfo.cFileName);

				//CString 转string
				WCHAR* name = Text.GetBuffer();
				CHAR he[MAX_PATH];
				WCHAR_TO_CHAR(name, he)
				std::string name2 = he;

				//得到文件的md5值
				std::string md5=getFileMD5(name2.c_str());

				//查找子串, 判断md5值是否在文件中
				if (strstr(m_md5, md5.c_str()))
				{

					//显示查杀结果
					m_listmdt.SetItemText(index, 1, L"通过");
				}
				else
				{
					m_listmdt.SetItemText(index, 1, L"MD5不匹配");
				}
				index++;
			}

		} while (FindNextFile(FindHande, &FileInfo));
	}
	FindClose(FindHande);
}

//云杀毒时 遍历文件
VOID CMyKillVirus::Fileyun(CString &FilePath)
{
	CString FindPath = FilePath + L"\\*";
	WIN32_FIND_DATA FileInfo = { 0 };
	HANDLE FindHande = FindFirstFileW(FindPath, &FileInfo);
	int index = 0;
	if (FindHande != INVALID_HANDLE_VALUE)
	{
		do {
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(L".", FileInfo.cFileName) &&
					wcscmp(L"..", FileInfo.cFileName))
				{
					FilePath = FilePath + L"\\" + FileInfo.cFileName;
					File(FilePath);
				}
			}
			else
			{
				//获得进程的路径,计算出md5 值
				// 得到进程名
				CString Text = FilePath + L"\\" + FileInfo.cFileName;
				CString virtusname = FileInfo.cFileName;

				//CString 转 string
				WCHAR* name = Text.GetBuffer();
				CHAR he[MAX_PATH];
				WCHAR_TO_CHAR(name, he)
				std::string name2 = he;
				std::string md5 = getFileMD5(name2.c_str());
				CString md5yun(md5.c_str());

				//打包md5和进程名,发送消息给服务器
				SENDINFO sendInfo = { };
				sendInfo.type = MyEnum::KILLVIRTUS;
				sendInfo.index = index;
				memcpy(sendInfo.dateinfo.md5, md5yun.GetBuffer(), md5yun.GetLength() * 2);
				memcpy(sendInfo.dateinfo.name, virtusname.GetBuffer(), virtusname.GetLength() * 2);
				SendMessage(UM_SEND_MSG, (WPARAM)&sendInfo, sizeof(SENDINFO));
				index++;
				Sleep(100);
			}

		} while (FindNextFile(FindHande, &FileInfo));
	}
	FindClose(FindHande);
}

//响应按钮 md5杀毒
void CMyKillVirus::OnBnClickedButtonmd5()
{
	m_listmdt.DeleteAllItems();
	md5KillVirtus();
}


BOOL CMyKillVirus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listmdt.MyInsertColumn(2, L"程序名", 0, 200, L"检测结果", 0, 100);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//响应按钮 全路径杀毒
void CMyKillVirus::OnBnClickedButtonmd5path()
{
	m_listmdt.DeleteAllItems();
	md5pathKill();
}

//全路径查杀 遍历文件
VOID CMyKillVirus::Filemd5path(CString &FilePath)
{
	// 1. 拼接想要遍历的路径
	CString FindPath = FilePath + L"\\*";

	// 提供结构体用于保存遍历到的信息
	WIN32_FIND_DATA FileInfo = { 0 };

	// 尝试遍历第一个文件\文件夹
	HANDLE FindHande = FindFirstFileW(FindPath, &FileInfo);
	int index = 0;
	// 如果遍历成功，则输出信息并继续遍历
	if (FindHande != INVALID_HANDLE_VALUE)
	{
		do {
			// 判断是否是一个目录
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 判断文件夹是否是 . 和 ..，排除掉进行递归
				if (wcscmp(L".", FileInfo.cFileName) &&
					wcscmp(L"..", FileInfo.cFileName))
				{
					// 拼接新的路径，下一层需要查找的路径
					FilePath = FilePath + L"\\" + FileInfo.cFileName;

					// 递归调用函数
					File(FilePath);
				}
			}
			else
			{
				CString Text = FilePath + L"\\" + FileInfo.cFileName;
				m_listmdt.InsertItem(index, FileInfo.cFileName);

				WCHAR* name = Text.GetBuffer();
				CHAR he[MAX_PATH];
				WCHAR_TO_CHAR(name, he)
				std::string name2 = he;
				std::string md5 = getFileMD5(name2.c_str());

				bool flag = FALSE;
				
				for (UINT i = 0; i < m_vectorfile.size(); i++)
				{
					//判断md5是否匹配
					if (strcmp(md5.c_str(), m_vectorfile[i].md5) == 0)
					{
						flag = TRUE;
						//判断路径是否匹配
						if (strcmp(name2.c_str(), m_vectorfile[i].path) == 0)
						{
							m_listmdt.SetItemText(index, 1, L"通过");
						}
						else
						{
							m_listmdt.SetItemText(index, 1, L"路径不正确");
						}
					}
				}
				if (flag == FALSE)
				{
					m_listmdt.SetItemText(index, 1, L"MD5不匹配");
				}
				index++;
			}

		} while (FindNextFile(FindHande, &FileInfo));
	}
	FindClose(FindHande);
}

//响应按钮 黑名单杀进程
void CMyKillVirus::OnBnClickedButtonprocess()
{
	m_listmdt.DeleteAllItems();

	ProcessKill();
}

//连接服务器
void CMyKillVirus::OnBnClickedButtonconnect()
{
	//设置主窗口
	m_socket.SetMainWnd(this);
	//设置ip,与端口
	m_socket.ConnectServer("127.0.0.1", 6666, m_flag);

	//连接成功隐藏按钮
	if (m_flag == 2)
	{
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_BUTTONconnect), SW_HIDE);
	}
	else
	{
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_BUTTONconnect), SW_NORMAL);
	}
}

//云查杀
void CMyKillVirus::OnBnClickedButton2()
{
	UpdateData(TRUE);
	//判断是否连接服务器
	if (m_flag == 1)
	{
		MessageBox(L"请先连接服务器",L"提示");
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_BUTTONconnect), SW_NORMAL);
		return;
	}
	else
	{
		//清空列表框
		m_listmdt.DeleteAllItems();
		
		//云杀毒 遍历文件
		Fileyun(m_editmd5);
		
	}	
	UpdateData(FALSE);
}

//提交样本
void CMyKillVirus::OnBnClickedButtoncommit()
{
	UpdateData(TRUE);

	//判断是否连接服务器
	if (m_flag == 1)
	{
		MessageBox(L"请先连接服务器", L"提示");
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_BUTTONconnect), SW_NORMAL);
		return;
	}

	//判断提交信息是否为空
	if (m_virtusName.IsEmpty() || m_virtusMd5.IsEmpty())
	{
		MessageBox(L"病毒名或者md5为空", L"友情提示", MB_OK);
		return;
	}

	//打包信息,发送给 处理发送消息
	SENDINFO sendInfo = { };
	sendInfo.type = MyEnum::COMMITVIRTUS;
	memcpy(sendInfo.dateinfo.name, m_virtusName.GetBuffer(), m_virtusName.GetLength() * 2);
	memcpy(sendInfo.dateinfo.md5, m_virtusMd5.GetBuffer(), m_virtusMd5.GetLength() * 2);
	SendMessage(UM_SEND_MSG, (WPARAM)&sendInfo, sizeof(SENDINFO));
}

//响应接收消息
afx_msg LRESULT CMyKillVirus::OnUmRecvMsg(WPARAM wParam, LPARAM lParam)
{
	PRECVINFO recvInfo = (PRECVINFO)lParam;

	//根据消息的类型,分发给不同的处理消息
	switch (recvInfo->type)
	{

		//云杀毒消息
	case MyEnum::KILLVIRTUS:
		::SendMessage(this->GetSafeHwnd(), UM_RECV_KILLVIRTUS, wParam, lParam);
		break;

		//样本提交消息
	case MyEnum::COMMITVIRTUS:
		::SendMessage(this->GetSafeHwnd(), UM_RECV_COMMITVIRTUS, wParam, lParam);
		break;
	default:
		break;
	}

	return 0;
}

//发送消息给socket
afx_msg LRESULT CMyKillVirus::OnUmSendMsg(WPARAM wParam, LPARAM lParam)
{
	m_socket.SendDataServer((LPVOID)wParam, lParam);
	return 0;
}

//处理 接收到的云杀毒 消息
afx_msg LRESULT CMyKillVirus::OnUmRecvKillvirtus(WPARAM wParam, LPARAM lParam)
{
	//根据服务器的处理过的消息,输出到杀毒列表框
	PRECVINFO recvInfo = (PRECVINFO)lParam;
	m_listmdt.InsertItem(recvInfo->index, recvInfo->dateinfo.name);
	m_listmdt.SetItemText(recvInfo->index, 1, recvInfo->dateinfo.md5);
	return 0;
}

//处理 接收到的提交样本 消息
afx_msg LRESULT CMyKillVirus::OnUmRecvCommitvirtus(WPARAM wParam, LPARAM lParam)
{
	//根据服务器的处理过的信息,输出到消息框
	PRECVINFO recvInfo = (PRECVINFO)lParam;
	CString temp;
	temp.Format(L"%s\n%s", recvInfo->dateinfo.name, recvInfo->dateinfo.md5);
	MessageBox(temp, L"提示");
	return 0;
}
