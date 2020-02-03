
// peGuiDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "peGuiDlg.h"
#include "afxdialogex.h"
#include "PE.h"
#include "CExport.h"
#include "CImport.h"
#include "CMyRes.h"
#include "CMyReloc.h"
#include "CMyTls.h"
#include "CMydelay.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpeGuiDlg 对话框



CpeGuiDlg::CpeGuiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PEGUI_DIALOG, pParent)
	, m_pingtai(_T(""))
	, m_quduanNum(_T(""))
	, m_tuozhanSize(_T(""))
	, m_jiazaiBase(_T(""))
	, m_entryPoint(_T(""))
	, m_virtualAlign(_T(""))
	, m_fileAlign(_T(""))
	, m_subSystem(_T(""))
	, m_0Rva(_T(""))
	, m_1Rva(_T(""))
	, m_2Rva(_T(""))
	, m_3Rva(_T(""))
	, m_4Rva(_T(""))
	, m_5Rva(_T(""))
	, m_6Rva(_T(""))
	, m_7Rva(_T(""))
	, m_8Rva(_T(""))
	, m_9Rva(_T(""))
	, m_10Rva(_T(""))
	, m_11Rva(_T(""))
	, m_12Rva(_T(""))
	, m_13Rva(_T(""))
	, m_14Rva(_T(""))
	, m_15Rva(_T(""))
	, m_0Size(_T(""))
	, m_1Size(_T(""))
	, m_2Size(_T(""))
	, m_3Size(_T(""))
	, m_4Size(_T(""))
	, m_5Size(_T(""))
	, m_6Size(_T(""))
	, m_7Size(_T(""))
	, m_8Size(_T(""))
	, m_9Size(_T(""))
	, m_10Size(_T(""))
	, m_11Size(_T(""))
	, m_12Size(_T(""))
	, m_13Size(_T(""))
	, m_14Size(_T(""))
	, m_15Size(_T(""))
	, m_virtualAddress(_T(""))
	, m_foa(_T(""))
	, m_createTime(_T(""))
	, m_fileName(_T(""))
{
	
}

void CpeGuiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_pingtai);
	DDX_Text(pDX, IDC_EDIT2, m_quduanNum);
	DDX_Text(pDX, IDC_EDIT3, m_tuozhanSize);
	DDX_Text(pDX, IDC_EDIT4, m_jiazaiBase);
	DDX_Text(pDX, IDC_EDIT5, m_entryPoint);
	DDX_Text(pDX, IDC_EDIT6, m_virtualAlign);
	DDX_Text(pDX, IDC_EDIT7, m_fileAlign);
	DDX_Text(pDX, IDC_EDIT8, m_subSystem);
	DDX_Text(pDX, IDC_EDIT9, m_0Rva);
	DDX_Text(pDX, IDC_EDIT10, m_1Rva);
	DDX_Text(pDX, IDC_EDIT11, m_2Rva);
	DDX_Text(pDX, IDC_EDIT12, m_3Rva);
	DDX_Text(pDX, IDC_EDIT13, m_4Rva);
	DDX_Text(pDX, IDC_EDIT14, m_5Rva);
	DDX_Text(pDX, IDC_EDIT15, m_6Rva);
	DDX_Text(pDX, IDC_EDIT16, m_7Rva);
	DDX_Text(pDX, IDC_EDIT17, m_8Rva);
	DDX_Text(pDX, IDC_EDIT18, m_9Rva);
	DDX_Text(pDX, IDC_EDIT19, m_10Rva);
	DDX_Text(pDX, IDC_EDIT20, m_11Rva);
	DDX_Text(pDX, IDC_EDIT21, m_12Rva);
	DDX_Text(pDX, IDC_EDIT22, m_13Rva);
	DDX_Text(pDX, IDC_EDIT23, m_14Rva);
	DDX_Text(pDX, IDC_EDIT24, m_15Rva);
	DDX_Text(pDX, IDC_EDIT25, m_0Size);
	DDX_Text(pDX, IDC_EDIT26, m_1Size);
	DDX_Text(pDX, IDC_EDIT27, m_2Size);
	DDX_Text(pDX, IDC_EDIT28, m_3Size);
	DDX_Text(pDX, IDC_EDIT29, m_4Size);
	DDX_Text(pDX, IDC_EDIT30, m_5Size);
	DDX_Text(pDX, IDC_EDIT31, m_6Size);
	DDX_Text(pDX, IDC_EDIT32, m_7Size);
	DDX_Text(pDX, IDC_EDIT33, m_8Size);
	DDX_Text(pDX, IDC_EDIT34, m_9Size);
	DDX_Text(pDX, IDC_EDIT35, m_10Size);
	DDX_Text(pDX, IDC_EDIT36, m_11Size);
	DDX_Text(pDX, IDC_EDIT37, m_12Size);
	DDX_Text(pDX, IDC_EDIT38, m_13Size);
	DDX_Text(pDX, IDC_EDIT39, m_14Size);
	DDX_Text(pDX, IDC_EDIT40, m_15Size);
	DDX_Control(pDX, IDC_LIST1, m_sectionList);
	DDX_Text(pDX, IDC_EDIT41, m_virtualAddress);
	DDX_Text(pDX, IDC_EDIT42, m_foa);
	DDX_Text(pDX, IDC_EDIT43, m_createTime);
}

