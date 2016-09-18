#pragma once

#include "Unit.h"


class CShape : public CObject
{
	DECLARE_SERIAL(CShape);
public:
	CShape(void);
	virtual ~CShape(void);

public:
	CUnit m_CenterUnit;
	CUnit m_OtherUnit[3];

	INT m_nType;
	INT m_nState;

	COLORREF m_ShapeColor;

public:
	void InitShape(int maxX, int maxY);
	int GetLeftEnd(void);
	int GetRightEnd(void);
	int GetTopEnd(void);
	int GetBottomEnd(void);

	int GetXCount(void);
	int GetYCount(void);
	int GetType(void);
	int GetCurrentState(void);

	void MoveLeft(int n);
	void MoveRight(int n);
	void MoveDown(int n);
	void MoveUp(int n);

	void CWRotate90(void);
	void ACWRotate90(void);
	int GotoPrevState(void);
	int GotoNextState(void);

	void GenerateRelativePos();
	void CalcCoordinate();
	int CheckTouchUp();

	CShape & operator =(const CShape &);
	virtual void Serialize(CArchive& ar);
};

