// CMyFileInfo.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyFileInfo.h"
#include "afxdialogex.h"
#include "md5file.h"
#include<string>
// CMyFileInfo 对话框

IMPLEMENT_DYNAMIC(CMyFileInfo, CDialogEx)

CMyFileInfo::CMyFileInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_fileInfo, pParent)
{

}

CMyFileInfo::~CMyFileInfo()
{
}

void CMyFileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_fileName, m_fileNameList);
}


BEGIN_MESSAGE_MAP(CMyFileInfo, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_fileName, &CMyFileInfo::OnNMDblclkfilename)
	ON_BN_CLICKED(IDC_return, &CMyFileInfo::OnBnClickedreturn)
	ON_BN_CLICKED(IDC_gengmulu, &CMyFileInfo::OnBnClickedgengmulu)
	ON_NOTIFY(NM_RCLICK, IDC_fileName, &CMyFileInfo::OnNMRClickfilename)
	ON_COMMAND(ID_GetMd5, &CMyFileInfo::OnGetmd5)
	ON_COMMAND(ID_COPYMD5, &CMyFileInfo::OnCopymd5)
END_MESSAGE_MAP()


// CMyFileInfo 消息处理程序


void CMyFileInfo::OnNMDblclkfilename(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//得到点击的行的第一列信息,文件路径
	CString dir = m_fileNameList.GetItemText(pNMItemActivate->iItem, 0);

	//当路径是文件夹
	if (GetFileAttributes(dir)&FILE_ATTRIBUTE_DIRECTORY)
	{
		//对路径进行处理,获得下一层文件路径
		if (dir.GetString()[wcslen(dir) - 1] == '\\')
		{
			dir.Delete(wcslen(dir) - 1, wcslen(dir));
		}
		TCHAR* lppPart = { 0 };
		WCHAR buff[MAX_PATH] = { 0 };
		GetFullPathNameW(dir.GetBuffer(), MAX_PATH, buff, &lppPart);

		//显示下一层文件路径
		PrintList(dir);
	}
	else
	{
		//文件,直接打开
		ShellExecute(NULL, L"open", dir, NULL, NULL, SW_SHOW);
	}
	*pResult = 0;
}

