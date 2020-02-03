
// peGuiDlg.h: 头文件
//

#pragma once
#include"PE.h"

// CpeGuiDlg 对话框
class CpeGuiDlg : public CDialogEx
{
// 构造
public:
	CpeGuiDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void IsPe();
	DECLARE_MESSAGE_MAP()
	WCHAR m_fileName[MAX_PATH];
public:
	CString m_pingtai;
	CString m_quduanNum;
	CString m_tuozhanSize;
	CString m_jiazaiBase;
	CString m_entryPoint;
	CString m_virtualAlign;
	CString m_fileAlign;
	CString m_subSystem;
	CString m_0Rva;
	CString m_1Rva;
	CString m_2Rva;
	CString m_3Rva;
	CString m_4Rva;
	CString m_5Rva;
	CString m_6Rva;
	CString m_7Rva;
	CString m_8Rva;
	CString m_9Rva;
	CString m_10Rva;
	CString m_11Rva;
	CString m_12Rva;
	CString m_13Rva;
	CString m_14Rva;
	CString m_15Rva;
	CString m_0Size;
	CString m_1Size;
	CString m_2Size;
	CString m_3Size;
	CString m_4Size;
	CString m_5Size;
	CString m_6Size;
	CString m_7Size;
	CString m_8Size;
	CString m_9Size;
	CString m_10Size;
	CString m_11Size;
	CString m_12Size;
	CString m_13Size;
	CString m_14Size;
	CString m_15Size;
	CListCtrl m_sectionList;
	CString m_virtualAddress;
	CString m_foa;
	CString m_createTime;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedLoadfile();
};
