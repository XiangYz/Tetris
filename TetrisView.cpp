
// TetrisView.cpp : implementation of the CTetrisView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tetris.h"
#endif

#include "TetrisDoc.h"
#include "TetrisView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTetrisView

IMPLEMENT_DYNCREATE(CTetrisView, CView)

BEGIN_MESSAGE_MAP(CTetrisView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTetrisView construction/destruction

CTetrisView::CTetrisView()
{
	// TODO: add construction code here



}

CTetrisView::~CTetrisView()
{
}

BOOL CTetrisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTetrisView drawing

void CTetrisView::OnDraw(CDC* pDC)
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nWidth = ClientRect.Width();
	int nHeight = ClientRect.Height();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
	memDC.SelectObject(memBitmap);

	memDC.FillSolidRect(&ClientRect, RGB(255, 255, 255));

	OnMemDraw(&memDC);

	pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);

	memBitmap.DeleteObject();
	memDC.DeleteDC();
}

void CTetrisView::OnMemDraw(CDC* pDC)
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nWidth = ClientRect.Width();
	int nHeight = ClientRect.Height();

	// 整个客户区的背景
	pDC->FillSolidRect(&ClientRect, RGB(255, 128, 255));


	// 填充三个方块的背景
	CRect EdgeRt[3];
	for (int i = 0; i < 3; i++)
	{
		EdgeRt[i] = m_Rt[i];

		EdgeRt[i].left -= 2;
		EdgeRt[i].right += 2;
		EdgeRt[i].top -= 2;
		EdgeRt[i].bottom += 2;
	}
	pDC->FillSolidRect(&EdgeRt[0], RGB(128, 128, 128));
	pDC->FillSolidRect(&EdgeRt[1], RGB(128, 128, 128));
	pDC->FillSolidRect(&EdgeRt[2], RGB(128, 128, 128));

	pDC->FillSolidRect(&m_Rt[0], RGB(57, 0, 57));
	pDC->FillSolidRect(&m_Rt[1], RGB(0, 64, 0));
	pDC->FillSolidRect(&m_Rt[2], RGB(253, 78, 2));


	// 画游戏区
	// 画已落下的小方块
#if METHOD == method1

	int nFalled = pDoc->m_FalledUnit.GetSize();
	for (int i = 0; i < nFalled; i++)
	{
		CUnit *punit = (CUnit *)pDoc->m_FalledUnit[i];
		int x = punit->x;
		int y = punit->y;

		CRect SquareRect;
		SquareRect.left = m_LeftTopPoint[y][x].x;
		SquareRect.top = m_LeftTopPoint[y][x].y;
		SquareRect.right = SquareRect.left + m_nSquareSize;
		SquareRect.bottom = SquareRect.top + m_nSquareSize;

		COLORREF UnitColor = punit->m_UnitColor;
		DrawSquare(pDC, &SquareRect, UnitColor);

	}

#elif METHOD == method2

	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			if (pDoc->m_FalledUnitFlag[i][j] == 1)
			{
				CRect SquareRect;
				SquareRect.left = m_LeftTopPoint[i][j].x;
				SquareRect.top = m_LeftTopPoint[i][j].y;
				SquareRect.right = SquareRect.left + m_nSquareSize;
				SquareRect.bottom = SquareRect.top + m_nSquareSize;

				COLORREF UnitColor = pDoc->m_FalledUnitColor[i][j];
				DrawSquare(pDC, &SquareRect, UnitColor);
			}
		}
	}


