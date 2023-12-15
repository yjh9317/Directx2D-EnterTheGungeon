#pragma once

class CTexture;

#include "ListUI.h"

class ParamUI
{
public:
	static int KeyCount;

public:
	static int Param_Int(const string& _strName, const int* _pInOut);
	static float Param_Float(const string& _strName, const float* _pInOut);
	static Vec2 Param_Vec2(const string& _strName, const Vec2* _pInOut);
	static Vec4 Param_Vec4(const string& _strName, const Vec4* _pInOut);

	// ��ȯ�� : ����ƮUI Ȱ��ȭ ���� �� true �ƴϸ� false
	static bool Param_Tex(const string& _strName, CTexture* _pCurTex, UI* _pInst = nullptr, DBCLKED _pFunc = nullptr);
};

