#include <stdio.h>
#include <windows.h>
#include "gui.h"

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

HINSTANCE hinst;
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

    hinst = h_instance;
    HWND hwnd = create_main_window(CLASS_NAME, "Docker CLI", h_instance, NULL);

    if (hwnd == NULL)
        return ESYSTEM;

#ifdef __DEBUG
    printf("%d\n", status);
#endif

    char docker_path[MAX_PATH], icon_path[MAX_PATH];
    sprintf(docker_path, "%s%s", getenv("USERPROFILE"), "\\docker-cli");

    int status = create_tray_icon(hwnd);

    if (status != EOK) {
        perror_win("Create tray icon");
        return status;
    }
/*
    HWND cwin = GetConsoleWindow();

    BOOL close_status = CloseWindow(cwin);
    if (!close_status) {
        perror_win("Close Window");
    }

#ifndef __DEBUG
    FreeConsole();
#endif

    BOOL destroy_status = DestroyWindow(cwin);
    if (!destroy_status) {
        perror_win("Destroy Window");
    }
*/
    Sleep(1000);

    char daemon_path[MAX_PATH];
    // const char *docker_cli_home = getenv("DOCKER_CLI_HOME");
    sprintf(daemon_path, "%s\\daemon", docker_path);

    PROCESS_INFORMATION pinfo;
    status = init_daemon(daemon_path, NULL, &pinfo);

#ifdef __DEBUG
    printf("Init daemon: %d\n", status);
    printf("Process id: %d\n", pinfo.dwProcessId);
#endif

    ResumeThread(pinfo.hThread);
    // system(daemon_path);
    // printf("daemon running\n");
    Sleep(1000);

    printf("dockerd running\n");
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

void ShowContextMenu(HWND hwnd, POINT pt)
{
    HMENU hMenu = LoadMenuA(hinst, MAKEINTRESOURCE(0));

    if (!hMenu) {
        perror_win("hmenu");
    }

    if (hMenu)
    {
        HMENU hSubMenu = GetSubMenu(hMenu, 0);
        if (hSubMenu)
        {
            // our window must be foreground before calling TrackPopupMenu or the menu will not disappear when the user clicks away
            SetForegroundWindow(hwnd);

            // respect menu drop alignment
            UINT uFlags = TPM_RIGHTBUTTON;
            if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0)
            {
                uFlags |= TPM_RIGHTALIGN;
            }
            else
            {
                uFlags |= TPM_LEFTALIGN;
            }

            TrackPopupMenuEx(hSubMenu, uFlags, pt.x, pt.y, hwnd, NULL);
        }
        DestroyMenu(hMenu);
    }
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    switch (u_msg) {
    case WM_CREATE:
        // here should be the initialization process
        break;
    case WM_NOTIFYCALLBACK:
        switch (LOWORD(l_param)) {
        case WM_LBUTTONDBLCLK:
            // ShowWindow(hwnd, SW_SHOW);
            break;
        case WM_CONTEXTMENU:
            POINT pt = { LOWORD(w_param), HIWORD(w_param) };
            ShowContextMenu(hwnd, pt);
            break;
        default:
#ifdef __DEBUG
            printf("X %d\n", LOWORD(w_param));
            printf("Event %d\n", LOWORD(l_param));
#endif
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
