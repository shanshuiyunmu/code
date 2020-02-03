#pragma once
#include<vector>
using namespace std;
// CMyClear 对话框

class CMyClear : public CDialogEx
{
	DECLARE_DYNAMIC(CMyClear)

public:
	CMyClear(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyClear();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Clear};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//待检测数组
	vector<CString> m_WaitPath;

	//待删除数组
	vector<CString> m_DeletePath;

	//总文件大小
	DWORD allFileCount = 0;

	//总文件数量
	DWORD deleteFileCount = 0;

	//能删除的文件的总大小
	ULONGLONG allFileSize = 0;

	//能删除的文件的数量
	ULONGLONG deleteFileSize = 0;

	DECLARE_MESSAGE_MAP()
public:
	//文件路径
	CString m_pathfile;

	//文件后缀
	CString m_houzhuifile;

	//响应按钮 扫描系统垃圾 消息
	afx_msg void OnBnClickedlooksyslaji();

	//响应按钮 扫描浏览器垃圾 消息
	afx_msg void OnBnClickedexplorelaji();

	//响应按钮 扫描垃圾 消息
	afx_msg void OnBnClickedLooklaji();

	//遍历文件信息
	VOID EnumFileNum(CString &fileName, CString &fileBackName, CListCtrl & showList);

	//删除文件信息
	void MyDeleteFiles();

	//系统垃圾列表框控件
	CListCtrl m_sysList;

	//浏览器垃圾列表框控件
	CListCtrl m_explorelist;
	
	//vs列表框控件
	CListCtrl m_vsList;

	//响应按钮 删除垃圾 消息
	afx_msg void OnBnClickeddeletelaji();

	//响应拖拽文件消息
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
