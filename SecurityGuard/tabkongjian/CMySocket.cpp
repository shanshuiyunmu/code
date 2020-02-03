#include "stdafx.h"
#include "CMySocket.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>
#include <WS2tcpip.h>

//���캯��
CMySocket::CMySocket()
{
	WSADATA wsaData = { 0 };
	//��ʼ�����绷��
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	CheckResult(result != 0, L"������ʼ��ʧ��");

	CheckResult(wsaData.wVersion != MAKEWORD(2, 2), L"�汾����");
	//�����׽���
	m_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	CheckResult(m_client == SOCKET_ERROR, L"�׽��ִ���ʧ��");
}


CMySocket::~CMySocket()
{
	//�ر��׽���,������
	closesocket(m_client);
	WSACleanup();
}

//�̻߳ص�����,���ڽ�����Ϣ.
DWORD WINAPI ThreadProc(LPVOID lpThreadParameter)
{
	CMySocket* pMySocket = (CMySocket*)lpThreadParameter;
	CHAR* buffer = new CHAR[sizeof(RECVINFO)]{ 0 };
	//�����յ���Ϣ,�Ͱ���Ϣ���͸�������
	while (recv(pMySocket->m_client, buffer, sizeof(RECVINFO), 0)>0)
	{
		SendMessage(pMySocket->m_pMainWnd->GetSafeHwnd(), UM_RECV_MSG, NULL, (LPARAM)buffer);	
	}

	delete[] buffer;
	//���������ݴ���,���ǶϿ�������
	MessageBox(pMySocket->m_pMainWnd->GetSafeHwnd(), L"�����ж�", L"����", MB_OK);
	int* num = pMySocket->m_flagsocket;
	*num = 1;
	closesocket(pMySocket->m_client);
	pMySocket->m_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	pMySocket->CheckResult(pMySocket->m_client == SOCKET_ERROR, L"�׽��ִ���ʧ��");
	/*ExitProcess(-1);*/
	return 0;
}

//���ڼ�����
bool CMySocket::CheckResult(BOOL result, LPCWSTR errMsg)
{
	if (result == TRUE)
	{
		MessageBox(m_pMainWnd->GetSafeHwnd(), errMsg, L"����", MB_OK);
		return false;
		/*ExitProcess(-1);*/
	}
	return true;
}
//����������ָ��
void CMySocket::SetMainWnd(CDialogEx * pMainWnd)
{
	m_pMainWnd = pMainWnd;
}

//���ӷ�����
void CMySocket::ConnectServer(LPCSTR ip, WORD port,int &flag)
{
	//ʹ��TCP ,��ʼ��ip,��˿�
	m_flagsocket = &flag;
	sockaddr_in address = { 0 };
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr.S_un);
	//���Ӻ���
	int result=connect(m_client, (SOCKADDR*)&address, sizeof(sockaddr_in));
	if(CheckResult(result == SOCKET_ERROR, L"���ӷ�����ʧ��"))
	{
		flag = 2;
		MessageBox(m_pMainWnd->GetSafeHwnd(), L"���ӳɹ�", L"��ʾ", MB_OK);
		//�����������ݵ��߳�
		HANDLE hResult = CreateThread(NULL, 0, ThreadProc, (LPVOID)this, NULL, NULL);
		CheckResult(hResult == INVALID_HANDLE_VALUE, L"�̴߳���ʧ��");
	}
	else
	{
		flag = 1;
	}
	
}

//������Ϣ��������
void CMySocket::SendDataServer(LPVOID sendInfo,LPARAM size)
{
	int result = send(m_client, (PCHAR)sendInfo, size, 0);
	CheckResult(result == SOCKET_ERROR, L"������Ϣʧ��!");
}