#endif

	// 画当前的图形
	if (pDoc->m_bGameOver == FALSE)
	{

		// 画中心单元
		COLORREF ShapeColor = pDoc->m_CurrentShape.m_ShapeColor;
		CRect SquareRect;
		int x = pDoc->m_CurrentShape.m_CenterUnit.x;
		int y = pDoc->m_CurrentShape.m_CenterUnit.y;
		SquareRect.left = m_LeftTopPoint[y][x].x;
		SquareRect.top = m_LeftTopPoint[y][x].y;
		SquareRect.right = SquareRect.left + m_nSquareSize;
		SquareRect.bottom = SquareRect.top + m_nSquareSize;
		DrawSquare(pDC, &SquareRect, ShapeColor);

		// 画其它单元
		for (int i = 0; i < 3; i++)
		{
			CRect SquareRect;
			int x = pDoc->m_CurrentShape.m_OtherUnit[i].x;
			int y = pDoc->m_CurrentShape.m_OtherUnit[i].y;
			SquareRect.left = m_LeftTopPoint[y][x].x;
			SquareRect.top = m_LeftTopPoint[y][x].y;
			SquareRect.right = SquareRect.left + m_nSquareSize;
			SquareRect.bottom = SquareRect.top + m_nSquareSize;
			DrawSquare(pDC, &SquareRect, ShapeColor);
		}
	}


	// 画第一个说明区
	CFont font;
	font.CreateFont(26, 10, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, \
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, \
		VARIABLE_PITCH, TEXT("Times New Roman"));
	pDC->SelectObject(font);

	if (pDoc->m_bGameOver == FALSE)
	{

		CString sNext;
		sNext.Format(L"下一个图形");
		pDC->SetBkMode(TRANSPARENT);
		CRect rt1(m_Rt[1]);
		rt1.top += 20;
		pDC->DrawText(sNext, &rt1, DT_TOP | DT_CENTER | DT_SINGLELINE);


		DrawNextShape(pDC, CPoint(rt1.left + rt1.Width() / 2, rt1.top + rt1.Height() / 2 - 20));
	}
	else
	{
		CString sOver;
		sOver.Format(L"游戏结束");
		pDC->SetBkMode(TRANSPARENT);
		CRect rt1(m_Rt[1]);
		rt1.top += 30;
		pDC->DrawText(sOver, &rt1, DT_TOP | DT_CENTER | DT_SINGLELINE);
	}
	
	// 画第二个说明区

	CString sScore, sLine, sSpeedLevel;
	sLine.Format(L"行数：%d", pDoc->m_nLineDestroyed);
	sScore.Format(L"分数：%d", pDoc->m_nScore);
	sSpeedLevel.Format(L"速度级别：%d", g_Speed);

	CRect rt[2];
	rt[0] = m_Rt[2];
	rt[1] = m_Rt[2];
	rt[0].bottom = rt[0].top + rt[0].Height() / 2;
	rt[1].top = rt[0].bottom;




	pDC->DrawText(sSpeedLevel, &rt[0], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	pDC->DrawText(sLine, &rt[1], DT_SINGLELINE | DT_TOP | DT_CENTER);
	pDC->DrawText(sScore, &rt[1], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	font.DeleteObject();

}

void CTetrisView::DrawNextShape(CDC *pDC, const CPoint & point)
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int x = point.x;
	int y = point.y;
	int w = m_nSquareSize;
	COLORREF color = pDoc->m_NextShape.m_ShapeColor;

	CRect rt;
	rt.left = x;
	rt.top = y;
	rt.right = x + w;
	rt.bottom = y + w;
	//pDC->FillSolidRect(x, y, w, w, pDoc->m_NextShape.m_CenterUnit.m_UnitColor);
	DrawSquare(pDC, &rt, color);

	for (int i = 0; i < 3; i++)
	{
		int type = pDoc->m_NextShape.m_OtherUnit[i].m_RelativeDirection;
		int dist = pDoc->m_NextShape.m_OtherUnit[i].m_RelativeDistance;
		switch (type)
		{
		case LEFT:
		{
					 x = point.x - dist * w;
					 y = point.y;
		}
			break;
		case LEFT_TOP:
		{
						 x = point.x - dist * w;
						 y = point.y - dist * w;
		}
			break;
		case TOP:
		{
					x = point.x;
					y = point.y - dist * w;
		}
			break;
		case RIGHT_TOP:
		{
						  x = point.x + dist * w;
						  y = point.y - dist * w;
		}
			break;
		case RIGHT:
		{
					  x = point.x + dist * w;
					  y = point.y;
		}
			break;
		case RIGHT_BOTTOM:
		{
							 x = point.x + dist * w;
							 y = point.y + dist * w;
		}
			break;
		case BOTTOM:
		{
					   x = point.x;
					   y = point.y + dist * w;
		}
			break;
		case LEFT_BOTTOM:
		{
							x = point.x - dist * w;
							y = point.y + dist * w;
		}
			break;
		default:
			break;
		}

		//pDC->FillSolidRect(x, y, w, w, pDoc->m_NextShape.m_OtherUnit[i].m_UnitColor);

		rt.left = x;
		rt.top = y;
		rt.right = x + w;
		rt.bottom = y + w;
		DrawSquare(pDC, &rt, color);
	}
}

