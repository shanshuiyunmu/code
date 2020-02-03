// CMydelay.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMydelay.h"
#include "afxdialogex.h"


// CMydelay 对话框

IMPLEMENT_DYNAMIC(CMydelay, CDialogEx)

CMydelay::CMydelay(WCHAR* fileName, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent),
	m_fileName(fileName),
	pe(m_fileName)
{

}

CMydelay::~CMydelay()
{
}

void CMydelay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_delayList);
	DDX_Control(pDX, IDC_LIST2, m_secondList);
}


BEGIN_MESSAGE_MAP(CMydelay, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CMydelay::OnNMClickList1)
END_MESSAGE_MAP()


// CMydelay 消息处理程序


BOOL CMydelay::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);
	m_delayList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_delayList.InsertColumn(0, L"DLL名称", 0, 100);
	m_delayList.InsertColumn(1, L"IAT虚拟地址", 0, 100);
	m_delayList.InsertColumn(2, L"IAT文件偏移", 0, 100);
	m_delayList.InsertColumn(3, L"INT虚拟地址", 0, 100);
	m_delayList.InsertColumn(4, L"INT文件偏移", 0, 100);
	m_delayList.InsertColumn(5, L"延迟表地址", 0, 100);
	m_secondList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_secondList.InsertColumn(0, L"thunkRVA", 0, 80);
	m_secondList.InsertColumn(1, L"thunkFOA", 0, 80);
	m_secondList.InsertColumn(2, L"thunk", 0, 80);
	m_secondList.InsertColumn(3, L"序号", 0, 80);
	m_secondList.InsertColumn(4, L"函数名称", 0, 300);

	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = pe.GetDelay();
	int i = 0;
	while (pDelay->DllNameRVA)
	{
		char* funcname = (char*)(pe.RvaToFoa(pDelay->DllNameRVA) + (DWORD)pe.m_pBuff);
		CString tempname(funcname);
		m_delayList.InsertItem(i, tempname);
		tempname.Format(L"%08X", pDelay->ImportAddressTableRVA);
		m_delayList.SetItemText(i, 1, tempname);
		PIMAGE_THUNK_DATA pINT = (PIMAGE_THUNK_DATA)(pe.RvaToFoa(pDelay->ImportAddressTableRVA));
		tempname.Format(L"%08X", pINT);
		m_delayList.SetItemText(i, 2, tempname);
		tempname.Format(L"%08X", pDelay->ImportNameTableRVA);
		m_delayList.SetItemText(i, 3, tempname);
		PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)(pe.RvaToFoa(pDelay->ImportNameTableRVA));
		tempname.Format(L"%08X", pIAT);
		m_delayList.SetItemText(i, 4, tempname);
		tempname.Format(L"%08X", pDelay);
		m_delayList.SetItemText(i, 5, tempname);
		i++;
		pDelay++;
	}
	UpdateData(FALSE);
	return TRUE;  
}


void CMydelay::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	m_secondList.DeleteAllItems();
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int hang = pNMItemActivate->iItem;
	CString SrtImp = m_delayList.GetItemText(hang, 5);
	if (SrtImp.IsEmpty())
	{
		return;
	}
	char  he[9];
	WCHAR* name = SrtImp.GetBuffer();
	WCHAR_TO_CHAR(name, he)
		DWORD Delay = strtol(he, nullptr, 16);
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = (PIMAGE_DELAYLOAD_DESCRIPTOR)Delay;
	PIMAGE_THUNK_DATA pINT = (PIMAGE_THUNK_DATA)(pe.RvaToFoa(pDelay->ImportNameTableRVA) + (DWORD)pe.m_pBuff);
	DWORD pint = pDelay->ImportNameTableRVA;
	int i = 0;
	while (pINT->u1.Ordinal)
	{
		CString tempname;

		tempname.Format(L"%08X", pint);
		m_secondList.InsertItem(i, tempname);
		DWORD FOApint = pe.RvaToFoa(pint);
		tempname.Format(L"%08X", FOApint);
		m_secondList.SetItemText(i, 1, tempname);
		tempname.Format(L"%08X", pINT->u1.Ordinal);
		m_secondList.SetItemText(i, 2, tempname);
		if (pINT->u1.Ordinal & 0x80000000)
		{
			//无名字
			tempname.Format(L"%04X", pINT->u1.Ordinal & 0xFFFF);
			m_secondList.SetItemText(i, 3, tempname);

		}
		else
		{
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(pe.RvaToFoa(pINT->u1.AddressOfData) + (DWORD)pe.m_pBuff);
			tempname.Format(L"%04X", pName->Hint);
			m_secondList.SetItemText(i, 3, tempname);
			CString name(pName->Name);
			m_secondList.SetItemText(i, 4, name);
		}
		i++;
		pint += 4;
		pINT++;
	}
	UpdateData(FALSE);
	*pResult = 0;
}