BEGIN_MESSAGE_MAP(CpeGuiDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CpeGuiDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CpeGuiDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CpeGuiDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CpeGuiDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CpeGuiDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CpeGuiDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CpeGuiDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_LoadFile, &CpeGuiDlg::OnBnClickedLoadfile)
END_MESSAGE_MAP()


// CpeGuiDlg 消息处理程序

BOOL CpeGuiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sectionList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_sectionList.InsertColumn(0, L"区段名", 1, 80);
	m_sectionList.InsertColumn(1, L"区段内存地址", 1, 120);
	m_sectionList.InsertColumn(2, L"区段内存大小", 1, 120);
	m_sectionList.InsertColumn(3, L"区段文件地址", 1, 120);
	m_sectionList.InsertColumn(4, L"区段文件大小", 1, 120);
	return TRUE; 
}

void CpeGuiDlg::IsPe()
{
	PE pe(m_fileName);
	if (!pe.IsPe())
	{
		if (pe.GetNtHeader()->OptionalHeader.Magic == 0x20B)
		{
			MessageBox(L"不是32位PE文件!");
		}
		else
		{
			MessageBox(L"不是PE文件!");
		}
		
		return;
	}
	time_t nowTime;
	time(&nowTime);
	IMAGE_FILE_HEADER fileHeader = pe.GetFileHeader();
	struct tm systemTime;
	gmtime_s(&systemTime, (time_t *)&fileHeader.TimeDateStamp);
	m_createTime.Format(L"%d/%d/%d %d:%d:%d", systemTime.tm_year + 1900, systemTime.tm_mon + 1, systemTime.tm_mday, systemTime.tm_hour + 8, systemTime.tm_min, systemTime.tm_sec);
	m_pingtai.Format(L"%08X", fileHeader.Machine);
	m_quduanNum.Format(L"%08X", fileHeader.NumberOfSections);
	m_tuozhanSize.Format(L"%08X", fileHeader.SizeOfOptionalHeader);
	IMAGE_OPTIONAL_HEADER optionalHeader = pe.GetOptionalHeader();
	m_jiazaiBase.Format(L"%08X", optionalHeader.ImageBase);
	m_entryPoint.Format(L"%08X", optionalHeader.AddressOfEntryPoint);
	m_virtualAlign.Format(L"%08X", optionalHeader.SectionAlignment);
	m_fileAlign.Format(L"%08X", optionalHeader.FileAlignment);
	m_subSystem.Format(L"%08X", optionalHeader.Subsystem);


	m_0Rva.Format(L"%08X", optionalHeader.DataDirectory[0].VirtualAddress);
	m_0Size.Format(L"%08X", optionalHeader.DataDirectory[0].Size);
	m_1Rva.Format(L"%08X", optionalHeader.DataDirectory[1].VirtualAddress);
	m_1Size.Format(L"%08X", optionalHeader.DataDirectory[1].Size);
	m_2Rva.Format(L"%08X", optionalHeader.DataDirectory[2].VirtualAddress);
	m_2Size.Format(L"%08X", optionalHeader.DataDirectory[2].Size);
	m_3Rva.Format(L"%08X", optionalHeader.DataDirectory[3].VirtualAddress);
	m_3Size.Format(L"%08X", optionalHeader.DataDirectory[3].Size);
	m_4Rva.Format(L"%08X", optionalHeader.DataDirectory[4].VirtualAddress);
	m_4Size.Format(L"%08X", optionalHeader.DataDirectory[4].Size);
	m_5Rva.Format(L"%08X", optionalHeader.DataDirectory[5].VirtualAddress);
	m_5Size.Format(L"%08X", optionalHeader.DataDirectory[5].Size);
	m_6Rva.Format(L"%08X", optionalHeader.DataDirectory[6].VirtualAddress);
	m_6Size.Format(L"%08X", optionalHeader.DataDirectory[6].Size);
	m_7Rva.Format(L"%08X", optionalHeader.DataDirectory[7].VirtualAddress);
	m_7Size.Format(L"%08X", optionalHeader.DataDirectory[7].Size);
	m_8Rva.Format(L"%08X", optionalHeader.DataDirectory[8].VirtualAddress);
	m_8Size.Format(L"%08X", optionalHeader.DataDirectory[8].Size);
	m_9Rva.Format(L"%08X", optionalHeader.DataDirectory[9].VirtualAddress);
	m_9Size.Format(L"%08X", optionalHeader.DataDirectory[9].Size);
	m_10Rva.Format(L"%08X", optionalHeader.DataDirectory[10].VirtualAddress);
	m_10Size.Format(L"%08X", optionalHeader.DataDirectory[10].Size);
	m_11Rva.Format(L"%08X", optionalHeader.DataDirectory[11].VirtualAddress);
	m_11Size.Format(L"%08X", optionalHeader.DataDirectory[11].Size);
	m_12Rva.Format(L"%08X", optionalHeader.DataDirectory[12].VirtualAddress);
	m_12Size.Format(L"%08X", optionalHeader.DataDirectory[12].Size);
	m_13Rva.Format(L"%08X", optionalHeader.DataDirectory[13].VirtualAddress);
	m_13Size.Format(L"%08X", optionalHeader.DataDirectory[13].Size);
	m_14Rva.Format(L"%08X", optionalHeader.DataDirectory[14].VirtualAddress);
	m_14Size.Format(L"%08X", optionalHeader.DataDirectory[14].Size);
	m_15Rva.Format(L"%08X", optionalHeader.DataDirectory[15].VirtualAddress);
	m_15Size.Format(L"%08X", optionalHeader.DataDirectory[15].Size);
	m_sectionList.DeleteAllItems();
	for (DWORD i = 0; i < fileHeader.NumberOfSections; i++)
	{
		PIMAGE_SECTION_HEADER sectionHeader = pe.GetSectionHeader();
		char sectionName[9] = { 0 };
		strncpy_s(sectionName, (char*)sectionHeader[i].Name, 8);
		CString tempname(sectionName);
		m_sectionList.InsertItem(i, tempname);
		tempname.Format(L"%08X", sectionHeader[i].VirtualAddress);
		m_sectionList.SetItemText(i, 1, tempname);
		tempname.Format(L"%08X", sectionHeader[i].Misc.VirtualSize);
		m_sectionList.SetItemText(i, 2, tempname);
		tempname.Format(L"%08X", sectionHeader[i].PointerToRawData);
		m_sectionList.SetItemText(i, 3, tempname);
		tempname.Format(L"%08X", sectionHeader[i].SizeOfRawData);
		m_sectionList.SetItemText(i, 4, tempname);
	}
	UpdateData(FALSE);
}


