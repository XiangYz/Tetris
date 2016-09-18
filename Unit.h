#pragma once

class CUnit : public CObject
{
	DECLARE_SERIAL(CUnit);
public:
	CUnit(void);
	CUnit(const CUnit &);
	virtual ~CUnit(void);

public:
	int x;
	int y;
	COLORREF m_UnitColor;
	INT m_RelativeDirection;
	INT m_RelativeDistance;

	friend BOOL operator == (CUnit &unit1, CUnit &unit2);
	friend BOOL operator < (CUnit &unit1, CUnit &unit2);
	friend BOOL operator > (CUnit &unit1, CUnit &unit2);

	CUnit & operator = (const CUnit &);


	virtual void Serialize(CArchive& ar);
};

