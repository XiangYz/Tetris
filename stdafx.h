
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars



////////////global var////////////////////
extern int g_Speed;
//extern const int MAX_Y;
//extern const int MAX_X;
const int MAX_Y = 20;
const int MAX_X = 12;
//extern const INT g_ShapeState[7];
//extern const COLORREF g_ShapeColor[6];
const INT g_ShapeState[7] = {0, 2, 2, 2, 4, 4, 4};
const COLORREF g_ShapeColor[6] = {0xFF, 0xFF00, 0xFF0000, 0xFFFF, 0xFFFF00, 0xFF00FF};


////////////global fun///////////////////////
COLORREF GetNextShapeColor();

////////////type def//////////////////////
enum ShapeType { SHAPE_TIAN = 0, SHAPE_I, SHAPE_Z, SHAPE_INVERSE_Z, SHAPE_T, SHAPE_L, SHAPE_INVERSE_L, SHAPE_MAX };
enum RelativeDirectionType { NONE = 0, LEFT, LEFT_TOP, TOP, RIGHT_TOP, RIGHT, RIGHT_BOTTOM, BOTTOM, LEFT_BOTTOM };
enum GameStateType {PAUSE = 0, RUN};

//////////////macro def////////////////////
unsigned __int64 GetCpuCycle();
#define RAND() (srand((unsigned int)GetCpuCycle()), rand())

extern int g_Speed;
#define SPEED (800 / g_Speed)

#define TIMER_SPEED 101

#define method1 1
#define method2 2
#define METHOD method2
//////////////////////////////////////////////

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


