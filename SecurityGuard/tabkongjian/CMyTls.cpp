// CMyTls.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyTls.h"
#include "afxdialogex.h"


// CMyTls 对话框

IMPLEMENT_DYNAMIC(CMyTls, CDialogEx)

CMyTls::CMyTls(WCHAR* fileName, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent),
	m_fileName(fileName),
	pe(m_fileName)
	, m_start(_T(""))
	, m_end(_T(""))
	, m_index(_T(""))
	, m_callback(_T(""))
{

}

CMyTls::~CMyTls()
{
}

void CMyTls::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_start);
	DDX_Text(pDX, IDC_EDIT2, m_end);
	DDX_Text(pDX, IDC_EDIT3, m_index);
	DDX_Text(pDX, IDC_EDIT4, m_callback);
}


BEGIN_MESSAGE_MAP(CMyTls, CDialogEx)
END_MESSAGE_MAP()


// CMyTls 消息处理程序


BOOL CMyTls::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	auto pTls = pe.GetTls();
	UpdateData(TRUE);
	m_start.Format(L"%08X", pTls->StartAddressOfRawData);
	m_end.Format(L"%08X", pTls->EndAddressOfRawData);
	m_index.Format(L"%08X", pTls->AddressOfIndex);
	m_callback.Format(L"%08X", pTls->AddressOfCallBacks);
	UpdateData(FALSE);
	return TRUE;  
}