void CTetrisView::DrawSquare(CDC *pDC, LPRECT lpRect, COLORREF color)
{
	CBrush brush;
	brush.CreateStockObject(DKGRAY_BRUSH);
	pDC->SelectObject(brush);
	pDC->Rectangle(lpRect);
	brush.DeleteObject();

	lpRect->left += 1;
	lpRect->top += 1;
	lpRect->right -= 1;
	lpRect->bottom -= 1;
	brush.CreateSolidBrush(color);
	pDC->SelectObject(brush);
	pDC->Rectangle(lpRect);
	brush.DeleteObject();
}


// CTetrisView printing

BOOL CTetrisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTetrisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTetrisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTetrisView diagnostics

#ifdef _DEBUG
void CTetrisView::AssertValid() const
{
	CView::AssertValid();
}

void CTetrisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTetrisDoc* CTetrisView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTetrisDoc)));
	return (CTetrisDoc*)m_pDocument;
}
#endif //_DEBUG


// CTetrisView message handlers



void CTetrisView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	CRect ClientRect;
	GetClientRect(&ClientRect);       // 在OnCreate中无用

	ClientRect.bottom -= 30;   // 状态栏的宽度减去

	m_Rt[0] = ClientRect;
	m_Rt[1] = ClientRect;
	m_Rt[2] = ClientRect;

	m_Rt[0].top += 2;
	m_Rt[0].left += 2;
	m_nSquareSize = ClientRect.Height() / MAX_Y;			// 记录小方格的边长
	m_Rt[0].bottom = m_Rt[0].top + MAX_Y * m_nSquareSize;
	m_Rt[0].right = m_Rt[0].left + MAX_X * m_nSquareSize;

	m_Rt[1].bottom = m_Rt[0].bottom;
	m_Rt[2].bottom = m_Rt[0].bottom;

	m_Rt[1].left = m_Rt[0].right + 4;
	m_Rt[2].left = m_Rt[0].right + 4;

	m_Rt[1].right -= 2;
	m_Rt[2].right -= 2;

	m_Rt[1].top += 2;
	m_Rt[1].bottom = m_Rt[1].top + m_Rt[1].Height() / 3 - 2;
	m_Rt[2].top = m_Rt[1].bottom + 4;
	m_Rt[2].bottom -= 2;

	//记录每个小方格的坐标
	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			m_LeftTopPoint[i][j].x = m_Rt[0].left + m_nSquareSize * j;
			m_LeftTopPoint[i][j].y = m_Rt[0].top + m_nSquareSize * i;
		}
	}

}

BOOL CTetrisView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE; // CView::OnEraseBkgnd(pDC);
}

void CTetrisView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (nIDEvent == TIMER_SPEED)
	{
		pDoc->m_CurrentShape.MoveDown(1);
		if (CheckBottomTouch())
		{
			pDoc->m_CurrentShape.MoveUp(1);
		}

		if (pDoc->m_IsCurrentStop)    // 当前块落停了
		{
			pDoc->m_IsCurrentStop = 0;
			HandleCurrentStop();
			
			if (pDoc->m_bGameOver)
			{
				KillTimer(TIMER_SPEED);
			}
		}
	}

	Invalidate();

	CView::OnTimer(nIDEvent);
}

void CTetrisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (nChar == VK_ESCAPE)
	{
		short ShiftState = GetKeyState(VK_SHIFT);
		if ((ShiftState & 0x8000) >> 15)
		{
			PostQuitMessage(0);
		}
	}
	if (nChar == VK_F1)
	{

	}
	else if (nChar == VK_F2 && (nFlags & VK_SHIFT))
	{
		pDoc->NewGame();
		KillTimer(TIMER_SPEED);
	}
	else if (nChar == VK_F3)
	{
		PrintScr();
	}
	else if ( (pDoc->m_IsCurrentGameRun == PAUSE) && \
		(nChar == VK_UP || nChar == VK_DOWN))
	{
		if ((GetKeyState(VK_SHIFT) & 0x8000) >> 15)
		{

			if (nChar == VK_UP)
			{
				if (g_Speed < 5) g_Speed++;
			}
			else if (nChar == VK_DOWN)
			{
				if (g_Speed > 1) g_Speed--;
			}
		}

	}
	else if (nChar == ' ' && pDoc->m_bGameOver == FALSE)
	{
		if (pDoc->m_IsCurrentGameRun == PAUSE)
		{
			pDoc->m_IsCurrentGameRun = RUN;
			SetTimer(TIMER_SPEED, SPEED, NULL);
		}
		else if (pDoc->m_IsCurrentGameRun == RUN)
		{
			pDoc->m_IsCurrentGameRun = PAUSE;
			KillTimer(TIMER_SPEED);
		}
	}
	else if (pDoc->m_bGameOver == FALSE && pDoc->m_IsCurrentGameRun == RUN)
	{

		if (nChar == VK_UP)
		{
			pDoc->m_CurrentShape.GotoNextState();
			if (CheckTouch())
			{
				pDoc->m_CurrentShape.GotoPrevState();
			}
		}
		else if (nChar == VK_DOWN)
		{
			pDoc->m_CurrentShape.MoveDown(1);
			if (CheckBottomTouch())
			{
				pDoc->m_CurrentShape.MoveUp(1);
			}
		}
		else if (nChar == VK_LEFT)
		{
			pDoc->m_CurrentShape.MoveLeft(1);
			if (CheckLeftTouch())
			{
				pDoc->m_CurrentShape.MoveRight(1);
			}
		}
		else if (nChar == VK_RIGHT)
		{
			pDoc->m_CurrentShape.MoveRight(1);
			if (CheckRightTouch())
			{
				pDoc->m_CurrentShape.MoveLeft(1);
			}
		}

		if (pDoc->m_IsCurrentStop)
		{
			pDoc->m_IsCurrentStop = 0;
			HandleCurrentStop();
			
			if (pDoc->m_bGameOver)
			{
				pDoc->m_IsCurrentGameRun = PAUSE;
				KillTimer(TIMER_SPEED);
			}
		}

	}

	Invalidate(TRUE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTetrisView::HandleCurrentStop()
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	AddCurrentShapeToArray();

	int n = DestroyLine();
	if (n > 0)
	{

		pDoc->m_nLineDestroyed += n;
		pDoc->m_nScore += n * 10 + (n - 1) * 3;
		
	}

	if (pDoc->IsGameOver() == FALSE)
	{
		pDoc->m_CurrentShape = pDoc->m_NextShape;
		pDoc->m_NextShape.InitShape(MAX_X, MAX_Y);
		
	}
	else
	{
		pDoc->m_bGameOver = TRUE;
	}
}

int CTetrisView::DestroyLine()
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return -1;

	int nDestroyed = 0;

#if METHOD == method1

	int min_y = MAX_Y - 1;
	int n = pDoc->m_FalledUnit.GetSize();
	for (int i = 0; i < n; i++)
	{
		CUnit *pUnit = (CUnit *)pDoc->m_FalledUnit[i];
		if (pUnit->y < min_y)
			min_y = pUnit->y;
	}

	for (int i = min_y; i < MAX_Y; i++)     // 需要检测的行
	{
		int j;
		for (j = 0; j < MAX_X; j++)
		{
			if (pDoc->m_FalledUnitFlag[i][j] != 1)
			{
				break;
			}
		}

		if (j == MAX_X)  // 碰到满了的行
		{
			nDestroyed += 1;

			// 上面的行下移
			for (int k = i - 1; k >= min_y; k--)
			{
				for (int w = 0; w < MAX_X; w++)
				{
					pDoc->m_FalledUnitFlag[k + 1][w] = pDoc->m_FalledUnitFlag[k][w];
					pDoc->m_FalledUnitFlag[k][w] = 0;
				}
			}

		}
	}

	if (nDestroyed != 0)
	{
		pDoc->SyncFromFlagToArray();
	}

