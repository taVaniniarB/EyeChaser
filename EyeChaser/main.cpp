#include "global.h"
#include "framework.h"
#include "EyeChaser.h"

#include "CCore.h"
#pragma comment(lib, "winmm.lib")

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND    g_hWnd; // 메인 윈도우 핸들
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


std::string GetWindowsVersion()
{
	typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
	HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
	if (!hMod) return "Unknown";

	auto fn = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
	if (!fn) return "Unknown";

	RTL_OSVERSIONINFOW info = { sizeof(RTL_OSVERSIONINFOW) };
	if (fn(&info) != 0) return "Unknown"; // STATUS_SUCCESS 체크

	std::string osName = "Windows ";

	// Windows 11 판별 (Major 10, Build 22000 이상)
	if (info.dwMajorVersion == 10 && info.dwBuildNumber >= 22000) {
		osName += "11";
	}
	else {
		osName += std::to_string(info.dwMajorVersion);
	}

	return osName + " (" +
		std::to_string(info.dwMajorVersion) + "." +
		std::to_string(info.dwMinorVersion) + "." +
		std::to_string(info.dwBuildNumber) + ")";
}
void LogArchitecture()
{
	BOOL isWow64 = FALSE;
	IsWow64Process(GetCurrentProcess(), &isWow64);

#ifdef _WIN64
	LOG("프로세스: 64bit");
#else
	LOG(std::string("프로세스: 32bit, WOW64: ") + (isWow64 ? "Yes" : "No"));
#endif
}
bool IsRunAsAdmin()
{
	BOOL isAdmin = FALSE;
	PSID adminGroup;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

	AllocateAndInitializeSid(
		&NtAuthority, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&adminGroup
	);

	CheckTokenMembership(NULL, adminGroup, &isAdmin);
	FreeSid(adminGroup);
	return isAdmin;
}
std::string GetExePath()
{
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	return path;
}
std::string GetWorkingDirectory()
{
	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);
	return path;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 메모리 누수 체크
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(285);

	/*LOG_CLEAR();
	MessageBox(NULL, L"진입했습니다.", L"Test", MB_OK | MB_TOPMOST);
	LOG("=== 시작 ===");*/

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/*LogArchitecture();
	LOG("Windows 버전: " + GetWindowsVersion());
	LOG(std::string("관리자 권한: ") + (IsRunAsAdmin() ? "Yes" : "No"));
	LOG("Exe 경로: " + GetExePath());
	LOG("작업 디렉토리: " + GetWorkingDirectory());

	LOG("===========");*/

	// 전역 문자열을 초기화합니다.
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadStringW(hInstance, IDC_GAZECOMMISSION, szWindowClass, MAX_LOADSTRING);
	// 전역 문자열 초기화 후 체크
	if (LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0)
	{
		MessageBox(NULL, L"타이틀 로드 실패", L"Error", MB_OK);
		LOG("타이틀 로드 실패");
	}

	if (LoadStringW(hInstance, IDC_GAZECOMMISSION, szWindowClass, MAX_LOADSTRING) == 0)
	{
		MessageBox(NULL, L"클래스 이름 로드 실패", L"Error", MB_OK);
		LOG("클래스 이름 로드 실패");
	}

	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	LOG("InitInstance 시작");
	if (!InitInstance(hInstance, nCmdShow))
	{
		LOG("InitInstance Failed");
		MessageBox(nullptr, L"InitInstance 실패", L"ERROR", MB_OK);
		system("pause");

		return FALSE;
	}
	//core 객체 초기화
	LOG("코어 객체 초기화 시작");
	if (FAILED(CCore::GetInst()->init(g_hWnd, hInst, POINT{ WINDOW_SIZE, WINDOW_SIZE })))
	{
		LOG("Core 객체 초기화 실패");
		MessageBox(nullptr, L"Core 객체 초기화 실패", L"ERROR", MB_OK);
		system("pause");
		return FALSE;
	}
	LOG("코어 객체 초기화 완료");
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAZECOMMISSION));

	MSG msg;

	// 기본 메시지 루프입니다:
	timeBeginPeriod(1);

	DWORD prevTime = (DWORD)GetTickCount64();
	LOG("=== 메시지 루프 진입 ===");
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			DWORD curTime = (DWORD)GetTickCount64();
			if (curTime - prevTime >= 33)
			{
				CCore::GetInst()->progress();
				prevTime = curTime;
			}
			else
			{
				Sleep(1);
			}
		}
	}

	// 프로그램 종료 시
	timeEndPeriod(1);

	return (int)msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;

	// 아이콘
	HICON ic = LoadIcon(NULL, IDI_WINLOGO);
	switch (ICON_TYPE)
	{
	case EIconType::Image:
		ic = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		break;
	case EIconType::Windows:
		ic = LoadIcon(NULL, IDI_WINLOGO);
		break;
	case EIconType::Error:
		ic = LoadIcon(NULL, IDI_ERROR);
		break;
	case EIconType::Warning:
		ic = LoadIcon(NULL, IDI_WARNING);
		break;
	default:
		break;
	}
	wcex.hIcon = ic;		// 큰 아이콘
	wcex.hIconSm = ic;     // 작은 아이콘

	// hIconSm도 같은 방식으로 처리
	//if(ICON_DEFAULT)
	//{
	//	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO); // 큰 아이콘
	//	wcex.hIconSm = LoadIcon(NULL, IDI_WINLOGO);     // 작은 아이콘 (또는 IDI_APPLICATION)
	//	//wcex.hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	//	//wcex.hIconSm = (HICON)LoadImage(NULL, MAKEINTRESOURCE(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	//}
	//else
	//{
	//	wcex.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	//	wcex.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	//}

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	SetLastError(0);
	g_hWnd = CreateWindow(
		szWindowClass, // 창 클래스 이름
		szTitle,       // 창 제목
		WS_OVERLAPPED | WS_SYSMENU,     // 창 스타일 (크기 고정)
		(width - WINDOW_SIZE) / 2,           // 창 초기 위치 (X)
		(height - WINDOW_SIZE) / 2,           // 창 초기 위치 (Y)
		WINDOW_SIZE,                    // 창 너비
		WINDOW_SIZE,                    // 창 높이
		nullptr,                   // 부모 창 핸들
		nullptr,                   // 메뉴 핸들
		hInstance,              // 인스턴스 핸들
		nullptr                    // 추가 창 생성 데이터
	);

	if (!g_hWnd)
	{
		LOG("윈도우 생성 실패");
		LOG("오류 코드: " + std::to_string(GetLastError()));
		return FALSE;
	}

	RECT rt = { 0, 0, WINDOW_SIZE, WINDOW_SIZE };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(g_hWnd, nullptr, (width - rt.right) / 2, (height - rt.bottom) / 2, rt.right - rt.left, rt.bottom - rt.top, 0);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	LOG("초기 윈도우 위치: " + std::to_string((width - rt.right) / 2) + ", " + std::to_string((height - rt.bottom) / 2));
	LOG("모니터: " + std::to_string(width) + "x" + std::to_string(height));

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool isAlwaysOnTop = false;
	static bool isMousetracking = true;

	switch (message)
	{
	case WM_ERASEBKGND:
		// 배경 지우기를 건너뛰고 직접 처리함을 알림
		return 1;

	case WM_CREATE:
	{
		// 아이콘
		//HICON hIcon;
		//if(ICON_DEFAULT) // 기본아이콘
		//{
		//	hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_WINLOGO), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		//}
		//else // 이미지 아이콘
		//{
		//	hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		//}

//#ifdef USE_CUSTOM_ICON
//// 커스텀 아이콘 리소스가 포함된 경우
//		hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
//#else
//	// 리소스가 없으므로 시스템 기본 아이콘을 로드 (hInst를 NULL로 설정)
//		hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
//#endif
//
//		// hIconSm도 같은 방식으로 처리
//		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	}
	break;
	case WM_RBUTTONDOWN: {
		// 우클릭 시 팝업 메뉴 표시
		LOG("우클릭 메뉴 표시");
		HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
		if (hMenu) {
			HMENU hSubMenu = GetSubMenu(hMenu, 0);

			if (isAlwaysOnTop) {
				CheckMenuItem(hSubMenu, ID_TOGGLE_ALWAYS_ON_TOP, MF_CHECKED);
			}
			else {
				CheckMenuItem(hSubMenu, ID_TOGGLE_ALWAYS_ON_TOP, MF_UNCHECKED);
			}

			if (isMousetracking) {
				CheckMenuItem(hSubMenu, ID_MOUSETRACKIG, MF_CHECKED);
			}
			else {
				CheckMenuItem(hSubMenu, ID_MOUSETRACKIG, MF_UNCHECKED);
			}

			if (hSubMenu) {
				POINT pt;
				GetCursorPos(&pt);
				TrackPopupMenu(hSubMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
			}
			DestroyMenu(hMenu);
		}
	}
					   break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case ID_TOGGLE_ALWAYS_ON_TOP:
			// 창을 맨 앞에 두기
			isAlwaysOnTop = !isAlwaysOnTop;
			SetWindowPos(hWnd, isAlwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			LOG("ALWAYS_ON_TOP: " + std::string(isAlwaysOnTop ? "ON" : "OFF"));
			break;
		case ID_MOUSETRACKIG:
			// 마우스트래킹 ON/OFF
			isMousetracking = !isMousetracking;
			CCore::GetInst()->ChangeMTMode();
			LOG("MOUSETRACKING: " + std::string(isMousetracking ? "ON" : "OFF"));
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	// 종료 다이얼로그
	case WM_CLOSE:
	{
		if (EXIT_WINDOW)
		{
			WCHAR szDialog[512] = {};
			WCHAR szTitle[128] = {};

			LoadStringW(hInst, IDS_CLOSE_DIALOG, szDialog, _countof(szDialog));
			LoadStringW(hInst, IDS_CLOSE_DIALOG_TITLE, szTitle, _countof(szTitle));

			int result = MessageBoxW(
				hWnd,
				szDialog,
				szTitle,
				MB_YESNO
			);
			switch (result)
			{
			case IDYES:
				DestroyWindow(hWnd);
				break;
			case IDCANCEL:
				// TODO: add code
				break;
			case IDTRYAGAIN:
				// TODO: add code
				break;
			case IDCONTINUE:
				// TODO: add code
				break;
			}
		}
		else
		{
			DestroyWindow(hWnd);
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
