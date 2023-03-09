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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_show_cmd)
{
    LPCTSTR CLASS_NAME = "Docker CLI Class";
    WNDCLASS wc;

    memset(&wc, 0, sizeof(wc));

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = h_instance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = create_main_window(CLASS_NAME, "Docker CLI", h_instance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    HICON ico;
    ico = load_icon(NULL, "..\\settings.ico");

    if (!ico)
        perror_win("LoadImage");

    NOTIFYICONDATA nicon;
    nicon.cbSize = sizeof(nicon);
    nicon.hWnd = hwnd;
    nicon.uID = 123456781;
    // nicon.uFlags = NIF_GUID;
    nicon.hIcon = ico;
    strcpy(nicon.szTip, "Docker CLI");
    nicon.dwState = NIS_SHAREDICON;
    strcpy(nicon.szInfo, "Docker CLI is starting...");
    strcpy(nicon.szInfoTitle, "Docker CLI");
    nicon.dwInfoFlags = NIIF_INFO;
    // nicon.guidItem = "e039d038-d9f3-432b-a8e0-97020e27987d";

    BOOL status = Shell_NotifyIconA(NIM_ADD, &nicon);
#ifdef __DEBUG
    printf("%d\n", status);
#endif
    if (!status) {
        perror_win("Notify Icon");
        return -1;
    }
    HWND cwin = GetConsoleWindow();
    
    /* we sleep one second so we give time to load resources, if we don't we got a random behaviour */
    Sleep(1000);
    BOOL close_status = CloseWindow(cwin);
    if (!close_status) {
        perror_win("Close Window");
    }
    FreeConsole();
    BOOL destroy_status = DestroyWindow(cwin);
    if (!destroy_status) {
        perror_win("Destroy Window");
    }
    // ShowWindow(hwnd, n_show_cmd);

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    switch (u_msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, u_msg, w_param, l_param);
}
