// CMyRes.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMyRes.h"
#include "afxdialogex.h"


// CMyRes 对话框

IMPLEMENT_DYNAMIC(CMyRes, CDialogEx)

CMyRes::CMyRes(WCHAR* fileName,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent),
	m_fileName(fileName),
	pe(m_fileName)
{

}

CMyRes::~CMyRes()
{
}

void CMyRes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_firstList);
	DDX_Control(pDX, IDC_LIST2, m_secondList);
	DDX_Control(pDX, IDC_LIST3, m_thirdList);
}


BEGIN_MESSAGE_MAP(CMyRes, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CMyRes::OnNMClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CMyRes::OnNMClickList2)
END_MESSAGE_MAP()


// CMyRes 消息处理程序


BOOL CMyRes::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_firstList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_firstList.InsertColumn(0, L"资源类型ID或名称", 0, 200);
	m_firstList.InsertColumn(1, L"资源表VA", 0, 200);
	m_secondList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_secondList.InsertColumn(0, L"资源名称或ID", 0, 200);
	m_secondList.InsertColumn(1, L"资源表VA", 0, 200);
	m_thirdList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_thirdList.InsertColumn(0, L"资源大小", 0, 200);
	m_thirdList.InsertColumn(1, L"资源RVA", 0, 200);
	m_thirdList.InsertColumn(2, L"资源FOA", 0, 200);

	auto pRes = pe.GetResource();
	auto pFirstRes = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes + 1);
	DWORD num = pRes->NumberOfIdEntries + pRes->NumberOfNamedEntries;
	CString StrTemp;
	for (DWORD i = 0; i < num; i++)
	{
		
		if (pFirstRes[i].NameIsString)
		{
			auto pName =
				(PIMAGE_RESOURCE_DIR_STRING_U)(pFirstRes[i].NameOffset + (DWORD)pRes);
			WCHAR buff[100];
			wcsncpy_s(buff, pName->NameString, pName->Length);
			StrTemp = buff;
			m_firstList.InsertItem(i, StrTemp);
		}
		else
		{
			if ((pFirstRes[i].Id >0 && pFirstRes[i].Id <= 13) || pFirstRes[i].Id== 16|| pFirstRes[i].Id == 14)
			{
				WCHAR* namefirst = (WCHAR*)pe.PE::RES[pFirstRes[i].Id];
				CString tempname(namefirst);
				m_firstList.InsertItem(i, tempname);	
			}
			else
			{
				StrTemp.Format(L"%04d\n", pFirstRes[i].Id);
				m_firstList.InsertItem(i, StrTemp);
			}

		}
		StrTemp.Format(L"%08X\n", &pFirstRes[i]);
		m_firstList.SetItemText(i, 1, StrTemp);
	}
	return TRUE;  
}

//左键点击firstLIst
void CMyRes::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
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
	DWORD FirstRes = strtol(he, nullptr, 16);
	auto pRes = pe.GetResource();
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pFirstRes = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)FirstRes;
	if (pFirstRes->DataIsDirectory)
	{
		auto pRessecond = (PIMAGE_RESOURCE_DIRECTORY)(pFirstRes->OffsetToDirectory + (DWORD)pRes);
		auto pSecondRes = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRessecond + 1);
		DWORD num2 = pRessecond->NumberOfIdEntries + pRessecond->NumberOfNamedEntries;
		for (DWORD i = 0; i < num2; i++)
		{
			
			if (pSecondRes[i].NameIsString)
			{
				auto pName =
					(PIMAGE_RESOURCE_DIR_STRING_U)(pSecondRes[i].NameOffset + (DWORD)pRes);
				WCHAR buff[100];
				wcsncpy_s(buff, pName->NameString, pName->Length);
				CString tempName(buff);
				m_secondList.InsertItem(i, tempName);
				
			}
			else
			{
				SrtImp.Format(L"%d", pSecondRes[i].Id);
				m_secondList.InsertItem(i, SrtImp);
			}
			SrtImp.Format(L"%08X", &pSecondRes[i]);
			m_secondList.SetItemText(i, 1, SrtImp);
		}
		
	}
	UpdateData(FALSE);
	*pResult = 0;
}

//secondList 点击
void CMyRes::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	m_thirdList.DeleteAllItems();
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int hang = pNMItemActivate->iItem;
	CString SrtImp = m_secondList.GetItemText(hang, 1);
	if (SrtImp.IsEmpty())
	{
		return;
	}
	char  he[9];
	WCHAR* name = SrtImp.GetBuffer();
	WCHAR_TO_CHAR(name, he)
	DWORD SecondRes = strtol(he, nullptr, 16);
	auto pRes = pe.GetResource();
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pSecondRes = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)SecondRes;
	if (pSecondRes->DataIsDirectory)
	{
		auto pResthird = (PIMAGE_RESOURCE_DIRECTORY)(pSecondRes->OffsetToDirectory + (DWORD)pRes);
		auto pThirdRes = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResthird + 1);
		DWORD num2 = pResthird->NumberOfIdEntries + pResthird->NumberOfNamedEntries;
		auto pResData = (PIMAGE_RESOURCE_DATA_ENTRY)(pThirdRes->OffsetToData + (DWORD)pRes);
		SrtImp.Format(L"%08X", pResData->Size);
		m_thirdList.InsertItem(0, SrtImp);
		SrtImp.Format(L"%08X", pResData->OffsetToData);
		m_thirdList.SetItemText(0,1, SrtImp);
		SrtImp.Format(L"%08X", pe.RvaToFoa(pResData->OffsetToData));
		m_thirdList.SetItemText(0,2, SrtImp);
	}
	*pResult = 0;
}
