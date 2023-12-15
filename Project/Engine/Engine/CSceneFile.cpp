#include "pch.h"
#include "CSceneFile.h"

CSceneFile::CSceneFile()
	: CRes(RES_TYPE::SCENEFILE)
{
}

CSceneFile::~CSceneFile()
{
}


int CSceneFile::Load(const wstring& _strFilePath)
{	
	return S_OK;
}