#pragma once


// CMyList

class CMyList : public CListCtrl
{
	DECLARE_DYNAMIC(CMyList)

public:
	CMyList();
	virtual ~CMyList();

	//封装列表控件的添加列操作
	void MyInsertColumn(int count, ...);
protected:
	DECLARE_MESSAGE_MAP()
};


