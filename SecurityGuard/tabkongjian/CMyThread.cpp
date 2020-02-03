// CMyThread.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyThread.h"
#include "afxdialogex.h"


// CMyThread 对话框

IMPLEMENT_DYNAMIC(CMyThread, CDialogEx)

CMyThread::CMyThread(UINT pid,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREAD, pParent),
	m_pid(pid)
{

}

CMyThread::~CMyThread()
{
}

void CMyThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_proclist, m_threadList);
}

//遍历线程
void CMyThread::LookThread()
{
	//创建快照
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	//判断快照是否成功
	if (Snapshot == INVALID_HANDLE_VALUE)
	{
		MessageBox( L"快照创建失败");
		return;
	}

	//创建线程信息结构体
	THREADENTRY32 ThreadInfo = { sizeof(THREADENTRY32) };
	int i = 0;

	//第一次遍历快照,如果成功,就进行下一次遍历
	if (Thread32First(Snapshot, &ThreadInfo))
	{
		do {

			//筛选线程,对属于PID的线程,进行处理
			if (m_pid == ThreadInfo.th32OwnerProcessID)
			{

				//显示线程信息到列表控件
				ShowThread(i, ThreadInfo);
				i++;
			}
		} while (Thread32Next(Snapshot, &ThreadInfo));
	}

	//关闭快照句柄
	CloseHandle(Snapshot);
}

//响应菜单消息,结束线程,挂起线程,恢复线程,刷新
void CMyThread::OnThreadMenu(UINT id)
{
	//根据菜单id,响应不同操作
	switch (id)
	{

		//刷新列表
	case ID_flushThread:
		m_threadList.DeleteAllItems();
		LookThread();
		break;

		//关闭线程
	case ID_closeThread:
	{
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, m_tid);
		TerminateThread(hThread, 1);
		CloseHandle(hThread);
	}
	break;

	//挂起线程
	case ID_guaThread:
	{
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, m_tid);
		SuspendThread(hThread);
		CloseHandle(hThread);
	}
	break;

	//恢复线程
	case ID_huiThread:
	{
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, m_tid);
		ResumeThread(hThread);
		CloseHandle(hThread);
	}
	break;
	default:
		break;
	}
}

//将线程信息显示到列表控件
void CMyThread::ShowThread(int i, THREADENTRY32 thread)
{
	CString temp;

	//显示线程id
	temp.Format(L"%d", thread.th32ThreadID);
	m_threadList.InsertItem(i, temp);

	//显示线程拥有者的进程id
	temp.Format(L"%d", thread.th32OwnerProcessID);
	m_threadList.SetItemText(i, 1, temp);

	//显示线程的优先级
	temp.Format(L"%d", thread.tpBasePri);
	m_threadList.SetItemText(i, 2, temp);
}


BEGIN_MESSAGE_MAP(CMyThread, CDialogEx)
	ON_COMMAND_RANGE(ID_flushThread, ID_huiThread,OnThreadMenu)
	ON_NOTIFY(NM_RCLICK, IDC_proclist, &CMyThread::OnNMRClickproclist)
END_MESSAGE_MAP()


// CMyThread 消息处理程序


BOOL CMyThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//线程列表控件插入列信息
	m_threadList.MyInsertColumn(3, L"线程ID", 0, 150, L"进程ID", 2, 150, L"线程优先级", 2, 150);

	//遍历线程
	LookThread();

	return TRUE;  
}


//响应右键列表框消息
void CMyThread::OnNMRClickproclist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//获得选中的行
	int hang = pNMItemActivate->iItem;

	//得到选中行,第0列信息(线程id)
	CString  Text = m_threadList.GetItemText(hang, 0);

	//如果选中行内容为空,退出
	if (Text.IsEmpty())
	{
		return;
	}

	//将字符串转换成DWORD
	WCHAR* name = Text.GetBuffer();
	CHAR he[9];
	WCHAR_TO_CHAR(name, he)

	//线程id
	m_tid = strtol(he, nullptr, 10);

	//获得鼠标坐标
	CPoint point;
	GetCursorPos(&point);

	//弹出菜单
	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu* submenu = menu.GetSubMenu(1);
	submenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}
