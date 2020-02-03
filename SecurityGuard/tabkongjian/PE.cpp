#include "stdafx.h"
#include "PE.h"
#include<iostream>

PE::PE()
{
}
PE::PE(WCHAR* szPath) :m_szPath(szPath)
{
	HANDLE hFile = CreateFileW(m_szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//计算文件大小
	DWORD SizeOfFile = GetFileSize(hFile, NULL);
	//保存文件的空间
	char * fileBuff = new char[SizeOfFile] {};
	DWORD realSize = {};
	ReadFile(hFile, fileBuff, SizeOfFile, &realSize, NULL);
	//
	m_pBuff = fileBuff;
	//
	m_dos = (PIMAGE_DOS_HEADER)(m_pBuff);
}

PE::~PE()
{
}


BOOL PE::IsPe()
{
	if (m_dos->e_magic != 0x5A4D)
	{
		return FALSE;
	}
	if (GetNtHeader()->Signature != 0x00004550)
	{
		return FALSE;
	}
	if (GetNtHeader()->OptionalHeader.Magic != 0x10B)
	{
		return FALSE;
	}
	return TRUE;
}

PIMAGE_NT_HEADERS32 PE::GetNtHeader()
{
	return (PIMAGE_NT_HEADERS32)(m_dos->e_lfanew + (DWORD)m_pBuff);
}

IMAGE_FILE_HEADER PE::GetFileHeader()
{
	return GetNtHeader()->FileHeader;
}

void PE::ShowFile()
{
	IMAGE_FILE_HEADER fileHeader = GetFileHeader();
	printf("平台信息:%08x\n", fileHeader.Machine);
	printf("区段数量:%08x\n", fileHeader.NumberOfSections);
	printf("拓展头大小:%08x\n", fileHeader.SizeOfOptionalHeader);
}

IMAGE_OPTIONAL_HEADER PE::GetOptionalHeader()
{
	return GetNtHeader()->OptionalHeader;
}

void PE::ShowOptional()
{
	IMAGE_OPTIONAL_HEADER optionalHeader = GetOptionalHeader();
	printf("加载基址:%08x\n", optionalHeader.ImageBase);
	printf("入口点地址:%08x\n", optionalHeader.AddressOfEntryPoint);
	printf("内存对齐大小:%08x\n", optionalHeader.SectionAlignment);
	printf("文件对齐大小:%08x\n", optionalHeader.FileAlignment);
	printf("子系统:%08x\n", optionalHeader.Subsystem);
	for (int i = 0; i < 16; i++)
	{
		printf("数据目录表地址: %d  %08x\n", i, optionalHeader.DataDirectory[i].VirtualAddress);
		printf("数据目录表大小:%08x\n", optionalHeader.DataDirectory[i].Size);
	}
}

PIMAGE_SECTION_HEADER PE::GetSectionHeader()
{
	return IMAGE_FIRST_SECTION(GetNtHeader());
}

void PE::ShowSection()
{
	PIMAGE_SECTION_HEADER sectionHeader = GetSectionHeader();
	for (DWORD i = 0; i < GetFileHeader().NumberOfSections; i++)
	{
		char sectionName[9] = { 0 };
		strncpy_s(sectionName, (char*)sectionHeader[i].Name, 8);
		printf("区段名: %s\n", sectionName);
		printf("区段内存地址: %08x\n", sectionHeader[i].VirtualAddress);
		printf("区段内存大小: %08x\n", sectionHeader[i].Misc.VirtualSize);
		printf("区段文件地址: %08x\n", sectionHeader[i].PointerToRawData);
		printf("区段文件大小: %08x\n", sectionHeader[i].SizeOfRawData);
	}

}

DWORD PE::RvaToFoa(DWORD rva)
{

	PIMAGE_SECTION_HEADER sectionHeader = GetSectionHeader();
	for (DWORD i = 0; i < GetFileHeader().NumberOfSections; i++)
	{
		if (rva >= sectionHeader[i].VirtualAddress
			&& rva < sectionHeader[i].VirtualAddress + sectionHeader[i].SizeOfRawData)
		{
			return rva - sectionHeader[i].VirtualAddress + sectionHeader[i].PointerToRawData;
		}
	}
	return 0;
}

PIMAGE_EXPORT_DIRECTORY PE::GetExport()
{
	IMAGE_OPTIONAL_HEADER pOpal = GetOptionalHeader();
	
	return PIMAGE_EXPORT_DIRECTORY(RvaToFoa(pOpal.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress) + (DWORD)m_pBuff);
}

void PE::showExport()
{
	PIMAGE_EXPORT_DIRECTORY pExport = GetExport();
	printf("%08x", pExport->Name);
	printf("%08x", pExport->Base);
	printf("%08x", pExport->AddressOfFunctions);
	printf("%08x", pExport->AddressOfNames);
	printf("%08x", pExport->AddressOfNameOrdinals);
	printf("%08x", pExport->NumberOfFunctions);
	printf("%08x", pExport->NumberOfNames);
	//得到地址表的实际值
	DWORD* paddre = (DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + (DWORD)m_pBuff);
	//名称表
	DWORD* pName = (DWORD*)(RvaToFoa(pExport->AddressOfNames) + (DWORD)m_pBuff);
	//序号表
	WORD* pOrder = (WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + (DWORD)m_pBuff);
	for (DWORD i = 0; i < pExport->NumberOfFunctions; i++)
	{
		BOOL isName = FALSE;
		//计算函数偏移
		DWORD pfunc = (RvaToFoa(paddre[i]));
		for (DWORD j = 0; j < pExport->NumberOfNames; j++)
		{
			if (i == pOrder[j])
			{
				printf("%d        ", i + pExport->Base);
				printf("%08x       ", paddre[i]);
				printf("%08x       ", pfunc);
				char* funcname = (char*)(RvaToFoa(pName[j]) + (DWORD)m_pBuff);
				printf("%s\n", funcname);
				isName = TRUE;
			}
		}
		if (isName == FALSE)
		{
			printf("%d       ", i + pExport->Base);
			printf("%08x       ", paddre[i]);
			printf("%08x     \n", pfunc);
		}
	}
}

PIMAGE_IMPORT_DESCRIPTOR PE::GetImport()
{
	IMAGE_OPTIONAL_HEADER pOpal = GetOptionalHeader();

	return PIMAGE_IMPORT_DESCRIPTOR(RvaToFoa(pOpal.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) + (DWORD)m_pBuff);
}

void PE::ShowImport()
{
	PIMAGE_IMPORT_DESCRIPTOR pImp = GetImport();
	while (pImp->Name)
	{
		char* namedll = (char*)(RvaToFoa(pImp->Name) + (DWORD)m_pBuff);
		printf("%s\n", namedll);

		printf("%08x\n", pImp->OriginalFirstThunk);
		printf("%08x\n", pImp->FirstThunk);
		PIMAGE_THUNK_DATA pINT = (PIMAGE_THUNK_DATA)(RvaToFoa(pImp->OriginalFirstThunk) + (DWORD)m_pBuff);
		PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)(RvaToFoa(pImp->FirstThunk) + (DWORD)m_pBuff);

		while (pIAT->u1.Ordinal)
		{
			if (pIAT->u1.Ordinal & 0x80000000)
			{
				//无名字
				printf("序号:%2d\n", pIAT->u1.Ordinal & 0xFFFF);
			}
			else
			{
				PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(RvaToFoa(pIAT->u1.AddressOfData) + (DWORD)m_pBuff);
				printf("序号%2d", pName->Hint);
				printf("    %s\n", pName->Name);
			}
			pIAT++;
		}
		pImp++;
	}


}

