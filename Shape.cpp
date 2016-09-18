#include "StdAfx.h"
#include "Shape.h"

IMPLEMENT_SERIAL(CShape, CObject, 1)

CShape::CShape(void)
{

}


CShape::~CShape(void)
{

}


CShape & CShape::operator = (const CShape &shape)
{
		m_CenterUnit = shape.m_CenterUnit;
		for (int i = 0; i < 3; i++)
		{
			m_OtherUnit[i] = shape.m_OtherUnit[i];
		}
		m_nType = shape.m_nType;
		m_nState = shape.m_nState;
		m_ShapeColor = shape.m_ShapeColor;
	
		return *this;
}


void CShape::InitShape(int maxX, int maxY)
{
	COLORREF CurrentColor = GetNextShapeColor();
	m_nType = RAND() % SHAPE_MAX;
	m_nState = 0;  // Ĭ�ϵ�״̬
	m_ShapeColor = CurrentColor;

	m_CenterUnit.x = maxX / 2;
	m_CenterUnit.y = 0;
	m_CenterUnit.m_UnitColor = CurrentColor;
	m_CenterUnit.m_RelativeDirection = NONE;
	m_CenterUnit.m_RelativeDistance = 0;

	for (int i = 0; i < 3; i++)
	{
		m_OtherUnit[i].m_UnitColor = CurrentColor;
	}

	GenerateRelativePos();
	int n = RAND() % 4;
	while (n--)
	{
		GotoNextState();
	}
	CalcCoordinate();

	int nToMove = CheckTouchUp();
	if (nToMove != 0)
	{
		MoveDown(nToMove);
	}
}



void CShape::GenerateRelativePos()
{
	switch (m_nType)
	{
	case SHAPE_TIAN:
	{
		m_OtherUnit[0].m_RelativeDirection = RIGHT;
		m_OtherUnit[0].m_RelativeDistance = 1;
		m_OtherUnit[1].m_RelativeDirection = RIGHT_BOTTOM;
		m_OtherUnit[1].m_RelativeDistance = 1;
		m_OtherUnit[2].m_RelativeDirection = BOTTOM;
		m_OtherUnit[2].m_RelativeDistance = 1;
	}
		break;
	case SHAPE_I:
	{
		m_OtherUnit[0].m_RelativeDirection = TOP;
		m_OtherUnit[0].m_RelativeDistance = 1;
		m_OtherUnit[1].m_RelativeDirection = BOTTOM;
		m_OtherUnit[1].m_RelativeDistance = 1;
		m_OtherUnit[2].m_RelativeDirection = BOTTOM;
		m_OtherUnit[2].m_RelativeDistance = 2;
	}
		break;
	case SHAPE_Z:
	{
		m_OtherUnit[0].m_RelativeDirection = TOP;
		m_OtherUnit[0].m_RelativeDistance = 1;
		m_OtherUnit[1].m_RelativeDirection = LEFT_TOP;
		m_OtherUnit[1].m_RelativeDistance = 1;
		m_OtherUnit[2].m_RelativeDirection = RIGHT;
		m_OtherUnit[2].m_RelativeDistance = 1;
	}
		break;
	case SHAPE_INVERSE_Z:
	{
		m_OtherUnit[0].m_RelativeDirection = RIGHT_TOP;
		m_OtherUnit[0].m_RelativeDistance = 1;
		m_OtherUnit[1].m_RelativeDirection = TOP;
		m_OtherUnit[1].m_RelativeDistance = 1;
		m_OtherUnit[2].m_RelativeDirection = LEFT;
		m_OtherUnit[2].m_RelativeDistance = 1;
	}
		break;
	case SHAPE_T:
	{
		m_OtherUnit[0].m_RelativeDirection = TOP;
		m_OtherUnit[0].m_RelativeDistance = 1;
		m_OtherUnit[1].m_RelativeDirection = LEFT;
		m_OtherUnit[1].m_RelativeDistance = 1;
		m_OtherUnit[2].m_RelativeDirection = RIGHT;
		m_OtherUnit[2].m_RelativeDistance = 1;
	}
		break;
	case SHAPE_L:
	{
		m_OtherUnit[0].m_RelativeDirection = LEFT_TOP;
		m_OtherUnit[0].m_RelativeDistance = 1;
		m_OtherUnit[1].m_RelativeDirection = LEFT;
		m_OtherUnit[1].m_RelativeDistance = 1;
		m_OtherUnit[2].m_RelativeDirection = RIGHT;
		m_OtherUnit[2].m_RelativeDistance = 1;
	}
		break;
	case SHAPE_INVERSE_L:
	{
		m_OtherUnit[0].m_RelativeDirection = RIGHT_TOP;
		m_OtherUnit[0].m_RelativeDistance = 1;
		m_OtherUnit[1].m_RelativeDirection = RIGHT;
		m_OtherUnit[1].m_RelativeDistance = 1;
		m_OtherUnit[2].m_RelativeDirection = LEFT;
		m_OtherUnit[2].m_RelativeDistance = 1;
	}
		break;
	default:
		break;
	}
}

