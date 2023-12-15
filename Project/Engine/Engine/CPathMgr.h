#pragma once

class CPathMgr
	: public CSingleton<CPathMgr>
{
	SINGLE(CPathMgr);
private:
	wchar_t m_strContentPath[256];
	wchar_t m_strRelativePath[256];

public:
	void init();


public:
	const wchar_t* GetContentPath() { return m_strContentPath; }
	const wchar_t* GetRelativePath(const wstring& _strFilePath);

};

