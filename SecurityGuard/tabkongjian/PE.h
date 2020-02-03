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
	//��ȡntͷ
	PIMAGE_NT_HEADERS32 GetNtHeader();
	//��ȡ�ļ�ͷ ��Ϣ:ƽ̨,��������,��չͷ��С
	IMAGE_FILE_HEADER GetFileHeader();
	void ShowFile();
	//��ȡ��չͷ ��Ϣ:��ڵ���������ַ,�ڴ�����С,�ļ������С,//����Ŀ¼��������ַ���С.16��.
	IMAGE_OPTIONAL_HEADER GetOptionalHeader();
	void ShowOptional();
	//�������ͷ. ��Ϣ:������,�����ڴ���������ַ,�����ڴ��С,�����ļ���ַ.�����ļ���С,
	PIMAGE_SECTION_HEADER GetSectionHeader();
	void ShowSection();
	//rva תfoa
	DWORD RvaToFoa(DWORD rva);
	//�õ�������
	PIMAGE_EXPORT_DIRECTORY GetExport();
	//������������Ϣ ��Ϣ:�����ļ���,������������,���������ĺ�������,rov������ַ.rov�������Ʊ�,rov������ű�
	void showExport();
	//�õ������
	PIMAGE_IMPORT_DESCRIPTOR GetImport();
	//�������Ϣ: ��Ϣ:�����dll�ļ���,INT��,IAT��,ÿ��dll�ļ������ĺ��������������,�������ŵ�����ֻ������.
	void ShowImport();
	//����������Ϣ
	//�õ���Դ��
	PIMAGE_RESOURCE_DIRECTORY GetResource();
	//������Դ��
	void ShowRes();
	//�õ�tls��
	PIMAGE_TLS_DIRECTORY GetTls();
	//��ʾtls��Ϣ
	void ShowTls();
	//����ض�λ��
	PIMAGE_BASE_RELOCATION GetReloc();
	void ShowReloc();
	//����ӳټ��ر�
	PIMAGE_DELAYLOAD_DESCRIPTOR GetDelay();

	typedef struct myBlock
	{
		WORD offset : 12;
		WORD type : 4;
	}MYBLOCK, *PMYBLOCK;

	const WCHAR* RES[20] = {
	L"NULL",
	L"���",
	L"λͼ",
	L"ͼ��",
	L"�˵�",
	L"�Ի���",
	L"�ַ����б�",
	L"����Ŀ¼",
	L"����",
	L"���ټ�",
	L"�Ǹ�ʽ����Դ",
	L"��Ϣ�б�",
	L"�����",
	L"NULL",
	L"ͼ����",
	L"NULL",
	L"�汾��Ϣ",
	};
};
