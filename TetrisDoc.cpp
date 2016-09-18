
// TetrisDoc.cpp : implementation of the CTetrisDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tetris.h"
#endif

#include "TetrisDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTetrisDoc

IMPLEMENT_DYNCREATE(CTetrisDoc, CDocument)

BEGIN_MESSAGE_MAP(CTetrisDoc, CDocument)
END_MESSAGE_MAP()


// CTetrisDoc construction/destruction

CTetrisDoc::CTetrisDoc()
{
	// TODO: add one-time construction code here

}

CTetrisDoc::~CTetrisDoc()
{
}

BOOL CTetrisDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	this->SetTitle(L"新游戏");

	NewGame();

	return TRUE;
}



void CTetrisDoc::NewGame()
{
	m_nLineDestroyed = 0;
	m_nScore = 0;

	memset(m_FalledUnitFlag, 0, MAX_X * MAX_Y * sizeof (int));
	
#if METHOD == method1
	int n = m_FalledUnit.GetSize();
	for (int i = 0; i < n; i++)
	{
		
		CUnit *pUnit = (CUnit *)m_FalledUnit[i];	
		delete pUnit;
	}
	m_FalledUnit.RemoveAll();

#elif METHOD == method2
	memset(m_FalledUnitColor, 0, MAX_X * MAX_Y * sizeof (COLORREF));
#endif

	m_IsCurrentGameRun = PAUSE;
	m_IsCurrentStop = 0;
	m_bGameOver = FALSE;

	m_CurrentShape.InitShape(MAX_X, MAX_Y);
	m_NextShape.InitShape(MAX_X, MAX_Y);
	
}


// CTetrisDoc serialization

void CTetrisDoc::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << m_nLineDestroyed << m_nScore << m_IsCurrentGameRun \
			<< m_IsCurrentStop << m_bGameOver;

		ar << g_Speed;

		m_CurrentShape.Serialize(ar);
		m_NextShape.Serialize(ar);	

		for (int i = 0; i < MAX_Y; i++)
		{
			for (int j = 0; j < MAX_X; j++)
			{
				ar << m_FalledUnitFlag[i][j];
			}
		}

#if METHOD == method1
		m_FalledUnit.Serialize(ar);

#elif METHOD == method2

		for (int i = 0; i < MAX_Y; i++)
		{
			for (int j = 0; j < MAX_X; j++)
			{
				ar << m_FalledUnitColor[i][j];
			}
		}
#endif

	}

	else
	{
		ar >> m_nLineDestroyed >> m_nScore >> m_IsCurrentGameRun \
			>> m_IsCurrentStop >> m_bGameOver;

		ar >> g_Speed;

		m_CurrentShape.Serialize(ar);
		m_NextShape.Serialize(ar);

		for (int i = 0; i < MAX_Y; i++)
		{
			for (int j = 0; j < MAX_X; j++)
			{
				ar >> m_FalledUnitFlag[i][j];
			}
		}

#if METHOD == method1
		m_FalledUnit.Serialize(ar);
#elif METHOD == method2

		for (int i = 0; i < MAX_Y; i++)
		{
			for (int j = 0; j < MAX_X; j++)
			{
				ar >> m_FalledUnitColor[i][j];
			}
		}
#endif
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CTetrisDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CTetrisDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTetrisDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTetrisDoc diagnostics

#ifdef _DEBUG
void CTetrisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTetrisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTetrisDoc commands



int CTetrisDoc::IsGameOver()
{

#if METHOD == method1
	int min_y = MAX_Y - 1;
	int n = m_FalledUnit.GetSize();
	for (int i = 0; i < n; i++)
	{
		CUnit *pUnit = (CUnit *)m_FalledUnit[i];
		if (pUnit->y < min_y)
			min_y = pUnit->y;
	}

	if (min_y <= 0) return TRUE;
	return FALSE;

#elif METHOD == method2

	for (int i = 0; i < MAX_X; i++)
	{
		if (m_FalledUnitFlag[0][i] == 1)
		{
			return TRUE;
		}
	}

	return FALSE;

#endif
}


#if METHOD == method1

void swap(void *p1, void *p2)
{
	void *pTmp;

	pTmp = p1;
	p1 = p2;
	p2 = pTmp;
}

void CTetrisDoc::SortUnitArray()
{
	CUnit *pUnit1, *pUnit2, *pMaxUnit;
	int N = m_FalledUnit.GetSize();

	int i, j;
	for (i = 0; i < N; i++)
	{
		pUnit1 = (CUnit *)m_FalledUnit[i];
		pMaxUnit = pUnit1;

		for (j = i + 1; j < N; j++)
		{
			pUnit2 = (CUnit *)m_FalledUnit[j];
			if (*pUnit2 > *pMaxUnit)     // 从大到小
				pMaxUnit = pUnit2;
		}

		swap(pUnit1, pUnit2);
	}
}


void CTetrisDoc::SyncFromArrayToFlag()
{

	memset(m_FalledUnitFlag, 0, MAX_X * MAX_Y * sizeof (m_FalledUnitFlag[0][0]));

	int n = m_FalledUnit.GetSize();
	for (int i = 0; i < n; i++)
	{
		CUnit *pUnit = (CUnit *)m_FalledUnit[i];
		int x = pUnit->x;
		int y = pUnit->y;
		m_FalledUnitFlag[y][x] = 1;
	}

}

// 此同步函数有缺陷
void CTetrisDoc::SyncFromFlagToArray()
{
	BOOL IsDel = FALSE;
	while (1)
	{
		for (int i = 0; i < m_FalledUnit.GetSize(); i++)
		{
			CUnit *pUnit = (CUnit *)m_FalledUnit[i];
			int x = pUnit->x;
			int y = pUnit->y;

			if (m_FalledUnitFlag[y][x] == 0)
			{
				delete pUnit;
				m_FalledUnit.RemoveAt(i);
				IsDel = TRUE;
				break;
			}
		}

		if (!IsDel)
			break;
		else
			IsDel = FALSE;
	}

}

#endif