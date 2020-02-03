// CMyList.cpp: 实现文件
//

#include "stdafx.h"
#include "process.h"
#include "CMyList.h"


// CMyList

IMPLEMENT_DYNAMIC(CMyList, CListCtrl)

CMyList::CMyList()
{

}

CMyList::~CMyList()
{
}

//封装列表控件的添加列操作
void CMyList::MyInsertColumn(int count, ...)
{
	//设置列表风格
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	va_list valist;
	va_start(valist, count);
	for (int i = 0; i < count; i++)
	{
		LPCWSTR Text = va_arg(valist, LPCWSTR);
		int align = va_arg(valist, int);
		int length = va_arg(valist, int);

		//插入列信息
		InsertColumn(i, Text, align, length);
	}
	va_end(valist);
}


BEGIN_MESSAGE_MAP(CMyList, CListCtrl)
END_MESSAGE_MAP()


