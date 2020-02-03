// CExport.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CExport.h"
#include "afxdialogex.h"
#include "PE.h"

// CExport 对话框

IMPLEMENT_DYNAMIC(CExport, CDialogEx)

CExport::CExport(WCHAR* fileName ,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_Name(_T(""))
	, m_numberOfFunctions(_T(""))
	, m_numberOfNames(_T(""))
	, m_addressOfNames(_T(""))
	, m_addressOfFunctions(_T(""))
	, m_addressOfNameOrdinals(_T(""))
	,m_fileName(fileName)
{

}

CExport::~CExport()
{
}

void CExport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT2, m_numberOfFunctions);
	DDX_Text(pDX, IDC_EDIT3, m_numberOfNames);
	DDX_Text(pDX, IDC_EDIT4, m_addressOfNames);
	DDX_Text(pDX, IDC_EDIT6, m_addressOfFunctions);
	DDX_Text(pDX, IDC_EDIT5, m_addressOfNameOrdinals);
	DDX_Control(pDX, IDC_LIST1, m_exportList);
}


BEGIN_MESSAGE_MAP(CExport, CDialogEx)
END_MESSAGE_MAP()


// CExport 消息处理程序


BOOL CExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);
	PE pe(m_fileName);
	//得到输出表
	PIMAGE_EXPORT_DIRECTORY pExport = pe.GetExport();
	char* dllname = (char*)(pe.RvaToFoa(pExport->Name) + (DWORD)pe.m_pBuff);
	CString tempfunname(dllname);
	m_Name=tempfunname;
	m_addressOfFunctions.Format(L"%08X",pExport->AddressOfFunctions);
	m_addressOfNames.Format( L"%08X", pExport->AddressOfNames);
	m_addressOfNameOrdinals.Format(L"%08X", pExport->AddressOfNameOrdinals);
	m_numberOfFunctions.Format( L"%08X", pExport->NumberOfFunctions);
	m_numberOfNames.Format(L"%08X", pExport->NumberOfNames);
	//得到地址表的实际值
	DWORD* paddre = (DWORD*)(pe.RvaToFoa(pExport->AddressOfFunctions) + (DWORD)pe.m_pBuff);
	//名称表
	DWORD* pName = (DWORD*)(pe.RvaToFoa(pExport->AddressOfNames) + (DWORD)pe.m_pBuff);
	//序号表
	WORD* pOrder = (WORD*)(pe.RvaToFoa(pExport->AddressOfNameOrdinals) + (DWORD)pe.m_pBuff);
	m_exportList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_exportList.InsertColumn(0, L"序号", 0, 80);
	m_exportList.InsertColumn(1, L"函数地址RVA", 0, 100);
	m_exportList.InsertColumn(2, L"函数地址偏移", 0, 120);
	m_exportList.InsertColumn(3, L"函数名", 0, 200);
	m_exportList.DeleteAllItems();
	for (DWORD i = 0; i < pExport->NumberOfFunctions; i++)
	{
		BOOL isName = FALSE;
		CString tempname;
		//计算函数偏移
		DWORD* pfunc = (DWORD*)(pe.RvaToFoa(paddre[i]));
		for (DWORD j = 0; j < pExport->NumberOfNames; j++)
		{
			if (i == pOrder[j])
			{
				tempname.Format(L"%d", i + pExport->Base);
				m_exportList.InsertItem(i, tempname);
				tempname.Format(L"%08X", paddre[i]);
				m_exportList.SetItemText(i, 1, tempname);
				tempname.Format(L"%08X", pfunc);
				m_exportList.SetItemText(i, 2, tempname);
				char* funcname = (char*)(pe.RvaToFoa(pName[j]) + (DWORD)pe.m_pBuff);
				CString tempfunname(funcname);
				m_exportList.SetItemText(i, 3, tempfunname);
				isName = TRUE;
				break;
			}
		}
		if (isName == FALSE)
		{
			tempname.Format(L"%d", i + pExport->Base);
			m_exportList.InsertItem(i, tempname);
			tempname.Format(L"%08X", paddre[i]);
			m_exportList.SetItemText(i, 1, tempname);
			tempname.Format(L"%08X", pfunc);
			m_exportList.SetItemText(i, 2, tempname);
		}
	}
	UpdateData(FALSE);
	return TRUE;
}


