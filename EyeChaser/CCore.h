#pragma once
class CScene;

class CCore
{
	SINGLE(CCore);
private:
	HWND	m_hWnd;			// 메인 윈도우 핸들
	HINSTANCE m_hInst;
	POINT	m_ptResolution; // 메인 윈도우 해상도
	HDC		m_hDC;			// 메인 윈도우에 Draw 할 DC

	HBITMAP m_hBit;			// 버퍼 비트맵
	HDC		m_memDC;		// 버퍼에 Draw 할 DC

	CScene* m_pScene;

	bool isFocused; // 창의 포커스 여부
	bool m_bMousetracking; // 마우스트래킹 여부

	float m_iratio;

public: //코어 클래스의 초기화 함수를 만들자
//실패 체크 위해 정수 값 반환하자
	int init(HWND _hWnd, HINSTANCE _hInst, POINT _ptResolution);
	void progress(); //메시지가 없는 동안의 코어 클래스의 작업
	bool GetIsFocused() { return isFocused; }
	float GetRatio() { return m_iratio; }
	bool GetMTMode() { return m_bMousetracking; }
	void ChangeMTMode();

public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	POINT GetResolution() { return m_ptResolution; }
};