PIMAGE_RESOURCE_DIRECTORY PE::GetResource()
{
	auto poptional = GetOptionalHeader();
	return PIMAGE_RESOURCE_DIRECTORY(RvaToFoa(poptional.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress) + m_pBuff);
}


void PE::ShowRes()
{
	auto pRes = GetResource();
	auto pFirstRes = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes + 1);
	DWORD num = pRes->NumberOfIdEntries + pRes->NumberOfNamedEntries;
	for (DWORD i = 0; i < num; i++)
	{
		if (pFirstRes[i].NameIsString)
		{
			auto pName =
				(PIMAGE_RESOURCE_DIR_STRING_U)(pFirstRes[i].NameOffset + (DWORD)pRes);
			WCHAR buff[100];
			wcsncpy_s(buff, pName->NameString, pName->Length);
			wprintf(L"%s\n", buff);
		}
		else
		{
			if (pFirstRes[i].Id < 16)
			{
				wprintf(L"%s\n", RES[pFirstRes[i].Id]);
			}
			else
			{
				printf("%08x\n", pFirstRes->Id);
			}

		}
		if (pFirstRes[i].DataIsDirectory)
		{
			auto pRessecond = (PIMAGE_RESOURCE_DIRECTORY)(pFirstRes[i].OffsetToDirectory + (DWORD)pRes);
			auto pSecondRes = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRessecond + 1);
			DWORD num2 = pRessecond->NumberOfIdEntries + pRessecond->NumberOfNamedEntries;
			for (DWORD i = 0; i < num2; i++)
			{
				if (pSecondRes[i].NameIsString)
				{
					auto pName =
						(PIMAGE_RESOURCE_DIR_STRING_U)(pSecondRes[i].NameOffset + (DWORD)pRes);
					WCHAR buff[100];
					wcsncpy_s(buff, pName->NameString, pName->Length);
					wprintf(L"           %s", buff);
				}
				else
				{
					printf("           %08x", pSecondRes[i].Id);
				}
				if (pSecondRes[i].DataIsDirectory)
				{
					auto pResthird = (PIMAGE_RESOURCE_DIRECTORY)(pSecondRes[i].OffsetToDirectory + (DWORD)pRes);
					auto pThirdRes = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResthird + 1);
					DWORD num2 = pResthird->NumberOfIdEntries + pResthird->NumberOfNamedEntries;
					auto pResData = (PIMAGE_RESOURCE_DATA_ENTRY)(pThirdRes->OffsetToData + (DWORD)pRes);
					printf("           %08x", pResData->Size);
					printf("           RVA:%08x\n", pResData->OffsetToData);
				}
			}
		}
	}
}

