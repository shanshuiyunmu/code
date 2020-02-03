// CMytab.cpp: 实现文件
//

#include "stdafx.h"
#include "tabkongjian.h"
#include "CMytab.h"


// CMytab

IMPLEMENT_DYNAMIC(CMytab, CTabCtrl)

CMytab::CMytab()
{

}

CMytab::~CMytab()
{
}

//封装的tab控件,添加列表项
void CMytab::InsertItemText(int count, ...)
{
	va_list valist;
	va_start(valist, count);
	for (int i = 0; i < count; ++i)
	{
		LPCWSTR Text = va_arg(valist, LPCWSTR);

		//添加列表项
		InsertItem(i, Text);
	}
	va_end(valist);
}

// //添加对话框 [对话框][id]
void CMytab::InsertDialogs(int count, ...)
{
	va_list valist;
	va_start(valist, count);

	//获得tab控件的用户区大小
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(2, 27, 4, 3);

	for(int i=0;i<count;i++)
	{
		CDialog* dlg = va_arg(valist, CDialog*);
		UINT id = va_arg(valist, UINT);
		dlg->Create(id,this);

		//移动对话框
		dlg->MoveWindow(&rect);
		myTabDialogs.push_back(dlg);
	}
	va_end(valist);
	ShowTabs(0);
}

//控制显示对话框控件
void CMytab::ShowTabs(int index)
{
	for (UINT i = 0; i < myTabDialogs.size(); i++)
	{
		if (i == index)
		{
			//只有选中的显示,其他对话框隐藏
			myTabDialogs[i]->ShowWindow(SW_NORMAL);
		}
		else
		{
			myTabDialogs[i]->ShowWindow(SW_HIDE);
		}
	}
}


BEGIN_MESSAGE_MAP(CMytab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMytab::OnTcnSelchange)
END_MESSAGE_MAP()



// CMytab 消息处理程序



//响应切换tab选项消息
void CMytab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 获得当前选中项
	int index = GetCurSel();

	ShowTabs(index);

	*pResult = 0;
}
