#include "global.h"
#include "CCore.h"
#include "CObject.h"
#include "CKeyMgr.h"
#include "CScene.h"
#include "CChar.h"
#include "CBackground.h"
#include "resource.h"

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

int CCore::init(HWND _hWnd, HINSTANCE _hInst, POINT _ptResolution)
{
	m_iratio = (float)WINDOW_SIZE / (float)CANVAS_SIZE;

	// GDI+ 초기화
	ULONG_PTR gdiplusToken = 0;
	Gdiplus::GdiplusStartupInput gdistartupinput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdistartupinput, nullptr);

	m_hWnd = _hWnd;
	m_hInst = _hInst;
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
	CKeyMgr::GetInst()->update(); // 키 상태 체크

	CScene::GetInst()->update(); // 현재 씬 돌리기

	// 화면 클리어
	RECT rt = { 0, 0, m_ptResolution.x, m_ptResolution.y };
	FillRect(m_memDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));

	HWND hWnd = GetFocus();
	if (nullptr != hWnd)
	{
		isFocused = true;
	}
	else
	{
		isFocused = false;
	}

	// 제목표시줄 업데이트
	static bool prevFocused = false; // false로 초기화 (첫 실행 시 업데이트 보장)
	static bool prevMouseTracking = !m_bMousetracking; // 현재 값과 다르게 초기화
	bool curFocused = (hWnd != nullptr);

	// 포커스 상태나 마우스트래킹 모드가 변경되었을 때만 타이틀 업데이트
	if (curFocused != prevFocused || m_bMousetracking != prevMouseTracking)
	{
		wchar_t szBuffer[32] = {};
		if (curFocused)
		{
			LoadStringW(m_hInst, IDS_FOCUSED_TEXT, szBuffer, _countof(szBuffer));
		}
		else
		{
			if (m_bMousetracking)
				LoadStringW(m_hInst, IDS_UNFOCUSED_TEXT, szBuffer, _countof(szBuffer));
			else
				LoadStringW(m_hInst, IDS_FOCUSED_TEXT, szBuffer, _countof(szBuffer));
		}
		SetWindowText(m_hWnd, szBuffer);
		prevFocused = curFocused;
		prevMouseTracking = m_bMousetracking;
	}

	static Gdiplus::Graphics* graphics = nullptr;
	if (!graphics)
	{
		graphics = new Gdiplus::Graphics(m_memDC);
		graphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
		//graphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
	}
	CScene::GetInst()->render(m_memDC, graphics);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_memDC, 0, 0, SRCCOPY);
}

void CCore::ChangeMTMode()
{
	m_bMousetracking = !m_bMousetracking;
}