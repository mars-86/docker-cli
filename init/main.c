#include <stdio.h>
#include <windows.h>
#include "init.h"

void perror_win(const char *msg)
{
        WCHAR *buff;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buff, 0, NULL);
        fprintf(stderr, "%s: %S\n", msg, buff);
        LocalFree(buff);
}

#ifndef UNICODE
#define UNICODE
#endif

LRESULT CALLBACK windowProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param);

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_show_cmd)
{
    LPCTSTR CLASS_NAME = "Docker CLI Class";
    WNDCLASS wc;

    memset(&wc, 0, sizeof(wc));

    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = h_instance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = create_main_window(CLASS_NAME, "Docker CLI", h_instance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

#ifdef __DEBUG
    printf("%d\n", status);
#endif

    HICON ico;
    ico = load_icon(NULL, "..\\settings.ico");

    if (!ico)
        printf("ERROR");

    NOTIFYICONDATA nicon;
    nicon.cbSize = sizeof(nicon);
    nicon.hWnd = hwnd;
    nicon.uID = 123456781;
    // nicon.uFlags = NIF_GUID;
    nicon.hIcon = ico;
    strcpy(nicon.szTip, "Docker CLI");
    nicon.dwState = NIS_SHAREDICON;
    nicon.uCallbackMessage =  (UINT)WM_NOTIFYCALLBACK;
    strcpy(nicon.szInfo, "Docker CLI is starting...");
    strcpy(nicon.szInfoTitle, "Docker CLI");
    nicon.dwInfoFlags = NIIF_INFO;
    // nicon.guidItem = (GUID){{0xE0, 0x39, 0xD0, 0x38}, {0xD9, 0xF3}, {0x43, 0x2B}, {"97020e27"}};

    WINBOOL status = Shell_NotifyIconA(NIM_ADD, &nicon);
    if (!status)
        printf("ERROR");

    nicon.uVersion = NOTIFYICON_VERSION_4;
    Shell_NotifyIconA(NIM_SETVERSION, &nicon);
    // WINBOOL status = create_tray_icon(hwnd);

    if (!status) {
        perror_win("Notify ICON");
        system("PAUSE");
        return ESYSTEM;
    }

    /* we sleep one second so we give time to load resources, if we don't we get a random behavior */
    Sleep(1000);
    HWND cwin = GetConsoleWindow();

    BOOL close_status = CloseWindow(cwin);
    if (!close_status) {
        perror_win("Close Window");
    }
    FreeConsole();
    BOOL destroy_status = DestroyWindow(cwin);
    if (!destroy_status) {
        perror_win("Destroy Window");
    }

    Sleep(1000);
    init_daemon("..\\..\\daemon\\bin", NULL);
    // ShowWindow(hwnd, n_show_cmd);

    Sleep(1000);
    // Run the message loop.
    MSG msg = {};
    BOOL bRet;
    while (bRet = GetMessage(&msg, hwnd, 0, 0) > 0)
    {
        if (bRet == -1)
        {
            printf("ERROR");
            // handle the error and possibly exit
        }
        else
        {
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        }
    }

    return msg.wParam;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    switch (u_msg) {
    case WM_CREATE:
        break;
    case WM_NOTIFYCALLBACK:
        switch (LOWORD(l_param))
        {
        case 515:
            printf("DOUBLE CLICK");
            // ShowWindow(hwnd, SW_SHOW);
            break;
        case WM_MOUSEFIRST:
            printf("MOUSE FIRST");
            break;
        default:
            printf("Event\n");
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, u_msg, w_param, l_param);
    }

    return 0;
}
