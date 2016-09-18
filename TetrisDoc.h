
// TetrisDoc.h : interface of the CTetrisDoc class
//


#pragma once

#include "Shape.h"

class CTetrisDoc : public CDocument
{
protected: // create from serialization only
	CTetrisDoc();
	DECLARE_DYNCREATE(CTetrisDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CTetrisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

public:
	CShape m_CurrentShape;  // ��ǰͼ��
	CShape m_NextShape;     // ��һ��ͼ��

	int m_FalledUnitFlag[MAX_Y][MAX_X];			// �����µķ���λ�ñ�ʶ

#if METHOD == method1
	CObArray m_FalledUnit;  // �����µ�С����
#elif METHOD == method2
	COLORREF m_FalledUnitColor[MAX_Y][MAX_X];	// �����µķ������ɫ
#endif

	UINT m_nLineDestroyed;
	UINT m_nScore;

	int m_IsCurrentGameRun;  // ��Ϸ״̬
	int m_IsCurrentStop;	 // ��ǰͼ����ֹͣ�����־
	BOOL m_bGameOver;		 // ��ǰ��Ϸ����

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

	void NewGame();
	int IsGameOver();

#if METHOD == method1
	void SortUnitArray();
	void SyncFromArrayToFlag();
	void SyncFromFlagToArray();
#endif
};
