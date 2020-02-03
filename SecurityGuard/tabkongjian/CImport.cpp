// CImport.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CImport.h"
#include "afxdialogex.h"
#include "PE.h"

// CImport 对话框

IMPLEMENT_DYNAMIC(CImport, CDialogEx)

CImport::CImport(WCHAR* fileName ,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent),
	 m_fileName(fileName),
	pe(m_fileName)
{
	
}

CImport::~CImport()
{
}

void CImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_dllList);
	DDX_Control(pDX, IDC_LIST2, m_funcLIst);
}


BEGIN_MESSAGE_MAP(CImport, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CImport::OnNMClickList1)
END_MESSAGE_MAP()


// CImport 消息处理程序


BOOL CImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(TRUE);
	m_dllList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_dllList.InsertColumn(0, L"DLL名称", 0, 300);
	m_dllList.InsertColumn(1, L"INT虚拟地址", 0, 100);
	m_dllList.InsertColumn(2, L"INT文件偏移", 0, 100);
	m_dllList.InsertColumn(3, L"IAT虚拟地址", 0, 100);
	m_dllList.InsertColumn(4, L"IAT文件偏移", 0, 100);
	m_dllList.InsertColumn(5, L"导入表地址", 0, 100);
	m_funcLIst.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_funcLIst.InsertColumn(0, L"thunkRVA", 0, 80);
	m_funcLIst.InsertColumn(1, L"thunkFOA", 0, 80);
	m_funcLIst.InsertColumn(2, L"thunk", 0, 80);
	m_funcLIst.InsertColumn(3, L"序号", 0, 80);
	m_funcLIst.InsertColumn(4, L"函数名称", 0, 300);
	
	PIMAGE_IMPORT_DESCRIPTOR pImp = pe.GetImport();
	int i = 0;
	while (pImp->Name)
	{	
		char* namedll = (char*)(pe.RvaToFoa(pImp->Name) + (DWORD)pe.m_pBuff);
		CString tempname(namedll);
		m_dllList.InsertItem(i, tempname);
		tempname.Format(L"%08X", pImp->OriginalFirstThunk);
		m_dllList.SetItemText(i, 1, tempname);
		PIMAGE_THUNK_DATA pINT = (PIMAGE_THUNK_DATA)(pe.RvaToFoa(pImp->OriginalFirstThunk));
		tempname.Format(L"%08X", pINT);
		m_dllList.SetItemText(i, 2, tempname);
		tempname.Format(L"%08X", pImp->OriginalFirstThunk);
		m_dllList.SetItemText(i, 3, tempname);
		PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)(pe.RvaToFoa(pImp->FirstThunk));
		tempname.Format(L"%08X", pIAT);
		m_dllList.SetItemText(i, 4, tempname);
		tempname.Format(L"%08X", pImp);
		m_dllList.SetItemText(i, 5, tempname);
		i++;
		pImp++;
	}
	UpdateData(FALSE);
	return TRUE;  
}


void CImport::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	m_funcLIst.DeleteAllItems();
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int hang = pNMItemActivate->iItem;
	CString SrtImp=m_dllList.GetItemText(hang,5);
	if (SrtImp.IsEmpty())
	{
		return;
	}
	char  he[9];
	WCHAR* name = SrtImp.GetBuffer();
	WCHAR_TO_CHAR(name, he)
	DWORD Imp = strtol(he, nullptr, 16);
	PIMAGE_IMPORT_DESCRIPTOR pImp = (PIMAGE_IMPORT_DESCRIPTOR)Imp;
	PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)(pe.RvaToFoa(pImp->FirstThunk) + (DWORD)pe.m_pBuff);
	DWORD pint = pImp->OriginalFirstThunk;
	int i = 0;
	while (pIAT->u1.Ordinal)
	{
		CString tempname;
		
		tempname.Format(L"%08X", pint);
		m_funcLIst.InsertItem(i, tempname);
		DWORD FOApint = pe.RvaToFoa(pint);
		tempname.Format(L"%08X", FOApint);
		m_funcLIst.SetItemText(i,1, tempname);
		tempname.Format(L"%08X", pIAT->u1.Ordinal);
		m_funcLIst.SetItemText(i, 2, tempname);
		if (pIAT->u1.Ordinal & 0x80000000)
		{
			//无名字
			tempname.Format(L"%2d", pIAT->u1.Ordinal & 0xFFFF);
			m_funcLIst.SetItemText(i,3, tempname);

		}
		else
		{
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(pe.RvaToFoa(pIAT->u1.AddressOfData) + (DWORD)pe.m_pBuff);
			tempname.Format(L"%2d", pName->Hint);
			m_funcLIst.SetItemText(i,3, tempname);
			CString name(pName->Name);
			m_funcLIst.SetItemText(i, 4, name);
		}
		i++;
		pint+=4;
		pIAT++;
	}
	UpdateData(FALSE);
	*pResult = 0;
}
