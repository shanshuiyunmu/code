#pragma once


// CMyMemory 对话框

class CMyMemory : public CDialogEx
{
	DECLARE_DYNAMIC(CMyMemory)

public:
	CMyMemory(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyMemory();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Memory};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//时间转换
	double FILETIME2Double(const FILETIME & fileTime);

	//得到cpu的使用情况
	INT GetCpuUsage();

	DECLARE_MESSAGE_MAP()
public:

	//edit控件 显示cpu
	CString m_strcpu;

	//edit控件 显示内存
	CString m_memorystr;

	virtual BOOL OnInitDialog();

	//得到内存的信息
	MEMORYSTATUS GetMemState();

	//释放内存
	afx_msg void OnBnClickedreleasemem();

	//刷新显示
	afx_msg void OnBnClickedflushmem();
};
