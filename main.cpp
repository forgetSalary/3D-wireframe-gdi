#include "resource.h"
#include "3D.h"

namespace env{
    int ScreenWidth = GetSystemMetrics(SM_CXSCREEN)/1.5;   //Разрешение экрана по вертикали
    int ScreenHeight = GetSystemMetrics(SM_CYSCREEN)/1.5;  //Разрешение экрана по горизонтали

    double window_size = 1;

    Camera main_camera = {{600, -180, 90},
                          {0,0,0},
                          {0,0}};


    double dc = 1000;//distance from screen to camera centre

    arena_t objects_arena = EMPTY_ARENA;
    Object* main_object = NULL;
}

using namespace env;

#define MAX_LOADSTRING 100
HINSTANCE hInst;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount){
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    printf("Importing object...\n");
    main_object = parse_obj(&objects_arena,"./ice.obj");
    printf("Object has been imported\n\n");

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc,hdc1;

    const double RotateAngle = 1;

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
        case 'W':
            main_camera.center_position.theta += RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 'S':
            main_camera.center_position.theta -= RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 'A':
            main_camera.center_position.phi += RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 'D':
            main_camera.center_position.phi -= RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
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

        case VK_F5:{
            printf("Camera phi angle:%f\n",main_camera.center_position.phi);
            printf("Camera theta angle:%f\n",main_camera.center_position.theta);
            printf("Camera distance to center:%f\n",main_camera.center_position.r);
            TaitBryanAngles C = main_camera.orientation;
            printf("Camera orientation:%f,%f,%f\n",C.x,C.y,C.z);
            printf("Distance to screen:%f\n\n",dc);
            break;
        }
        case VK_UP:
            main_camera.offsets.y -= 10;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_DOWN:
            main_camera.offsets.y += 10;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_LEFT:
            main_camera.offsets.x -= 10;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RIGHT:
            main_camera.offsets.x += 10;
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case 'R':
            dc *= 2;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 'T':
            dc /= 2;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 'F':
            main_camera.center_position.r /= 2;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 'G':
            main_camera.center_position.r *= 2;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
    } break;
    case WM_PAINT:{
        hdc = BeginPaint(hWnd, &ps);

        main_object->update(main_camera,dc,ScreenHeight,ScreenWidth);
        draw_ground(hdc,main_camera,dc,ScreenHeight,ScreenWidth);
        draw_coordinate_lines(hdc, main_camera, dc, ScreenHeight, ScreenWidth);
        main_object->draw(hdc,0);

        EndPaint(hWnd, &ps);
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