// CMyReloc.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyReloc.h"
#include "afxdialogex.h"


// CMyReloc 对话框

IMPLEMENT_DYNAMIC(CMyReloc, CDialogEx)

CMyReloc::CMyReloc(WCHAR* fileName,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent),
	m_fileName(fileName),
	pe(m_fileName)
{

}

CMyReloc::~CMyReloc()
{
}

void CMyReloc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_firstList);
	DDX_Control(pDX, IDC_LIST2, m_secondList);
}


BEGIN_MESSAGE_MAP(CMyReloc, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CMyReloc::OnNMClickList1)
END_MESSAGE_MAP()


// CMyReloc 消息处理程序


BOOL CMyReloc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_firstList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_firstList.InsertColumn(0, L"区段RVA", 0, 200);
	m_firstList.InsertColumn(1, L"重定位表VA", 0, 200);
	m_secondList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_secondList.InsertColumn(0, L"RVA", 0, 100);
	m_secondList.InsertColumn(1, L"FOA", 0, 100);
	m_secondList.InsertColumn(2, L"type", 0, 100);
	m_secondList.InsertColumn(3, L"FAR地址", 0, 100);
	CString Strtmp;
	auto pReloc = pe.GetReloc();
	int i = 0;
	while (pReloc->SizeOfBlock)
	{
		Strtmp.Format(L"%08X", pReloc->VirtualAddress);
		m_firstList.InsertItem(i, Strtmp);
		Strtmp.Format(L"%08X", pReloc);
		m_firstList.SetItemText(i,1, Strtmp);
		pReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pReloc + pReloc->SizeOfBlock);
		i++;
	}
	return TRUE; 
}


void CMyReloc::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	m_secondList.DeleteAllItems();
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int hang = pNMItemActivate->iItem;
	CString SrtImp = m_firstList.GetItemText(hang, 1);
	if (SrtImp.IsEmpty())
	{
		return;
	}
	char  he[9];
	WCHAR* name = SrtImp.GetBuffer();
	WCHAR_TO_CHAR(name, he)
	DWORD Reloc = strtol(he, nullptr, 16);
	auto pReloc= (PIMAGE_BASE_RELOCATION)Reloc;
	auto myblo = (PE::PMYBLOCK)(pReloc + 1);
	DWORD num = (pReloc->SizeOfBlock - 8) / 2;
	CString Strtemp;
	for (DWORD i = 0; i < num; i++)
	{
		DWORD dwRva = myblo->offset + pReloc->VirtualAddress;
		Strtemp.Format(L"%08X", dwRva);
		m_secondList.InsertItem(i,Strtemp);
		DWORD dwfoa = pe.RvaToFoa(dwRva);
		Strtemp.Format(L"%08X", dwfoa);
		m_secondList.SetItemText(i,1,Strtemp);
		Strtemp.Format(L"%d", myblo->type);
		m_secondList.SetItemText(i, 2, Strtemp);
		DWORD zhi = *(DWORD*)(dwfoa + (DWORD)pe.m_pBuff);
		Strtemp.Format(L"%08X", zhi);
		m_secondList.SetItemText(i, 3, Strtemp);
		myblo++;
	}
	UpdateData(FALSE);
	*pResult = 0;
}
