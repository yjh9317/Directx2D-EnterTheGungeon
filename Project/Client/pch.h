﻿// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#include <commdlg.h>
#include <crtdbg.h>

// 엔진 라이브러리
#include <Engine/global.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine/Engine_debug.lib")
#else
#pragma comment(lib, "Engine/Engine.lib")
#endif

// 스크립트 라이브러리
#ifdef _DEBUG
#pragma comment(lib, "Script/Script_debug.lib")
#else
#pragma comment(lib, "Script/Script.lib")
#endif

// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;

using std::pair;
using std::make_pair;


using std::to_string;
using std::to_wstring;
#endif //PCH_H
