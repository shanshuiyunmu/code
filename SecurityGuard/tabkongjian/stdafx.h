
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include <afxcmn.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#define WCHAR_TO_CHAR(lpw_Char,lpChar) WideCharToMultiByte(0,NULL,lpw_Char,-1,lpChar,_countof(lpChar),NULL,0);

//响应接收套接字发送过来的消息
#define UM_RECV_MSG WM_USER + 1

//发送给主窗口的消息
#define UM_SEND_MSG WM_USER + 2

#define UM_RECV_KILLVIRTUS WM_USER + 3

#define UM_RECV_COMMITVIRTUS WM_USER + 4


//枚举,消息类型,用于分发
enum MyEnum
{
	KILLVIRTUS = 0, COMMITVIRTUS = 1, 
};
typedef struct _DATAINFO
{
	WCHAR name[32];
	WCHAR md5[32];
}MYDATAINFO,*PDATAINFO;

//接收消息信息
typedef struct _RECVINFO
{
	MyEnum type;
	DWORD index;
	MYDATAINFO dateinfo;
	
}RECVINFO, *PRECVINFO;

//发送的信息
typedef struct _SENDINFO
{
	MyEnum type;
	DWORD index;
	MYDATAINFO dateinfo;
}SENDINFO, *PSENDINFO;