//显示信息到列表控件
VOID CMyFileInfo::PrintList(CString &FileName)
{
	m_fileNameList.DeleteAllItems();

	//如果第一次进入,就显示磁盘路径
	if (FileName == "first" || FileName == L"这台电脑")
	{

		//得到所有的磁盘文件路径
		WCHAR filePath[MAX_PATH];
		int numDrive = GetLogicalDriveStrings(MAX_PATH, filePath);

		for (WCHAR* pPath = filePath; *pPath; pPath += wcslen(pPath) + 1)
		{
			//如果不是DRIVE_CDROM类型的磁盘 就添加到列表控件中显示
			if (GetDriveType(pPath) != DRIVE_CDROM)
			{
				int index = m_fileNameList.GetItemCount();
				m_fileNameList.InsertItem(index, pPath);
			}
		}

	}
	else
	{
		//双击进入的:根据传入的文件路径,查找子文件与目录,并显示

		//遍历文件
		WIN32_FIND_DATA FileInfo = { 0 };
		HANDLE hFile = FindFirstFile(FileName + L"\\*", &FileInfo);
		m_lastMulu = FileName;
		if (hFile != INVALID_HANDLE_VALUE)
		{
			do {
				int index = m_fileNameList.GetItemCount();

				//去掉上层路径和当前路径
				if (CString(".") == FileInfo.cFileName || CString("..") == FileInfo.cFileName)
				{
					continue;
				}
				else
				{
					//设置列表行信息,文件路径
					m_fileNameList.InsertItem(index, FileName + L"\\" + FileInfo.cFileName);

					//设置文件创建时间,文件修改时间
					FILETIME createTime = {}, changeTime = {};
					SYSTEMTIME systemCreateTime = {}, systemChangeTime = {};

					//文件时间转系统时间
					FileTimeToLocalFileTime(&FileInfo.ftCreationTime, &createTime);
					FileTimeToSystemTime(&createTime, &systemCreateTime);
					FileTimeToLocalFileTime(&FileInfo.ftLastWriteTime, &changeTime);
					FileTimeToSystemTime(&changeTime, &systemChangeTime);

					//显示文件创建时间
					CString buf;
					buf.Format(L"%d\\%d\\%d %d:%d:%d", systemCreateTime.wYear,
						systemCreateTime.wMonth, systemCreateTime.wDay, systemCreateTime.wHour,
						systemCreateTime.wMinute, systemCreateTime.wSecond);
					m_fileNameList.SetItemText(index, 1, buf);

					//显示文件修改时间
					buf.Format(L"%d\\%d\\%d %d:%d:%d", systemChangeTime.wYear,
						systemChangeTime.wMonth, systemChangeTime.wDay, systemChangeTime.wHour,
						systemChangeTime.wMinute, systemChangeTime.wSecond);
					m_fileNameList.SetItemText(index, 2, buf);

					//设置显示文件大小的单位
					if (!(FileInfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
					{

						if ((FileInfo.nFileSizeLow / 1024) < 1)
						{
							buf.Format(L"%db", FileInfo.nFileSizeLow);
						}
						else if (FileInfo.nFileSizeLow / 1024 / 1024 < 1)
						{
							buf.Format(L"%.2lfkb", FileInfo.nFileSizeLow / 1024.0);
						}
						else if (FileInfo.nFileSizeLow / 1024 / 1024 / 1024 < 1)
						{
							buf.Format(L"%.2lfMB", FileInfo.nFileSizeLow / 1024.0 / 1024.0);
						}
						else
						{
							buf.Format(L"%.2lfGB", FileInfo.nFileSizeLow / 1024.0 / 1024.0 / 1024.0);
						}

						//显示到列表行信息, 文件大小
						m_fileNameList.SetItemText(index, 3, buf);				
					}

				}
			} while (FindNextFile(hFile, &FileInfo));
		}

		//关闭文件句柄
		FindClose(hFile);
	}

}


BOOL CMyFileInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//添加列表控件 列信息
	m_fileNameList.MyInsertColumn(5, L"文件", 0, 350, L"创建时间", 2, 200, L"修改时间", 2, 200, L"文件大小", 2, 100, L"MD5", 2, 300);
	
	//第一次显示磁盘文件路径
	PrintList(CString("first"));

	return TRUE;  
}


//响应返回一层按钮消息
void CMyFileInfo::OnBnClickedreturn()
{
	//得到当前列表选中行的第一列信息
	CString dir;
	dir = m_fileNameList.GetItemText(0, 0);

	//如果内容为空
	if (dir.IsEmpty())
	{
		//得到上一层路径
		dir = m_lastMulu;

		//对路径进行处理
		CFileFind find, find2;
		find.FindFile(dir);
		find.FindNextFile();
		dir = find.GetRoot();
		dir = find.GetRoot();
		if (dir.GetString()[wcslen(dir) - 1] == '\\')
		{
			dir.Delete(wcslen(dir) - 1, wcslen(dir));
		}

		//显示上一层目录
		PrintList(dir);
		return;
	}

	//对路径进行处理,得到上一层路径
	CFileFind find, find2;
	if (find.FindFile(dir)==0)
	{
		return;
	}
	find.FindNextFile();
	dir = find.GetRoot();
	if (dir.GetString()[wcslen(dir) - 1] == '\\')
	{
		dir.Delete(wcslen(dir) - 1, wcslen(dir));
	}
	if (dir.GetString()[wcslen(dir) - 1] == ':')
	{
		dir = L"这台电脑";
	}
	else
	{
		find.FindFile(dir);
		find.FindNextFile();
		dir = find.GetRoot();
		if (dir.GetString()[wcslen(dir) - 1] == '\\')
		{
			dir.Delete(wcslen(dir) - 1, wcslen(dir));
		}
	}

	//显示上一层文件目录信息
	PrintList(dir);
}

//响应回到磁盘目录消息
void CMyFileInfo::OnBnClickedgengmulu()
{

	//显示磁盘文件信息
	PrintList(CString("first"));
}

//响应右键列表框消息
void CMyFileInfo::OnNMRClickfilename(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//得到选中行的内容
	int hang = pNMItemActivate->iItem;
	CString  Text = m_fileNameList.GetItemText(hang, 0);

	//如果内容为空,就退出
	if (Text.IsEmpty())
	{
		return;
	}

	//得到鼠标坐标
	CPoint point;
	GetCursorPos(&point);

	//弹出菜单
	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu* submenu = menu.GetSubMenu(4);
	submenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}

//响应菜单 得到md5 消息
void CMyFileInfo::OnGetmd5()
{

	//得到列表当前选中行的第0列内容
	DWORD nNow=m_fileNameList.GetSelectionMark();
	CString  Text = m_fileNameList.GetItemText(nNow, 0);

	//字符串转string
	WCHAR* name = Text.GetBuffer();
	CHAR he[MAX_PATH];
	WCHAR_TO_CHAR(name, he)
	std::string name2 = he;

	//得到文件的md5值
	CString temp(getFileMD5(name2).c_str());

	//显示到列表控件 md5值
	m_fileNameList.SetItemText(nNow, 4, temp);
}

//响应菜单 复制md5 消息
void CMyFileInfo::OnCopymd5()
{

	//打开剪贴板
	if (OpenClipboard())
	{
		//得到选中行的md5值
		DWORD nNow = m_fileNameList.GetSelectionMark();
		CString  Text = m_fileNameList.GetItemText(nNow, 4);

		//转string
		WCHAR* name = Text.GetBuffer();
		CHAR he[MAX_PATH];
		WCHAR_TO_CHAR(name, he)
		std::string name2 = he;

		//将信息复制到剪贴板
		HGLOBAL   clipbuffer;
		char   *   buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, name2.size()+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy_s(buffer, name2.size() + 1, name2.c_str());
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}
}