#elif METHOD == method2

	for (int i = 0; i < MAX_Y; i++)     // 需要检测的行
	{
		int j;
		for (j = 0; j < MAX_X; j++)
		{
			if (pDoc->m_FalledUnitFlag[i][j] != 1)
			{
				break;
			}
		}

		if (j == MAX_X)  // 碰到满了的行
		{
			nDestroyed += 1;

			// 上面的行下移
			for (int k = i - 1; k >= 0; k--)
			{
				for (int w = 0; w < MAX_X; w++)
				{
					pDoc->m_FalledUnitFlag[k + 1][w] = pDoc->m_FalledUnitFlag[k][w];
					pDoc->m_FalledUnitFlag[k][w] = 0;

					pDoc->m_FalledUnitColor[k + 1][w] = pDoc->m_FalledUnitColor[k][w];
					pDoc->m_FalledUnitColor[k][w] = 0;
				}
			}

		}
	}


#endif


	return nDestroyed;
}

void CTetrisView::AddCurrentShapeToArray()
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// pay attention here

#if METHOD == method1
	CUnit *pUnitToSave = new CUnit(pDoc->m_CurrentShape.m_CenterUnit);
	pDoc->m_FalledUnit.Add(pUnitToSave);
	for (int i = 0; i < 3; i++)
	{
		pUnitToSave = new CUnit(pDoc->m_CurrentShape.m_OtherUnit[i]);
		pDoc->m_FalledUnit.Add(pUnitToSave);
	}

	pDoc->SyncFromArrayToFlag();

#elif METHOD == method2
	int y = pDoc->m_CurrentShape.m_CenterUnit.y;
	int x = pDoc->m_CurrentShape.m_CenterUnit.x;

	pDoc->m_FalledUnitFlag[y][x] = 1;
	pDoc->m_FalledUnitColor[y][x] = pDoc->m_CurrentShape.m_ShapeColor;

	for (int i = 0; i < 3; i++)
	{
		y = pDoc->m_CurrentShape.m_OtherUnit[i].y;
		x = pDoc->m_CurrentShape.m_OtherUnit[i].x;

		pDoc->m_FalledUnitFlag[y][x] = 1;
		pDoc->m_FalledUnitColor[y][x] = pDoc->m_CurrentShape.m_ShapeColor;
	}


#endif

}

void CTetrisView::PrintScr()
{
	HWND hDesktopWnd = ::GetDesktopWindow();
	CWnd *pDesktopWnd = CWnd::FromHandle(hDesktopWnd);
	CWindowDC DesktopDc(pDesktopWnd);
	CWindowDC Windowdc(this->GetParent());
	CClientDC ClientDc(this);

	CDC *pDc = &Windowdc;

	CRect Rt;
	GetParent()->GetWindowRect(&Rt);
	//GetClientRect(&Rt);
	//pDesktopWnd->GetWindowRect(&Rt);

	INT nWidth = Rt.Width();
	INT nHeight = Rt.Height();

	CDC memDC;
	memDC.CreateCompatibleDC(pDc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDc, nWidth, nHeight);
	memDC.SelectObject(memBitmap);
	memDC.BitBlt(0, 0, nWidth, nHeight, pDc, 0, 0, SRCCOPY);
	CDC *pmemDC = &memDC;



	INT nBitsOfPixel = pDc->GetDeviceCaps(BITSPIXEL);
	INT nLineBytes = (int)((int)(nWidth * nBitsOfPixel / 8.0 + 0.5) / 4.0 + 0.5) * 4;
	INT nImageSize = nWidth * nHeight * nBitsOfPixel / 8;  //Bytes
	INT nImageSpace = nLineBytes * nHeight;
	CHAR *pData = new CHAR[nImageSize];

	BITMAPINFOHEADER BmpInfoHead = { 0 };
	BmpInfoHead.biSize = sizeof (BITMAPINFOHEADER);
	BmpInfoHead.biWidth = nWidth;
	BmpInfoHead.biHeight = nHeight;
	BmpInfoHead.biPlanes = 1;
	BmpInfoHead.biBitCount = nBitsOfPixel;
	BmpInfoHead.biCompression = BI_RGB;
	BmpInfoHead.biSizeImage = nImageSize;

	::GetDIBits(pmemDC->GetSafeHdc(),
		(HBITMAP)(pmemDC->GetCurrentBitmap())->GetSafeHandle(),
		0, nHeight, pData, (LPBITMAPINFO)&BmpInfoHead, DIB_RGB_COLORS);

	INT nTotalSize = sizeof (BITMAPINFOHEADER)+sizeof (BITMAPFILEHEADER)+
		nImageSize;

	BITMAPFILEHEADER BmpFileHead = { 0 };
	BmpFileHead.bfType = MAKEWORD('B', 'M');
	BmpFileHead.bfSize = nTotalSize;
	BmpFileHead.bfReserved1 = 0;
	BmpFileHead.bfReserved2 = 0;
	BmpFileHead.bfOffBits = sizeof (BITMAPINFOHEADER)+
		sizeof (BITMAPFILEHEADER);

	BmpInfoHead.biSizeImage = nImageSize;
	BmpInfoHead.biXPelsPerMeter = 0;
	BmpInfoHead.biYPelsPerMeter = 0;
	BmpInfoHead.biClrUsed = 0;
	BmpInfoHead.biClrImportant = 0;


	CFile file;
	INT iRet = file.Open(L"scprt.bmp", CFile::modeCreate | CFile::modeWrite);
	if (iRet)
	{
		file.Write(&BmpFileHead, sizeof (BmpFileHead));
		file.Write(&BmpInfoHead, sizeof (BmpInfoHead));
		file.Write(pData, nImageSize);

		file.Close();
	}

	delete[] pData;
}

