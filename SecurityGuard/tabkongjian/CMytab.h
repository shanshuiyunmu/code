#pragma once
#include<vector>
using namespace std;
// CMytab

class CMytab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMytab)

public:
	CMytab();
	virtual ~CMytab();

	//封装的tab控件,添加列表项
	void InsertItemText(int count,...);

	//添加对话框 [对话框][id]
	void InsertDialogs(int count, ...);

protected:
	DECLARE_MESSAGE_MAP()

	//控制显示对话框控件
	void ShowTabs(int index);

	//存放所有tab控件的对话框
	vector<CDialog*> myTabDialogs;
public:

	//响应切换tab选项消息
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