PIMAGE_TLS_DIRECTORY PE::GetTls()
{
	auto poptional = GetOptionalHeader();
	return PIMAGE_TLS_DIRECTORY(RvaToFoa(poptional.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress) + (DWORD)m_pBuff);
}

void PE::ShowTls()
{
	auto pTls = GetTls();
	printf("起始:%08x\n", pTls->StartAddressOfRawData);
	printf("终止:%08x\n", pTls->EndAddressOfRawData);
	printf("索引:%08x\n", pTls->AddressOfIndex);
	printf("回调表:%08x\n", pTls->AddressOfCallBacks);
}
PIMAGE_BASE_RELOCATION PE::GetReloc()
{
	auto poptional = GetOptionalHeader();
	return PIMAGE_BASE_RELOCATION(RvaToFoa(poptional.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress) + m_pBuff);
}

void PE::ShowReloc()
{
	HANDLE hFile = CreateFileW(m_szPath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	auto pReloc = GetReloc();
	//
	while (pReloc->SizeOfBlock)
	{

		auto myblo = (PMYBLOCK)(pReloc + 1);
		DWORD num = (pReloc->SizeOfBlock - 8) / 2;
		for (DWORD i = 0; i < num; i++)
		{
			if (myblo->type == 3)
			{
				DWORD dwRva = myblo->offset + pReloc->VirtualAddress;
				DWORD dwfoa = RvaToFoa(dwRva);
				SetFilePointer(hFile, dwfoa, NULL, 0);
				DWORD writeread;
				DWORD zhi = 0;
				ReadFile(hFile, &zhi, 4, &writeread, NULL);
				DWORD name1 = zhi - GetNtHeader()->OptionalHeader.ImageBase + 0x00800000; //0x01500000
				SetFilePointer(hFile, dwfoa, NULL, 0);
				WriteFile(hFile, &name1, 4, &writeread, NULL);
			}
			myblo++;
		}
		pReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pReloc + pReloc->SizeOfBlock);
	}
	CloseHandle(hFile);
}

PIMAGE_DELAYLOAD_DESCRIPTOR PE:: GetDelay()
{
	auto poptional = GetOptionalHeader();
	return PIMAGE_DELAYLOAD_DESCRIPTOR(RvaToFoa(poptional.DataDirectory[13].VirtualAddress) + m_pBuff);
}