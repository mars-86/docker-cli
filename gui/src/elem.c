#include "../inc/elem.h"
#include <stdio.h>

HICON load_icon(HINSTANCE hinst, const char *path)
{
    return (HICON)LoadImageA(
        hinst,
        path,
        IMAGE_ICON,
        32,
        32,
        LR_LOADFROMFILE
    );
}

HWND create_main_window(LPCSTR lpClassName, LPCSTR lpWindowName, HINSTANCE hInstance, LPVOID lpParam)
{
    return CreateWindowExA(
        0,
        lpClassName,
        lpWindowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,    
        NULL,
        hInstance,
        lpParam
    );
}

int create_tray_icon(HWND hwnd)
{
    const char tooltip_name[] = "Docker CLI";
    const char tooltip_info[] = "Docker CLI is starting...";
    const char tooltip_info_title[] = "Docker CLI";

    char docker_path[MAX_PATH], icon_path[MAX_PATH];
    sprintf(docker_path, "%s%s", getenv("USERPROFILE"), "\\docker-cli");
    sprintf(icon_path, "%s\\settings.ico", docker_path);

#ifdef __DEBUG
    printf("%s\n", icon_path);
#endif

    HICON ico;
    ico = load_icon(NULL, icon_path);

    if (!ico)
        return DOCKERCLIE_SYSTEM;

    NOTIFYICONDATA nicon;
    nicon.cbSize = sizeof(nicon);
    nicon.hWnd = hwnd;
    nicon.uID = 123456781;
    nicon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_STATE | NIF_INFO; // NIF_GUID;
    nicon.hIcon = ico;
    strcpy(nicon.szTip, "Docker CLI");
    nicon.dwState = NIS_SHAREDICON;
    nicon.dwStateMask = NIS_SHAREDICON;
    nicon.uCallbackMessage =  (UINT)WM_NOTIFYCALLBACK;
    strcpy(nicon.szInfo, "Docker CLI is starting...");
    strcpy(nicon.szInfoTitle, "Docker CLI");
    nicon.dwInfoFlags = NIIF_INFO;
    // nicon.guidItem = (GUID){{0xE0, 0x39, 0xD0, 0x38}, {0xD9, 0xF3}, {0x43, 0x2B}, {"97020e27"}};

    WINBOOL status;

    status = Shell_NotifyIconA(NIM_ADD, &nicon);
    if (!status)
        return DOCKERCLIE_SYSTEM;

    nicon.uVersion = NOTIFYICON_VERSION_4;
    status = Shell_NotifyIconA(NIM_SETVERSION, &nicon);
    if (!status)
        return DOCKERCLIE_SYSTEM;

    return DOCKERCLIE_OK;
}
