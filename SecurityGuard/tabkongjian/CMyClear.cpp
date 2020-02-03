// CMyClear.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyClear.h"
#include "afxdialogex.h"


// CMyClear 对话框

IMPLEMENT_DYNAMIC(CMyClear, CDialogEx)

CMyClear::CMyClear(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Clear, pParent)
	, m_pathfile(_T("D:\\Document\\15pb\\C"))
	, m_houzhuifile(_T(".obj\\.tlog\\.lastbuildstate\\.idb\\.pdb\\.pch\\.res\\.ilk\\.exe\\.sdf\\.ipch\\.log\\"))
{

}

CMyClear::~CMyClear()
{
}

void CMyClear::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITPATH, m_pathfile);
	DDX_Text(pDX, IDC_EDITHOUZHUI, m_houzhuifile);
	DDX_Control(pDX, IDC_syslIst, m_sysList);
	DDX_Control(pDX, IDC_exploreList, m_explorelist);
	DDX_Control(pDX, IDC_vsList, m_vsList);
}


BEGIN_MESSAGE_MAP(CMyClear, CDialogEx)
	ON_BN_CLICKED(IDC_looksyslaji, &CMyClear::OnBnClickedlooksyslaji)
	ON_BN_CLICKED(IDC_explorelaji, &CMyClear::OnBnClickedexplorelaji)
	ON_BN_CLICKED(IDC_LOOKLAJI, &CMyClear::OnBnClickedLooklaji)
	ON_BN_CLICKED(IDC_deleteLaji, &CMyClear::OnBnClickeddeletelaji)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMyClear 消息处理程序

//系统垃圾
void CMyClear::OnBnClickedlooksyslaji()
{
	UpdateData(TRUE);

	//清理回收站的垃圾
	SHQUERYRBINFO RecycleBinInfo = {sizeof(SHQUERYRBINFO)};
	SHQueryRecycleBin(NULL, &RecycleBinInfo);
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);

	//设置系统垃圾的路径
	CString path1(L"C:\\Windows\\Temp");
	CString path2(L"C:\\Users\\Administrator\\AppData\\Local\\Temp");
	CString path3(L"C:\\Users\\Administrator\\AppData\\Local\\Microsoft\\Windows\\WER\\ReportQueue");

	//设置系统垃圾的文件后缀
	CString houzhui(L".obj\\.log\\.ilk\\.pdb\\.pch\\.tlog\\");

	m_sysList.DeleteAllItems();

	//有几个文件夹,待删除的数组
	m_WaitPath.push_back(path1);
	m_WaitPath.push_back(path2);
	m_WaitPath.push_back(path3);

	for (UINT i = 0; i < m_WaitPath.size(); i++)
	{
		//遍历文件的函数.
		EnumFileNum(m_WaitPath[i], houzhui, m_sysList);
	}

	//每个文件夹进行遍历文件,计算出所有文件的大小,数量,待删除文件的大小和数量.

	//计算总个数与大小
	CString allNum;
	allNum.Format(L"总文件个数是%d,总文件大小是%.3lfMB\n 可删除文件的个数是%d,可删除文件的大小是%.3lfMB\n", allFileCount, allFileSize / 1024.0 / 1024.0, deleteFileCount, deleteFileSize / 1024.0 / 1024.0);

	allFileCount = 0; 
	allFileSize = 0; 
	deleteFileCount = 0;
	deleteFileSize = 0;
	m_WaitPath.clear();
	UpdateData(FALSE);
	MessageBox(allNum);
}

//浏览器垃圾
void CMyClear::OnBnClickedexplorelaji()
{
	UpdateData(TRUE);

	//设置浏览器垃圾路径
	/*CString path1(L"C:\\Users\\Administrator\\AppData\\Local");*/
	CString path2(L"C:\\Users\\Administrator\\AppData\\Local\\Temp");
	CString path3(L"C:\\Users\\Administrator\\AppData\\Local\\Microsoft\\Windows");

	//设置浏览器垃圾后缀
	CString houzhui(L".obj\\.log\\.ilk\\.pdb\\.pch\\.tlog\\");

	m_explorelist.DeleteAllItems();

	//有几个文件夹,待删除的数组
	/*m_WaitPath.push_back(path1);*/
	m_WaitPath.push_back(path2);
	m_WaitPath.push_back(path3);


	for (UINT i = 0; i < m_WaitPath.size(); i++)
	{
		//遍历文件的函数.
   		EnumFileNum(m_WaitPath[i], houzhui, m_explorelist);
	}

	//每个文件夹进行遍历文件,计算出所有文件的大小,数量,待删除文件的大小和数量.
	//计算总个数与大小
	CString allNum;
	allNum.Format(L"总文件个数是%d,总文件大小是%.3lfMB\n 可删除文件的个数是%d,可删除文件的大小是%.3lfMB\n", allFileCount, allFileSize / 1024.0 / 1024.0,deleteFileCount, deleteFileSize / 1024.0 / 1024.0);

	allFileCount = 0; 
	allFileSize = 0; 
	deleteFileCount = 0;
	deleteFileSize = 0;
	m_WaitPath.clear();
	UpdateData(FALSE);
	MessageBox(allNum);
}

