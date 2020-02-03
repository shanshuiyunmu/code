#pragma once
#include<windows.h>
class PE
{
public:
	PE();
	PE(WCHAR* szPath);
	~PE();
	PIMAGE_DOS_HEADER m_dos;
	CHAR * m_pBuff;
	WCHAR* m_szPath;
	BOOL IsPe();
	//获取nt头
	PIMAGE_NT_HEADERS32 GetNtHeader();
	//获取文件头 信息:平台,区段数量,拓展头大小
	IMAGE_FILE_HEADER GetFileHeader();
	void ShowFile();
	//获取拓展头 信息:入口点相对虚拟地址,内存对齐大小,文件对齐大小,//数据目录表的虚拟地址与大小.16个.
	IMAGE_OPTIONAL_HEADER GetOptionalHeader();
	void ShowOptional();
	//获得区段头. 信息:区段名,区段内存相对虚拟地址,区段内存大小,区段文件地址.区段文件大小,
	PIMAGE_SECTION_HEADER GetSectionHeader();
	void ShowSection();
	//rva 转foa
	DWORD RvaToFoa(DWORD rva);
	//得到导出表
	PIMAGE_EXPORT_DIRECTORY GetExport();
	//输出导出表的信息 信息:导出文件名,导出函数数量,导出有名的函数数量,rov函数地址.rov函数名称表,rov函数序号表
	void showExport();
	//得到导入表
	PIMAGE_IMPORT_DESCRIPTOR GetImport();
	//导入表信息: 信息:导入的dll文件名,INT表,IAT表,每个dll文件导出的函数的名字与序号,如果是序号导出就只输出序号.
	void ShowImport();
	//输出导入表信息
	//得到资源表
	PIMAGE_RESOURCE_DIRECTORY GetResource();
	//遍历资源表
	void ShowRes();
	//得到tls表
	PIMAGE_TLS_DIRECTORY GetTls();
	//显示tls信息
	void ShowTls();
	//获得重定位表
	PIMAGE_BASE_RELOCATION GetReloc();
	void ShowReloc();
	//获得延迟加载表
	PIMAGE_DELAYLOAD_DESCRIPTOR GetDelay();

	typedef struct myBlock
	{
		WORD offset : 12;
		WORD type : 4;
	}MYBLOCK, *PMYBLOCK;

	const WCHAR* RES[20] = {
	L"NULL",
	L"光标",
	L"位图",
	L"图标",
	L"菜单",
	L"对话框",
	L"字符串列表",
	L"字体目录",
	L"字体",
	L"加速键",
	L"非格式化资源",
	L"消息列表",
	L"光标组",
	L"NULL",
	L"图标组",
	L"NULL",
	L"版本信息",
	};
};