void CShape::CalcCoordinate()
{
	for (int i = 0; i < 3; i++)
	{
		switch (m_OtherUnit[i].m_RelativeDirection)
		{
		case LEFT:
		{
					 m_OtherUnit[i].x = m_CenterUnit.x - m_OtherUnit[i].m_RelativeDistance;
					 m_OtherUnit[i].y = m_CenterUnit.y;
		}
			break;
		case LEFT_TOP:
		{
					 m_OtherUnit[i].x = m_CenterUnit.x - m_OtherUnit[i].m_RelativeDistance;
					 m_OtherUnit[i].y = m_CenterUnit.y - m_OtherUnit[i].m_RelativeDistance;
		}
			break;
		case TOP:
		{
					 m_OtherUnit[i].x = m_CenterUnit.x;
					 m_OtherUnit[i].y = m_CenterUnit.y - m_OtherUnit[i].m_RelativeDistance;
		}
			break;
		case RIGHT_TOP:
		{
					 m_OtherUnit[i].x = m_CenterUnit.x + m_OtherUnit[i].m_RelativeDistance;
					 m_OtherUnit[i].y = m_CenterUnit.y - m_OtherUnit[i].m_RelativeDistance;
		}
			break;
		case RIGHT:
		{
					 m_OtherUnit[i].x = m_CenterUnit.x + m_OtherUnit[i].m_RelativeDistance;
					 m_OtherUnit[i].y = m_CenterUnit.y;
		}
			break;
		case RIGHT_BOTTOM:
		{
					 m_OtherUnit[i].x = m_CenterUnit.x + m_OtherUnit[i].m_RelativeDistance;
					 m_OtherUnit[i].y = m_CenterUnit.y + m_OtherUnit[i].m_RelativeDistance;
		}
			break;
		case BOTTOM:
		{
					 m_OtherUnit[i].x = m_CenterUnit.x;
					 m_OtherUnit[i].y = m_CenterUnit.y + m_OtherUnit[i].m_RelativeDistance;
		}
			break;
		case LEFT_BOTTOM:
		{
					 m_OtherUnit[i].x = m_CenterUnit.x - m_OtherUnit[i].m_RelativeDistance;
					 m_OtherUnit[i].y = m_CenterUnit.y + m_OtherUnit[i].m_RelativeDistance;
		}
			break;
		default:
			break;
		}
	}
}



int CShape::CheckTouchUp()
{
	int min_y = GetTopEnd();

	return min_y >= 0 ? 0 : -min_y;
}

int CShape::GetType(void)
{
	return m_nType;
}


int CShape::GetCurrentState(void)
{
	return m_nState;
}


int CShape::GetLeftEnd(void)
{
	int min_x = m_CenterUnit.x;

	for (int i = 0; i < 3; i++)
	{
		if (m_OtherUnit[i].x < min_x)
		{
			min_x = m_OtherUnit[i].x;
		}
	}

	return min_x;
}



