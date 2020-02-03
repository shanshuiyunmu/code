// saolei.cpp: 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "saolei.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CsaoleiApp

BEGIN_MESSAGE_MAP(CsaoleiApp, CWinApp)
END_MESSAGE_MAP()


// CsaoleiApp 构造

CsaoleiApp::CsaoleiApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CsaoleiApp 对象

CsaoleiApp theApp;
PDWORD g_pHeight = (PDWORD)0x1005338;
PDWORD g_pWidth = (PDWORD)0x1005334;
PDWORD g_pMineCount = (PDWORD)0x1005330;
HWND g_Wnd;
WNDPROC g_OldProc;
//雷区的地址
//雷区中的元素标识
PBYTE g_dwBase = (PBYTE)0x1005340;
#define MINE 0X8F
LRESULT WINAPI WindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	if (Msg == WM_KEYDOWN && wParam == VK_F5)
	{
		//一键秒杀
		OutputDebugString(L"F5\n");

		int nHeight = *g_pHeight;
		int nWidth = *g_pWidth;
		int nMineCount = *g_pMineCount;
		CString strString;
		strString.Format(L"高:%d,宽:%d,雷数:%d\n", nHeight, nWidth, nMineCount);
		OutputDebugString(strString.GetBuffer());
		int nFindCount=0;
		
		for (size_t y = 1; y < nHeight + 1; y++)
		{
			CString strLine;
			for (size_t x = 1; x < nWidth + 1; x++)
			{
				byte byCode = *(PBYTE)((DWORD)g_dwBase + x + (y * 32));
				if (byCode == MINE)
				{
					nFindCount++;
				}
				else
				{
					int xPos, yPos;
					xPos = (x << 4) - 4;
					yPos = (y << 4) + 0x27;
					SendMessage(hWnd, WM_LBUTTONDOWN, 0, MAKELPARAM(xPos, yPos));
					SendMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(xPos, yPos));

				}
				CString strCode;
				strCode.Format(L"%02x", byCode);
				strLine += strCode;
			}
			OutputDebugString(strLine.GetBuffer());
		}
	}
	else if (Msg == WM_MOUSEMOVE)
	{
		//鼠标移动
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		x = (x + 4) >> 4;
		y = (y - 0x27) >> 4;	
		byte byCode = *(PBYTE)((DWORD)g_dwBase + x + y * 32);
		if (byCode == MINE)
		{
			CString strCode;
			strCode.Format(L"%02x,(%d,%d)", byCode,x,y);
			SetWindowText(hWnd, strCode);
		}
		else
		{
			CString strCode;
			strCode.Format(L"%02x,(%d,%d)", byCode, x, y);
			SetWindowText(hWnd, strCode);
		}
	}
	return CallWindowProc(g_OldProc, hWnd, Msg, wParam, lParam);
}
// CsaoleiApp 初始化

BOOL CsaoleiApp::InitInstance()
{
	CWinApp::InitInstance();

	//代码
	//查找窗口,获得窗口句柄
	g_Wnd=::FindWindow(L"扫雷", L"扫雷");
	if (g_Wnd==NULL)
	{
		OutputDebugString(L"没有找到程序窗口");
		return FALSE;
	}
	//设置窗口回调函数
	g_OldProc=(WNDPROC)SetWindowLong(g_Wnd, GWL_WNDPROC, (LONG)WindowProc);
	if (NULL == g_OldProc)
	{
		OutputDebugString(L"回调函数设置失败");
		return FALSE;
	}
	return TRUE;
}
