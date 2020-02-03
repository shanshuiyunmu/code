#include "stdafx.h"
#include "CMySocket.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <WS2tcpip.h>

//构造函数
CMySocket::CMySocket()
{
	WSADATA wsaData = { 0 };
	//初始化网络环境
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	CheckResult(result != 0, L"环境初始化失败");

	CheckResult(wsaData.wVersion != MAKEWORD(2, 2), L"版本不对");
	//创建套接字
	m_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	CheckResult(m_client == SOCKET_ERROR, L"套接字创建失败");
}


CMySocket::~CMySocket()
{
	//关闭套接字,清理环境
	closesocket(m_client);
	WSACleanup();
}

//线程回调函数,用于接收消息.
DWORD WINAPI ThreadProc(LPVOID lpThreadParameter)
{
	CMySocket* pMySocket = (CMySocket*)lpThreadParameter;
	CHAR* buffer = new CHAR[sizeof(RECVINFO)]{ 0 };
	//当接收到消息,就把消息发送给主窗口
	while (recv(pMySocket->m_client, buffer, sizeof(RECVINFO), 0)>0)
	{
		SendMessage(pMySocket->m_pMainWnd->GetSafeHwnd(), UM_RECV_MSG, NULL, (LPARAM)buffer);	
	}

	delete[] buffer;
	//当接收数据错误,就是断开了网络
	MessageBox(pMySocket->m_pMainWnd->GetSafeHwnd(), L"连接中断", L"错误", MB_OK);
	int* num = pMySocket->m_flagsocket;
	*num = 1;
	closesocket(pMySocket->m_client);
	pMySocket->m_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	pMySocket->CheckResult(pMySocket->m_client == SOCKET_ERROR, L"套接字创建失败");
	/*ExitProcess(-1);*/
	return 0;
}

//用于检查错误
bool CMySocket::CheckResult(BOOL result, LPCWSTR errMsg)
{
	if (result == TRUE)
	{
		MessageBox(m_pMainWnd->GetSafeHwnd(), errMsg, L"错误", MB_OK);
		return false;
		/*ExitProcess(-1);*/
	}
	return true;
}
//设置主窗口指针
void CMySocket::SetMainWnd(CDialogEx * pMainWnd)
{
	m_pMainWnd = pMainWnd;
}

//连接服务器
void CMySocket::ConnectServer(LPCSTR ip, WORD port,int &flag)
{
	//使用TCP ,初始化ip,与端口
	m_flagsocket = &flag;
	sockaddr_in address = { 0 };
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr.S_un);
	//连接函数
	int result=connect(m_client, (SOCKADDR*)&address, sizeof(sockaddr_in));
	if(CheckResult(result == SOCKET_ERROR, L"连接服务器失败"))
	{
		flag = 2;
		MessageBox(m_pMainWnd->GetSafeHwnd(), L"连接成功", L"提示", MB_OK);
		//创建接收数据的线程
		HANDLE hResult = CreateThread(NULL, 0, ThreadProc, (LPVOID)this, NULL, NULL);
		CheckResult(hResult == INVALID_HANDLE_VALUE, L"线程创建失败");
	}
	else
	{
		flag = 1;
	}
	
}

//发送消息给服务器
void CMySocket::SendDataServer(LPVOID sendInfo,LPARAM size)
{
	int result = send(m_client, (PCHAR)sendInfo, size, 0);
	CheckResult(result == SOCKET_ERROR, L"发送消息失败!");
}