int CShape::GetRightEnd(void)
{
	int max_x = m_CenterUnit.x;

	for (int i = 0; i < 3; i++)
	{
		if (m_OtherUnit[i].x > max_x)
		{
			max_x = m_OtherUnit[i].x;
		}
	}

	return max_x;
}


int CShape::GetTopEnd(void)
{
	int min_y = m_CenterUnit.y;

	for (int i = 0; i < 3; i++)
	{
		if (m_OtherUnit[i].y < min_y)
		{
			min_y = m_OtherUnit[i].y;
		}
	}

	return min_y;
}


int CShape::GetBottomEnd(void)
{
	int max_y = m_CenterUnit.y;

	for (int i = 0; i < 3; i++)
	{
		if (m_OtherUnit[i].y > max_y)
		{
			max_y = m_OtherUnit[i].y;
		}
	}

	return max_y;
}


int CShape::GetXCount(void)
{
	return GetRightEnd() - GetLeftEnd() + 1;
}


int CShape::GetYCount(void)
{
	return GetBottomEnd() - GetTopEnd() + 1;
}


//////////////////////////////////////////

void CShape::MoveLeft(int n)
{
	m_CenterUnit.x -= n;
	for (int i = 0; i < 3; i++)
	{
		m_OtherUnit[i].x -= n;
	}
}


void CShape::MoveRight(int n)
{
	m_CenterUnit.x += n;
	for (int i = 0; i < 3; i++)
	{
		m_OtherUnit[i].x += n;
	}
}


void CShape::MoveDown(int n)
{
	m_CenterUnit.y += n;
	for (int i = 0; i < 3; i++)
	{
		m_OtherUnit[i].y += n;
	}
}

void CShape::MoveUp(int n)
{
	m_CenterUnit.y -= n;
	for (int i = 0; i < 3; i++)
	{
		m_OtherUnit[i].y -= n;
	}

}

int CShape::GotoNextState(void)
{
	if (g_ShapeState[m_nType] == 4)
	{
		if (++m_nState == 4)
		{
			m_nState = 0;
		}

		CWRotate90();
	}
	else if (g_ShapeState[m_nType] == 2)
	{
		if (m_nState == 0)
		{
			++m_nState;
			CWRotate90();
		}
		else if (m_nState == 1)
		{
			--m_nState;
			ACWRotate90();
		}
	}

	return 1;
}



int CShape::GotoPrevState(void)
{
	if (g_ShapeState[m_nType] == 4)
	{
		if (--m_nState == -1)
		{
			m_nState = 3;
		}

		ACWRotate90();
	}
	else if (g_ShapeState[m_nType] == 2)
	{
		if (m_nState == 0)
		{
			++m_nState;
			CWRotate90();
		}
		else if (m_nState == 1)
		{
			--m_nState;
			ACWRotate90();
		}
	}

	return 1;
}

void CShape::CWRotate90(void)
{
	if (m_nType != SHAPE_TIAN)
	{
		for (int i = 0; i < 3; i++)
		{
			m_OtherUnit[i].m_RelativeDirection += 2;
			if (m_OtherUnit[i].m_RelativeDirection > 8)
			{
				m_OtherUnit[i].m_RelativeDirection -= 8;
			}
		}

		CalcCoordinate();
	}
}


void CShape::ACWRotate90(void)
{
	if (m_nType != SHAPE_TIAN)
	{
		for (int i = 0; i < 3; i++)
		{
			m_OtherUnit[i].m_RelativeDirection -= 2;
			if (m_OtherUnit[i].m_RelativeDirection <= 0)
			{
				m_OtherUnit[i].m_RelativeDirection += 8;
			}
		}

		CalcCoordinate();
	}
}


void CShape::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_nType << m_nState << m_ShapeColor;
		m_CenterUnit.Serialize(ar);
		for (int i = 0; i < 3; i++)
		{
			m_OtherUnit[i].Serialize(ar);
		}
	}
	else
	{	// loading code
		ar >> m_nType >> m_nState >> m_ShapeColor;
		m_CenterUnit.Serialize(ar);
		for (int i = 0; i < 3; i++)
		{
			m_OtherUnit[i].Serialize(ar);
		}
	}
}
