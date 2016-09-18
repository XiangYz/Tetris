
// stdafx.cpp : source file that includes just the standard includes
// Tetris.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

int g_Speed = 1;
//const int MAX_Y = 20;
//const int MAX_X = 12;
//const INT g_ShapeState[7] = {0, 2, 2, 2, 4, 4, 4};
//const COLORREF g_ShapeColor[6] = {0xFF, 0xFF00, 0xFF0000, 0xFFFF, 0xFFFF00, 0xFF00FF};
int g_ShapeColorIndex = 0;

COLORREF GetNextShapeColor()
{
	g_ShapeColorIndex++;
	if (g_ShapeColorIndex == 6)
	{
		g_ShapeColorIndex = 0;

	}

	return g_ShapeColor[g_ShapeColorIndex];
}


__declspec (naked) unsigned __int64 GetCpuCycle()
{
	_asm
	{
		rdtsc
			ret
	}
}

