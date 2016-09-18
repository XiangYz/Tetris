
// TetrisView.h : interface of the CTetrisView class
//

#pragma once

class CTetrisView : public CView
{
protected: // create from serialization only
	CTetrisView();
	DECLARE_DYNCREATE(CTetrisView)

// Attributes
public:
	CTetrisDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTetrisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


public:
	
	CRect m_Rt[3];			 // 三个游戏区
	int m_nSquareSize;		 // 小方块的边长
	CPoint m_LeftTopPoint[MAX_Y][MAX_X];  // 小方块的左上角坐标

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnMemDraw(CDC* pDC);
	void DrawSquare(CDC *pDC, LPRECT lpRect, COLORREF color);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void PrintScr();
//	void InitGame();
	int CheckTouch();
	int CheckLeftTouch();
	int CheckRightTouch();
	int CheckBottomTouch();
	int CheckSquareTouch();
	void AddCurrentShapeToArray();
//	int CheckLine();
	int DestroyLine();
	void HandleCurrentStop();
	void DrawNextShape(CDC *pDC, const CPoint & point);
};

#ifndef _DEBUG  // debug version in TetrisView.cpp
inline CTetrisDoc* CTetrisView::GetDocument() const
   { return reinterpret_cast<CTetrisDoc*>(m_pDocument); }
#endif

