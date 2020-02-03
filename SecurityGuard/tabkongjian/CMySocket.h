#pragma once


class CMySocket
{
public:
	CMySocket();
	~CMySocket();

	//用于检查错误
	bool CheckResult(BOOL result, LPCWSTR errMsg);

	//设置主窗口指针
	void SetMainWnd(CDialogEx* pMainWnd);

	//连接服务器
	void ConnectServer(LPCSTR ip, WORD port, int &flag);

	//发送消息给服务器
	void SendDataServer(LPVOID sendInfo, LPARAM size);
	
	int *m_flagsocket;
public:

	//主窗口指针变量
	CDialogEx* m_pMainWnd;

	//套接字变量
	SOCKET m_client;
};

