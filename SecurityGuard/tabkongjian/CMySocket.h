#pragma once


class CMySocket
{
public:
	CMySocket();
	~CMySocket();

	//���ڼ�����
	bool CheckResult(BOOL result, LPCWSTR errMsg);

	//����������ָ��
	void SetMainWnd(CDialogEx* pMainWnd);

	//���ӷ�����
	void ConnectServer(LPCSTR ip, WORD port, int &flag);

	//������Ϣ��������
	void SendDataServer(LPVOID sendInfo, LPARAM size);
	
	int *m_flagsocket;
public:

	//������ָ�����
	CDialogEx* m_pMainWnd;

	//�׽��ֱ���
	SOCKET m_client;
};

