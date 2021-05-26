#include "framework.h"

#include "resource.h"
#include "3D.h"

int ScreenWidth;   //Разрешение экрана по вертикали
int ScreenHeight;  //Разрешение экрана по горизонтали

double window_size = 1;

ViewPoint viewPoint = {500,270,90};
Object* main_object = NULL;

#define MAX_LOADSTRING 100
HINSTANCE hInst;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount){
    ScreenWidth = GetSystemMetrics(SM_CXSCREEN)/2;
    ScreenHeight = GetSystemMetrics(SM_CYSCREEN)/2;

    FILE* obj = fopen("./Konus.obj","r");
    main_object = parse_obj(obj);

    // Register the window class
    const char *CLASS_NAME = "myWin32WindowClass";
    WNDCLASS wc{};
    wc.hInstance = currentInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);

    // Create the window
    CreateWindow(CLASS_NAME, "3D",
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style
                 CW_USEDEFAULT, CW_USEDEFAULT,                // Window initial position
                 ScreenWidth, ScreenHeight,                                    // Window size
                 nullptr, nullptr, nullptr, nullptr);


    // Цикл основного сообщения:
    HACCEL hAccelTable = LoadAccelerators(currentInstance, MAKEINTRESOURCE(IDC_LAB1));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    const double RotateAngle = 1;

    viewPoint.d = viewPoint.R/500;

    switch (message)
    {
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case 'A':
            viewPoint.A += RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case 'D':
            viewPoint.A -= RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case 'W':
            viewPoint.B += RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case 'S':
            viewPoint.B -= RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case 'X': {
            window_size *= 1.5;
            ScreenWidth *= 1.5;
            ScreenHeight *= 1.5;
            SetWindowPos(
                hWnd,
                HWND_TOP,
                0,
                0,
                ScreenWidth,
                ScreenHeight,
                SWP_NOMOVE
            );
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        }
        case 'Z': {
            window_size /= 1.5;
            ScreenWidth /= 1.5;
            ScreenHeight /= 1.5;
            SetWindowPos(
                hWnd,
                HWND_TOP,
                0,
                0,
                ScreenWidth,
                ScreenHeight,
                SWP_NOMOVE
            );
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        }
        case 'R':
            viewPoint.R += viewPoint.R/5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case 'T':
            viewPoint.R -= viewPoint.R/5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case 'F':
            viewPoint.d += viewPoint.d/5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        case 'G':
            viewPoint.d -= viewPoint.d/5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
            break;
        }
    } break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        main_object->update(viewPoint,ScreenHeight,ScreenWidth);
        main_object->draw(hdc);

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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