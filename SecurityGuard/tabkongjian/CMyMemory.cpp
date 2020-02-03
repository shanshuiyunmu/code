// CMyMemory.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyMemory.h"
#include "afxdialogex.h"
#include<Psapi.h>

// CMyMemory 对话框

IMPLEMENT_DYNAMIC(CMyMemory, CDialogEx)

CMyMemory::CMyMemory(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Memory, pParent)
	, m_strcpu(_T(""))
	, m_memorystr(_T(""))
{

}

CMyMemory::~CMyMemory()
{
}

void CMyMemory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_cpu, m_strcpu);
	DDX_Text(pDX, IDC_memory, m_memorystr);
}


BEGIN_MESSAGE_MAP(CMyMemory, CDialogEx)
	ON_BN_CLICKED(IDC_releaseMem, &CMyMemory::OnBnClickedreleasemem)
	ON_BN_CLICKED(IDC_flushmem, &CMyMemory::OnBnClickedflushmem)
END_MESSAGE_MAP()

//时间转换
double CMyMemory::FILETIME2Double(const FILETIME & fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}

// CMyMemory 消息处理程序

//得到cpu的使用情况
INT CMyMemory::GetCpuUsage()
{
	//空闲时间,内核时间,用户时间
	FILETIME idleTime, kernelTime, userTime;

	//获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	//等待1秒,使用内核对象等待
	WaitForSingleObject(hEvent, 1000);

	//获取新时间
	FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

	//将各个时间转换
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dNewUserTime = FILETIME2Double(newUserTime);

	//计算出使用率
	return int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}


BOOL CMyMemory::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);

	//显示cpu 与内存
	DWORD cpunum = GetCpuUsage();
	MEMORYSTATUS memnum = GetMemState();
	m_strcpu.Format(L"%d%%", cpunum);
	m_memorystr.Format(L"%d%%", memnum.dwMemoryLoad);
	UpdateData(FALSE);
	return TRUE;  
}

//得到内存状态
MEMORYSTATUS CMyMemory::GetMemState()
{
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	return memStatus;
}


//释放内存
void CMyMemory::OnBnClickedreleasemem()
{
	//获取当前的内存状态
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUseMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;

	//清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}

	//获得清理后的内存状态
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	m_memorystr.Format(L"%d%%", afterCleanUsedMem/100000000);
	UpdateData(FALSE);
}


//刷新显示
void CMyMemory::OnBnClickedflushmem()
{
	UpdateData(TRUE);
	UINT cpunum = GetCpuUsage();
	MEMORYSTATUS memnum = GetMemState();
	   
	m_strcpu.Format(L"%d%%", cpunum);
	m_memorystr.Format(L"%d%%", memnum.dwMemoryLoad);
	UpdateData(FALSE);
}