void CpeGuiDlg::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);
	WCHAR FileName[MAX_PATH] = { 0 };
	//根据循环获取的下标获取文件的名称
	DragQueryFile(hDropInfo, 0, FileName, MAX_PATH);
	wcscpy_s(m_fileName,FileName);
	IsPe();
	CDialogEx::OnDropFiles(hDropInfo);
}

//rva转foa
void CpeGuiDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_virtualAddress.IsEmpty())
	{
		MessageBox(L"rva不能为空");
		return;
	}	
	char  he[9];
	WCHAR* name = m_virtualAddress.GetBuffer();
	WCHAR_TO_CHAR(name, he)
	DWORD rva=strtol(he, nullptr, 16);
	PE pe(m_fileName);
	DWORD foa = pe.RvaToFoa(rva);
	m_foa.Format(L"%08X", foa);
	UpdateData(FALSE);
}


//导出表
void CpeGuiDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);
	if (m_0Rva.IsEmpty())
	{
		MessageBox(L"请先拖入PE文件!");
		return;
	}
	if (!wcscmp(m_0Rva, L"00000000"))
	{
		MessageBox(L"此文件没有导出表!");
		return;
	}
	CExport myExport(m_fileName);
	
	myExport.DoModal();
	UpdateData(FALSE);
}