//vs垃圾
void CMyClear::OnBnClickedLooklaji()
{
	UpdateData(TRUE);
	m_vsList.DeleteAllItems();

	//有几个文件夹,根据	CListCtrl m_Accept的行数确定
	m_WaitPath.push_back(m_pathfile);

	DWORD fileNumsaa = 0;

	//判断路径是否是文件夹
	if (((GetFileAttributes(m_pathfile) != -1) && (GetFileAttributes(m_pathfile)&FILE_ATTRIBUTE_DIRECTORY)) && CString(L"") != m_pathfile)
	{
		fileNumsaa = 1;
	}

	for (UINT i = 0; i < fileNumsaa; i++)
	{
		//遍历文件的函数.
		EnumFileNum(m_WaitPath[i], m_houzhuifile,m_vsList);
	}

	//每个文件夹进行遍历文件,计算出所有文件的大小,数量,待删除文件的大小和数量.
	//计算总个数与大小
	CString allNum;
	allNum.Format(L"总文件个数是%d,总文件大小是%.3lfMB\n 可删除文件的个数是%d,可删除文件的大小是%.3lfMB\n", allFileCount, allFileSize / 1024.0 / 1024.0,deleteFileCount, deleteFileSize / 1024.0 / 1024.0);

	allFileCount = 0; 
	allFileSize = 0; 
	deleteFileCount = 0; 
	deleteFileSize = 0;
	m_WaitPath.pop_back();
	UpdateData(FALSE);
	MessageBox(allNum);
}

//遍历文件信息
VOID CMyClear::EnumFileNum(CString &fileName, CString &fileBackName,CListCtrl & showList)
{
	//创建文件类型的结构体
	WIN32_FIND_DATA fileInfo = { 0 };

	//查找路径加上\\*;
	CString filePath = fileName + L"\\*";

	//找到第一个文件,得到文件句柄
	HANDLE hFile = FindFirstFile(filePath, &fileInfo);

	//找到下一个文件
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do {

			//判断是目录还是文件
			if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{

				//排除掉上级目录与本级目录.. .
				if (CString(".") == fileInfo.cFileName || CString("..") == fileInfo.cFileName)
				{

					//什么也不干
					continue;
				}

				//递归查找
				EnumFileNum(fileName + L"\\" + fileInfo.cFileName, fileBackName, showList);
			}
			else
			{
				allFileCount++;

				//文件的大小计算
				allFileSize += fileInfo.nFileSizeLow;

				//判断文件的类型是不是想要的类型,需要api,根据文件名获得 [.后缀名] pathfindextension
				if (-1 != fileBackName.Find(PathFindExtension(fileInfo.cFileName)))
				{
					deleteFileCount++;
					deleteFileSize += fileInfo.nFileSizeLow;
					m_DeletePath.push_back(fileName + L"\\" + fileInfo.cFileName);

					//输出到列表控件上
					m_DeletePath.push_back(fileName + L"\\" + fileInfo.cFileName);
					showList.InsertItem(0, fileName + L"\\" + fileInfo.cFileName);

				}
			}
		} while (FindNextFile(hFile, &fileInfo));
	}
	FindClose(hFile);
}

//删除文件信息
void CMyClear::MyDeleteFiles()
{
	int count = 0;
	//删除待删除文件的文件.
	for (auto va : m_DeletePath)
	{
		//删除文件的api
		if (1/*!DeleteFile(va)*/)
		{
			count++;
		}
	}
	CString temp;
	temp.Format(L"删除完成,有%d个文件未删除成功!", count);
	m_DeletePath.clear();
	MessageBox(temp);
}

//响应按钮 删除垃圾 消息
void CMyClear::OnBnClickeddeletelaji()
{
	//删除文件信息
	MyDeleteFiles();
}

//响应拖拽文件消息
void CMyClear::OnDropFiles(HDROP hDropInfo)
{
	//得到拖拽文件的数量
	DWORD dwFileNum = DragQueryFile(hDropInfo, -1, NULL, 0);

	//循环处理文件信息
	for (UINT i = 0; i < dwFileNum; i++)
	{
		//得到文件路径
		WCHAR fileName[100] = { 0 };
		DragQueryFile(hDropInfo, i, fileName, 100);

		//如果文件属性是目录
		if (GetFileAttributes(fileName)&FILE_ATTRIBUTE_DIRECTORY)
		{
			//显示到路径edit控件
			m_pathfile = fileName;
		}
	}
	UpdateData(FALSE);
	CDialogEx::OnDropFiles(hDropInfo);
}