int CTetrisView::CheckTouch()
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return -1;

	int LeftEnd = pDoc->m_CurrentShape.GetLeftEnd();
	if (LeftEnd < 0) return 1;
	int RightEnd = pDoc->m_CurrentShape.GetRightEnd();
	if (RightEnd >= MAX_X) return 1;
	int BottomEnd = pDoc->m_CurrentShape.GetBottomEnd();
	if (BottomEnd >= MAX_Y) return 1;
	int TopTouch = pDoc->m_CurrentShape.CheckTouchUp();
	if (TopTouch > 0) return 1;

	int iRet = CheckSquareTouch();
	if (iRet == TRUE) return 1;


	return 0;
}


int CTetrisView::CheckLeftTouch()
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return -1;

	int LeftEnd = pDoc->m_CurrentShape.GetLeftEnd();
	if (LeftEnd < 0) return 1;
	int iRet = CheckSquareTouch();
	if (iRet == TRUE) return 1;

	return 0;
}


int CTetrisView::CheckRightTouch()
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return -1;

	int RightEnd = pDoc->m_CurrentShape.GetRightEnd();
	if (RightEnd >= MAX_X) return 1;
	int iRet = CheckSquareTouch();
	if (iRet == TRUE) return 1;

	return 0;
}


int CTetrisView::CheckBottomTouch()
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return -1;

	int BottomEnd = pDoc->m_CurrentShape.GetBottomEnd();
	if (BottomEnd >= MAX_Y)
	{
		pDoc->m_IsCurrentStop = 1;
		return 1;
	}

	int iRet = CheckSquareTouch();
	if (iRet == TRUE) 
	{
		pDoc->m_IsCurrentStop = 1;
		return 1;
	}

	return 0;
}


int CTetrisView::CheckSquareTouch()
{
	CTetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return -1;

#if METHOD == method1
	int n = pDoc->m_FalledUnit.GetSize();
	for (int i = 0; i < n; i++)
	{
		CUnit *punit = (CUnit *)pDoc->m_FalledUnit[i];
		if (pDoc->m_CurrentShape.m_CenterUnit == *punit)
			return 1;
		if (pDoc->m_CurrentShape.m_OtherUnit[0] == *punit)
			return 1;
		if (pDoc->m_CurrentShape.m_OtherUnit[1] == *punit)
			return 1;
		if (pDoc->m_CurrentShape.m_OtherUnit[2] == *punit)
			return 1;
	}
#elif METHOD == method2

	int x = pDoc->m_CurrentShape.m_CenterUnit.x;
	int y = pDoc->m_CurrentShape.m_CenterUnit.y;

	if (pDoc->m_FalledUnitFlag[y][x] == 1)
		return 1;

	for (int i = 0; i < 3; i++)
	{
		x = pDoc->m_CurrentShape.m_OtherUnit[i].x;
		y = pDoc->m_CurrentShape.m_OtherUnit[i].y;

		if (pDoc->m_FalledUnitFlag[y][x] == 1)
			return 1;
	}

#endif

	return 0;
}


void CTetrisView::OnDestroy()
{
	CView::OnDestroy();

	KillTimer(TIMER_SPEED);
	// TODO: Add your message handler code here
}



