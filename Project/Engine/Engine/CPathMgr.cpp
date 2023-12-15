#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr()
	: m_strContentPath{}
{

}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::init()
{
	// Content ���� ��θ� ã�Ƴ���.
	GetCurrentDirectory(256, m_strContentPath);

	// �θ� ���� ��θ� �˾Ƴ���( OutputFile ���� )
	int iLen = (int)wcslen(m_strContentPath);

	for (int i = iLen - 1; 0 <= i; --i)
	{
		if (L'\\' == m_strContentPath[i])
		{
			m_strContentPath[i] = L'\0';
			break;
		}
	}

	// content ���� ��θ� ���δ�.
	wcscat_s(m_strContentPath, L"\\bin\\content\\");
}

const wchar_t* CPathMgr::GetRelativePath(const wstring& _strFilePath)
{
	size_t iPos = _strFilePath.find(m_strContentPath, 0);
	iPos += wcsnlen_s(m_strContentPath, 256);

	wstring strRelativePath = _strFilePath.substr(iPos, _strFilePath.length());
	wcscpy_s(m_strRelativePath, strRelativePath.data());

	return m_strRelativePath;
}
