#pragma once
#include"CMyList.h"
#include"CMySocket.h"
#include<vector>
using namespace std;
// CMyKillVirus 对话框

class CMyKillVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CMyKillVirus)

public:
	CMyKillVirus(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyKillVirus();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KillVirus};
#endif

protected:

	//套接字对象
	CMySocket m_socket;

	//设置路径与md5的 结构体
	typedef struct pathmd5
	{
		char md5[50];
		char path[MAX_PATH];
	}PATHMD5, *PPATHMD5;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//从文件中读取保存md5值
	char m_md5[1000] = {};

	//保存的进程名
	char m_processName[200] = {};

	//MD5查杀 遍历文件
	VOID File(CString &FilePath);

	//全路径查杀 遍历文件
	VOID Filemd5path(CString & FilePath);

	//云杀毒时 遍历文件
	VOID Fileyun(CString & FilePath);

	//文件名
	char* m_file;

	//保存md5和路径动态数组
	vector<PATHMD5> m_vectorfile;

	//黑名单杀进程
	void ProcessKill();

	//遍历进程 进行查杀
	void lookProcess();

	//md5杀毒
	void md5KillVirtus();

	//全路径杀毒
	void md5pathKill();

	DECLARE_MESSAGE_MAP()
public:
	//标志
	int m_flag = 1;

	//病毒名称
	CString m_virtusName;

	//病毒md5值
	CString m_virtusMd5;

	//查杀路径
	CString m_editmd5;

	//杀毒列表控件
	CMyList m_listmdt;

	virtual BOOL OnInitDialog();

	//响应按钮 md5杀毒
	afx_msg void OnBnClickedButtonmd5();	

	//响应按钮 全路径杀毒
	afx_msg void OnBnClickedButtonmd5path();

	//响应按钮 黑名单杀进程
	afx_msg void OnBnClickedButtonprocess();

	//响应按钮 连接服务器
	afx_msg void OnBnClickedButtonconnect();

	//响应按钮  云查杀
	afx_msg void OnBnClickedButton2();

	//响应按钮  提交样本
	afx_msg void OnBnClickedButtoncommit();
protected:
	//响应接收消息
	afx_msg LRESULT OnUmRecvMsg(WPARAM wParam, LPARAM lParam);

	//响应发送消息
	afx_msg LRESULT OnUmSendMsg(WPARAM wParam, LPARAM lParam);

	//处理 接收到的云杀毒 消息
	afx_msg LRESULT OnUmRecvKillvirtus(WPARAM wParam, LPARAM lParam);

	//处理 接收到的提交样本 消息
	afx_msg LRESULT OnUmRecvCommitvirtus(WPARAM wParam, LPARAM lParam);
};
