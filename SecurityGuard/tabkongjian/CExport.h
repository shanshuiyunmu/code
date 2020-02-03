#pragma once


// CExport 对话框

class CExport : public CDialogEx
{
	DECLARE_DYNAMIC(CExport)

public:
	CExport(WCHAR* fileName ,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CExport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	WCHAR*  m_fileName;
	CString m_Name;
	CString m_numberOfFunctions;
	CString m_numberOfNames;
	CString m_addressOfNames;
	CString m_addressOfFunctions;
	CString m_addressOfNameOrdinals;
	CListCtrl m_exportList;
	virtual BOOL OnInitDialog();
};