//导入表
void CpeGuiDlg::OnBnClickedButton3()
{
	UpdateData(TRUE);
	if (m_1Rva.IsEmpty())
	{
		MessageBox(L"请先拖入PE文件!");
		return;
	}
	if (!wcscmp(m_1Rva, L"00000000"))
	{
		MessageBox(L"此文件没有导入表!");
		return;
	}
	CImport myImport(m_fileName);

	myImport.DoModal();
	UpdateData(FALSE);
}



//资源表
void CpeGuiDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);
	if (m_2Rva.IsEmpty())
	{
		MessageBox(L"请先拖入PE文件!");
		return;
	}
	if (!wcscmp(m_2Rva, L"00000000"))
	{
		MessageBox(L"此文件没有资源表!");
		return;
	}
	CMyRes myRes(m_fileName);

	myRes.DoModal();
	UpdateData(FALSE);
}

//重定位表
void CpeGuiDlg::OnBnClickedButton6()
{
	UpdateData(TRUE);
	if (m_5Rva.IsEmpty())
	{
		MessageBox(L"请先拖入PE文件!");
		return;
	}
	if (!wcscmp(m_5Rva, L"00000000"))
	{
		MessageBox(L"此文件没有重定位表!");
		return;
	}
	CMyReloc myReloc(m_fileName);

	myReloc.DoModal();
	UpdateData(FALSE);
}

//TLS表
void CpeGuiDlg::OnBnClickedButton7()
{
	UpdateData(TRUE);
	if (m_9Rva.IsEmpty())
	{
		MessageBox(L"请先拖入PE文件!");
		return;
	}
	if (!wcscmp(m_9Rva, L"00000000"))
	{
		MessageBox(L"此文件没有重定位表!");
		return;
	}
	CMyTls myTls(m_fileName);

	myTls.DoModal();
	UpdateData(FALSE);
}

//延迟加载表
void CpeGuiDlg::OnBnClickedButton8()
{
	UpdateData(TRUE);
	if (m_13Rva.IsEmpty())
	{
		MessageBox(L"请先拖入PE文件!");
		return;
	}
	if (!wcscmp(m_13Rva, L"00000000"))
	{
		MessageBox(L"此文件没有延迟加载表!");
		return;
	}
	CMydelay mydelay(m_fileName);

	mydelay.DoModal();
	UpdateData(FALSE);
}

//加载文件路径
void CpeGuiDlg::OnBnClickedLoadfile()
{
	CFileDialog filedlg(TRUE);
	filedlg.DoModal();
	WCHAR FileName[MAX_PATH] = { 0 };
	CString filename(filedlg.GetPathName());
	wcscpy_s(m_fileName, filename.GetBuffer());
	if (filename.IsEmpty())
	{
		return;
	}
	else
	{
		IsPe();
	}
	
}
