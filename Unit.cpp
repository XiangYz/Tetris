#include "StdAfx.h"
#include "Unit.h"


IMPLEMENT_SERIAL(CUnit, CObject, 1)

CUnit::CUnit(void)
{
}

CUnit::CUnit(const CUnit &unit)
{
	x = unit.x;
	y = unit.y;
	m_RelativeDirection = unit.m_RelativeDirection;
	m_RelativeDistance = unit.m_RelativeDistance;
	m_UnitColor = unit.m_UnitColor;
}

CUnit::~CUnit(void)
{
}

BOOL operator == (CUnit &unit1, CUnit &unit2)
{
	return (unit1.x == unit2.x && unit1.y == unit2.y);
}

BOOL operator < (CUnit &unit1, CUnit &unit2)
{
	if (unit1.y == unit2.y)
		return unit1.x < unit2.x;
	return unit1.y < unit2.y;
}

BOOL operator > (CUnit &unit1, CUnit &unit2)
{
	if (unit1.y == unit2.y)
		return unit1.x > unit2.x;
	return unit1.y > unit2.y;
}

CUnit & CUnit::operator = (const CUnit &unit)
{
	x = unit.x;
	y = unit.y;
	m_RelativeDirection = unit.m_RelativeDirection;
	m_RelativeDistance = unit.m_RelativeDistance;
	m_UnitColor = unit.m_UnitColor;

	return *this;
}

void CUnit::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << x << y << m_UnitColor << m_RelativeDirection << m_RelativeDistance;
	}
	else
	{	// loading code
		ar >> x >> y >> m_UnitColor >> m_RelativeDirection >> m_RelativeDistance;
	}
}
