#include <stdio.h>
#include <windows.h>
#include "gui.h"

#define APPLICATION_NAME "Docker CLI"

#ifndef UNICODE
#define UNICODE
#endif

static pthread_t daemon_tid;

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
    HWND hwnd = create_main_window(CLASS_NAME, APPLICATION_NAME, h_instance, NULL);

    if (hwnd == NULL) {
        win_system_error("Create main window");
        return DOCKERCLIE_SYSTEM;
    }

#ifdef __DEBUG
    printf("%d\n", status);
#endif

    char docker_path[MAX_PATH], icon_path[MAX_PATH];
    sprintf(docker_path, "%s%s", getenv("USERPROFILE"), "\\docker-cli");

    DOCKERCLI_CODE status;
    status = create_tray_icon(hwnd);

    if (status != DOCKERCLIE_OK) {
        if (status == DOCKERCLIE_SYSTEM) {
            win_system_error("Create tray icon");
        }
        return status;
    }
/*
    HWND cwin = GetConsoleWindow();

    BOOL close_status = CloseWindow(cwin);
    if (!close_status) {
        win_system_error("Close Window");
    }

#ifndef __DEBUG
    FreeConsole();
#endif

    BOOL destroy_status = DestroyWindow(cwin);
    if (!destroy_status) {
        win_system_error("Destroy Window");
    }
*/
    char daemon_path[MAX_PATH];
    // const char *docker_cli_home = getenv("DOCKER_CLI_HOME");
    sprintf(daemon_path, "%s\\daemon", docker_path);

    status = init_daemon(daemon_path, NULL, &daemon_tid);
    if (status != DOCKERCLIE_OK) {
        docker_cli_error(status);
        return status;
    }

#ifdef __DEBUG
    printf("Init daemon: %d\n", status);
    printf("Process id: %d\n", pinfo.dwProcessId);
#endif

    Sleep(5000);

    check_daemon_status();
    printf("dockerd running\n");

    SetForegroundWindow(GetConsoleWindow());
    // ShowWindow(GetConsoleWindow(), SW_HIDE);

    // CloseWindow(GetConsoleWindow());
    // FreeConsole();
    // DestroyWindow(GetConsoleWindow());
    
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
    HMENU hMenu = LoadMenuA(hinst, MAKEINTRESOURCE(IDR_TRAY_POPUPMENU));

    if (!hMenu) {
        win_system_error("hmenu");
    }

    if (hMenu)
    {
        HMENU hSubMenu = GetSubMenu(hMenu, 0);
        if (hSubMenu)
        {
            ClientToScreen(hwnd, (LPPOINT) &pt);
            // fix: menu orientation
            TrackPopupMenuEx(
                hSubMenu,
                TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_HORPOSANIMATION | TPM_VERNEGANIMATION | TPM_RETURNCMD,
                pt.x,
                pt.y,
                hwnd,
                NULL
            );
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
            ShowWindow(hwnd, SW_SHOW);
            break;
        case WM_CONTEXTMENU:
            POINT pt; // = { LOWORD(w_param), HIWORD(w_param) };
            GetCursorPos(&pt);
#ifdef __DEBUG
            printf("x: %d - y: %d\n", pt.x, pt.y);
#endif
            ShowContextMenu(hwnd, pt);
            break;
        default:
#ifdef __DEBUG
            printf("X %d\n", LOWORD(w_param));
            printf("Event %d\n", LOWORD(l_param));
#endif
        }
        break;
    case WM_CLOSE:
        if (MessageBox(hwnd, "Really quit?", APPLICATION_NAME, MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
            // pthread_join(daemon_tid, NULL);
            // PostQuitMessage(0);
        }
        break;
    case WM_DESTROY:
        // DestroyWindow(hwnd);
        // pthread_join(daemon_tid, NULL);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, u_msg, w_param, l_param);
    }

    return 0;
}
