#include "global.h"
#include "CCore.h"
#include "CObject.h"
#include "CKeyMgr.h"
#include "CScene.h"
#include "CChar.h"
#include "CBackground.h"

CCore::CCore()
	: m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_hBit(0)
	, m_memDC(0)
	, m_bMousetracking(true)
{
}

CCore::~CCore()
{
}

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_iratio = (float)WINDOW_SIZE / (float)CANVAS_SIZE;


	// GDI+ 초기화
	ULONG_PTR gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput gdistartupinput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdistartupinput, nullptr);

	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 입력받은 해상도에 맞게 윈도우 크기 조절
	RECT rt = { 0, 0, m_ptResolution.x, m_ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);


	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(m_hWnd, nullptr, (width - rt.right) / 2, (height - rt.bottom) / 2, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hWnd);

	// 이중 버퍼링 용도의 비트맵과 DC를 만든다
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	CKeyMgr::GetInst()->init();
	CScene::GetInst()->Enter();

	m_pScene = CScene::GetInst();

	return S_OK;
}

void CCore::progress()
{
	// ==============
	// Manager Update
	// ==============
	CKeyMgr::GetInst()->update(); // 키 상태 체크

	// ============
	// Scene Update
	// ============
	CScene::GetInst()->update(); // 현재 씬 돌리기


	// ==========
	// Rendering
	// ==========
	// 화면 Clear
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);


	// ==========
	// 제목표시줄
	// ==========

	wchar_t szBuffer[255] = {};
	HWND hWnd = GetFocus();

	// 포커스 O
	if (nullptr != hWnd)
	{
		isFocused = true;
		swprintf_s(szBuffer, FOCUSED_TEXT);
	}
	else
	{
		isFocused = false;
		if (m_bMousetracking)
		{
			swprintf_s(szBuffer, UNFOCUSED_TEXT);
		}
		else
			swprintf_s(szBuffer, FOCUSED_TEXT);
	}

	/*
	// 배경에 따른 표정변화
	if (1 == m_pScene->m_pBackground->GetIdx())
	{
		m_pScene->m_pCharacter->SetIdx(2);
		swprintf_s(szBuffer, BLACK_TEXT);
	}
	*/

	SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);

	CScene::GetInst()->render(m_memDC);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_memDC, 0, 0, SRCCOPY);
}

void CCore::ChangeMTMode()
{
	m_bMousetracking = !m_bMousetracking;
}