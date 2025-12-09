#include "global.h"
#include "framework.h"
#include "EyeChaser.h"

#include "CCore.h"

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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 메모리 누수 체크
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(285);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

    LoadStringW(hInstance, IDC_GAZECOMMISSION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    //core 객체 초기화
    if (FAILED(CCore::GetInst()->init(g_hWnd, POINT{ WINDOW_SIZE, WINDOW_SIZE })))
    {
        MessageBox(nullptr, L"Core 객체 초기화 실패", L"ERROR", MB_OK);

        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAZECOMMISSION));

    MSG msg;

    // 기본 메시지 루프입니다:
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
            CCore::GetInst()->progress();
        }
    }

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
    wcex.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    wcex.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

    // 기본아이콘
    //wcex.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    //wcex.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
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
        return FALSE;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool isAlwaysOnTop = false;
    static bool isMousetracking = true;

    switch (message)
    {
    case WM_CREATE:
    {
        // 아이콘
        //HICON hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED); // 기본아이콘
        HICON hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
        SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        //SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon2);
    }
    break;
    case WM_RBUTTONDOWN: {
        // 우클릭 시 팝업 메뉴 표시


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
            break;
        case ID_MOUSETRACKIG:
            // 마우스트래킹 ON/OFF
            isMousetracking = !isMousetracking;
            CCore::GetInst()->ChangeMTMode();
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
    case WM_CLOSE: {
        // 종료 확인 대화 상자 표시
        if (EXIT_WINDOW)
        {
            int result = MessageBox(hWnd, CLOSE_DIALOG, CLOSE_DIALOG_TITLE, MB_YESNO);
            if (result == IDYES) {
                DestroyWindow(hWnd);
            }
            break;
        }
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
