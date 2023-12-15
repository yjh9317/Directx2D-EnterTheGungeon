#include "pch.h"

void SaveStringToFile(const string& _str, FILE* _pFile)
{
    BYTE len = (BYTE)_str.length();
    fwrite(&len, sizeof(BYTE), 1, _pFile);
    fwrite(_str.c_str(), sizeof(char), len, _pFile);
}

void LoadStringFromFile(string& _str, FILE* _pFile)
{
    char szBuffer[256] = {};
    BYTE len = 0;
    fread(&len, sizeof(BYTE), 1, _pFile);
    fread(szBuffer, sizeof(char), (size_t)len, _pFile);

    _str = szBuffer;
}

void SaveWStringToFile(const wstring& _str, FILE* _pFile)
{
    BYTE len = (BYTE)_str.length();
    fwrite(&len, sizeof(BYTE), 1, _pFile);
    fwrite(_str.c_str(), sizeof(wchar_t), len, _pFile);
}

void LoadWStringFromFile(wstring& _str, FILE* _pFile)
{
    wchar_t szBuffer[256] = {};
    BYTE len = 0;
    fread(&len, sizeof(BYTE), 1, _pFile);
    fread(szBuffer, sizeof(wchar_t), (size_t)len, _pFile);

    _str = szBuffer;
}

const wchar_t* ToWString(COMPONENT_TYPE _type)
{
    static const wchar_t* szWString[(UINT)COMPONENT_TYPE::END] = 
    {
        L"TRANSFORM",
        L"CAMERA",
        L"COLLIDER2D",
        L"COLLIDER3D",
        L"ANIMATOR2D",
        L"ANIMATOR3D",
        L"LIGHT2D",
        L"LIGHT3D",
        L"BOUNDINGBOX",
        L"MESHRENDER",
        L"TILEMAP",
        L"PARTICLESYSTEM",
        L"LANDSCAPE",
        L"DECAL",
    };

    return szWString[(UINT)_type];
}

const char* ToString(COMPONENT_TYPE _type)
{
    static const char* szString[(UINT)COMPONENT_TYPE::END] =
    {
        "TRANSFORM",
        "CAMERA",
        "COLLIDER2D",
        "COLLIDER3D",
        "ANIMATOR2D",
        "ANIMATOR3D",
        "LIGHT2D",
        "LIGHT3D",
        "BOUNDINGBOX",
        "MESHRENDER",
        "TILEMAP",
        "PARTICLESYSTEM",
        "LANDSCAPE",
        "DECAL",
    };

    return szString[(UINT)_type];
}

const wchar_t* ToWString(RES_TYPE _type)
{
    static const wchar_t* szWString[(UINT)RES_TYPE::END] =
    {
        L"PREFAB",
        L"MESHDATA",
        L"MATERIAL",
        L"GRAPHICS_SHADER",
        L"COMPUTE_SHADER",
        L"MESH",
        L"TEXTURE",
        L"SOUND",
        L"SCENEFILE"
    };

    return szWString[(UINT)_type];
}

const char* ToString(RES_TYPE _type)
{
    static const char* szString[(UINT)RES_TYPE::END] =
    {
        "PREFAB",
        "MESHDATA",
        "MATERIAL",
        "GRAPHICS_SHADER",
        "COMPUTE_SHADER",
        "MESH",
        "TEXTURE",
        "SOUND",
        "SCENEFILE"
    };

    return szString[(UINT)_type];
}


float CalcRadianBetweenObjects(const Vec3& Origin, const Vec3& Dest)
{
    Vec2 Dir = Vec2(Dest.x - Origin.x, Dest.y - Origin.y);
    Dir.Normalize();
    return atan2f(Dir.y, Dir.x);
}


float CalcAngleBetweenObjects(const Vec3& Origin, const Vec3& Dest)
{
    return CalcRadianBetweenObjects(Origin,Dest) * (float)ONE_RADIAN;
}

float CalcDistBetweenObjects(const Vec3& FirstObject, const Vec3& SecondObject)
{
    return sqrtf(powf(FirstObject.x - SecondObject.x, 2) + powf(FirstObject.y - SecondObject.y, 2));
}

Vec2 GetDirBetweenObjects(const Vec3& Origin, const Vec3& Dest)
{
    Vec2 Dir = Vec2(Dest.x - Origin.x, Dest.y - Origin.y); Dir.Normalize();
	return Dir;
}

Vec2 GetDirBetweenObjects(const Vec2& Origin, const Vec2& Dest)
{
    Vec2 Dir = Vec2(Dest.x - Origin.x, Dest.y - Origin.y); Dir.Normalize();
    return Dir;
}

#include "CGameObject.h"
#include "CEventMgr.h"    

CGameObject* PrefabInstantiate(const wstring& wstr)
{
    Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(wstr);
    if (pPrefab == nullptr) return nullptr;
    return pPrefab->Instantiate();
}

void AddCreateObjectEvent(CGameObject* Object, int Layer)
{
    tEventInfo evninfo;
    evninfo.eType = EVENT_TYPE::CREATE_OBJ;
    evninfo.lParam = (DWORD_PTR)Object;
    evninfo.wParam = Layer;

    CEventMgr::GetInst()->AddEvent(evninfo);
}

void AddChildObjectEvent(CGameObject* Parent, CGameObject* Child)
{
    tEventInfo evninfo;
    evninfo.eType = EVENT_TYPE::ADD_CHILD;
    evninfo.lParam = (DWORD_PTR)Parent;
    evninfo.wParam = (DWORD_PTR)Child;

    CEventMgr::GetInst()->AddEvent(evninfo);
}


